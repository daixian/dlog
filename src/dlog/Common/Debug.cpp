#include "Debug.h"
#include "FileHelper.h"
#include "JsonHelper.h"

#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>
#include <Poco/Format.h>
#include "Poco/Glob.h"
#include "Poco/Timespan.h"
#include "Poco/FileStream.h"
#include "Poco/Process.h"

#include "Common.h"
#include <iostream>
#include <regex>

using namespace std;
using namespace Poco;

namespace dlog {

Debug* Debug::m_pInstance = new Debug();

void Debug::Init(const char* logDir, const char* program, INIT_RELATIVE rel)
{
    if (!isEnable) {
        return;
    }

    mt.lock();
    if (isInit) {
        mt.unlock();
        return;
    }

    try {
        // 设置日志输出格式
        spdlog::set_pattern(logPattern);

        programName = program; // 记录程序名
        logDirPath.clear();
        logFilePath.clear();

        // 初始化控制台
        setIsConsoleEnable(isConsoleEnable);

        // 初始化文件日志
        if (isFileEnable) {
            Path inputDir = Path::forDirectory(logDir);

#if WIN32
            // 在windows下如果是相对地址,或者是没有盘符.如果是相对对路径,就从根目录去拼接
            if (inputDir.isRelative() || inputDir.getDevice().empty()) {

#else
            if (inputDir.isRelative()) {
#endif
                if (rel == INIT_RELATIVE::MODULE) {
                    FileHelper::makeAbsolute(Path(FileHelper::getModuleDir()), inputDir); // 模块目录
                    this->logDirPath = inputDir.toString();
                }
                else {
                    FileHelper::makeAbsolute(Path(FileHelper::getAppDir()), inputDir); // app目录
                    this->logDirPath = inputDir.toString();
                }
            }
            else {
                // 如果是绝对路径,就直接使用
                this->logDirPath = inputDir.toString();
            }

            if (!FileHelper::dirExists(this->logDirPath)) // 如果文件夹不存在
            {
                Path wantPath(this->logDirPath);
                if (wantPath.isFile()) {
                    // 如果它又已经被一个文件占用了文件名,那就在这个文件夹下使用log文件夹
                    this->logDirPath = wantPath.parent().append("log").toString();
                }
                FileHelper::isExistsAndCreat(this->logDirPath); // 如果文件夹不存在就创建
            }

            // 增加一个pid，防止不同进程多次调用这个日志文件
            uint32_t pid = Poco::Process::id();
            string logFileName = Poco::format("%s.%s[%u].log", programName, Common::secTimeStr(), pid);
            logFilePath = Path(logDirPath).append(logFileName).toString();

            // 如果使用写入UTF8BOM那么就写入3个字节的bom头
            if (isUTF8BOM) {
                Poco::FileOutputStream fs(logFilePath);
                const unsigned char bom[3] = {0xef, 0xbb, 0xbf};
                fs.write((char*)bom, 3);
                fs.close();
            }
#if WIN32
            // 这个函数居然不支持utf8编码的文件名,在CMake文件中定义SPDLOG_WCHAR_FILENAMES可以解决,这个函数只在win下有效.
            filelogger = spdlog::basic_logger_mt(programName, JsonHelper::utf8To16(logFilePath));
#else
            filelogger = spdlog::basic_logger_mt(programName, logFilePath);
#endif
            filelogger->set_level(spdlog::level::trace);
            filelogger->flush_on(spdlog::level::level_enum::warn);
            spdlog::flush_every(std::chrono::seconds(3)); // 每3秒自动刷新一次
        }

        isInit = true; // 标记已经初始化了
        isInitFail = false;
    }
    catch (const std::exception& e) {
        isInitFail = true; // 标记初始化失败过了
        clear();
    }

    mt.unlock();
    // 检查看看日志文件是否存在了,如果filelogger指针为null,或者连文件也不存在
    if (isFileEnable && (filelogger == nullptr || !File(logFilePath).exists())) {
        // string msg = "Debug.init():无法创建日志文件 -> " + logFilePath; //底下马上要clear(),所以这里先写了
        if (!isInitFail) {
            isInitFail = true; // 标记初始化失败过了
            clear();
        }
    }
    else {
        // 清空老文件
        removeOldFile();
    }

    // 成功初始化完毕之后清空一次内存日志(删除日志文件的多余日志)
    MemoryLog::GetInst()->clear();
}

void Debug::clear()
{
    mt.lock();
    try {
        if (!isInit) {
            mt.unlock();
            return;
        }
        isInit = false;

        isEnable = true;
        isMemLogEnable = false;
        isConsoleEnable = true;
        isFileEnable = true;
        logFileThr = spdlog::level::level_enum::debug;
        logMemoryThr = spdlog::level::level_enum::debug;
        logConsoleThr = spdlog::level::level_enum::debug;

        isEncryptConsole = false;
        isEncryptFile = false;

        MemoryLog::GetInst()->clear();

        logPattern = _logPattern;

        if (filelogger != nullptr) {
            filelogger->flush();
        }
        if (consolelogger != nullptr) {
            consolelogger->flush();
        }
        filelogger = nullptr;
        consolelogger = nullptr;
        spdlog::drop_all(); // 这里要注意删除,否则无法再创建同名的

        exLoggerCallback = nullptr;
        exLoggerEncryptCallback = nullptr;
        exLoggerEncrypDeletetCallback = nullptr;

        programName.clear();
        logDirPath.clear();
        logFilePath.clear();
    }
    catch (const std::exception&) {
        filelogger = nullptr;
        consolelogger = nullptr;
        spdlog::drop_all();
        exLoggerCallback = nullptr;
        exLoggerEncryptCallback = nullptr;
        exLoggerEncrypDeletetCallback = nullptr;
    }
    mt.unlock();
}

void Debug::removeOldFile(long long sec)
{
    using namespace std;
    try {
        // 这两个
        vector<File> vFilesProgramLogs;
        vector<Timestamp::TimeDiff> vFilesProgramLogDiffTime;
        File logDir(logDirPath);

        vector<File> vFiles;
        logDir.list(vFiles);
        for (size_t i = 0; i < vFiles.size(); i++) {
            File& file = vFiles[i];
            if (file.isFile() && file.exists()) {
                Path filePath(vFiles[i].path());
                string fileName = filePath.getFileName();
                // 只清理自己这个programName名字的日志
                if (std::regex_search(fileName, regex(format("%s[.]\\d+[-]\\d+.*[.]log", programName)))) {
                    Poco::Timestamp now;
                    Poco::Timestamp t = file.getLastModified();
                    Timestamp::TimeDiff diff = now - t; // microseconds
                    Timespan ts(diff);
                    vFilesProgramLogDiffTime.push_back(ts.totalSeconds()); // 记录所有文件距离现在的时间秒
                    vFilesProgramLogs.push_back(file);                     // 和那个index一致的保存那个文件.
                }
            }
        }

        vector<Timestamp::TimeDiff> vFileSubTimeSort = vFilesProgramLogDiffTime;
        std::sort(vFileSubTimeSort.begin(), vFileSubTimeSort.end());
        if (vFileSubTimeSort.size() < 50) {
            if (isPrintInternalMessage)
                LogMsg(spdlog::level::level_enum::info, format("日志启动,\"%s\" 当前有%z个日志文件", programName, vFileSubTimeSort.size()).c_str());
        }
        else {
            Timestamp::TimeDiff thr = vFileSubTimeSort[49]; // 第50个文件的时间
            // 删除的时间门限,如果超过50个文件那么保留50个文件,如果是3天内的文件那么时间设成3天
            if (thr < sec) {
                thr = sec;
            }
            for (size_t i = 0; i < vFilesProgramLogDiffTime.size(); i++) {
                if (vFilesProgramLogDiffTime[i] > thr) {
                    vFilesProgramLogs[i].remove();
                    if (isPrintInternalMessage)
                        LogMsg(spdlog::level::level_enum::info, (format("移除过早的日志文件%s", vFilesProgramLogs[i].path())).c_str());
                }
            }
        }
    }
    catch (const std::exception& e) {
        if (isPrintInternalMessage)
            LogMsg(spdlog::level::level_enum::err, format("移除过早的日志文件异常:%s", std::string(e.what())).c_str());
    }
}

void Debug::setIsConsoleEnable(bool enable)
{
    isConsoleEnable = enable;

    // 如果是设置为不使能那么就直接退出
    if (isConsoleEnable == false) {
        return;
    }
    try {
        // 如果是设置为使能,如果指针还没有初始化过
        if (consolelogger == nullptr) {
            // ischcp65001设置控制台为UTF8输出
#if defined(_WIN32) || defined(_WIN64)
            if (ischcp65001) {
                system("chcp 65001"); // 测试了在windows下有效
            }
#endif

            consolelogger = spdlog::stdout_color_mt("console");

            consolelogger->set_level(spdlog::level::trace);
            auto console_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(consolelogger->sinks().back().get());

#if defined(_WIN32) || defined(_WIN64)
            // win下好像可以选择的颜色不多
            // console_sink->set_color(spdlog::level::trace,  console_sink->BOLD);
            // console_sink->set_color(spdlog::level::debug, console_sink->CYAN);
            // 新版本的API貌似改了
            console_sink->set_color(spdlog::level::info, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
            // console_sink->set_color(spdlog::level::info, console_sink->white);
#endif
            // console_sink->set_color(spdlog::level::warn, console_sink->YELLOW);
            // console_sink->set_color(spdlog::level::err, console_sink->RED);
        }
    }
    catch (const std::exception&) {
    }
}

} // namespace dlog

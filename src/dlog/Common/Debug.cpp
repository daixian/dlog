#include "Debug.h"
#include "FileHelper.h"

#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>
#include <Poco/Format.h>
#include "Poco/Glob.h"
#include "Poco/Timespan.h"
#include "Poco/FileStream.h"

#include "Common.h"
#include <iostream>
#include <regex>

using namespace std;
using namespace Poco;

namespace dlog {

Debug* Debug::m_pInstance = new Debug();

void Debug::init(const char* logDir, const char* program, INIT_RELATIVE rel, bool utf8bom)
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
        //设置日志输出格式
        spdlog::set_pattern(logPattern);

        programName = program; //记录程序名
        isUTF8BOM = utf8bom;
        logDirPath.clear();
        logFilePath.clear();

        //初始化控制台
        setIsConsoleEnable(isConsoleEnable);

        Path inputDir = Path::forDirectory(logDir);

#if WIN32
        //在windows下如果是相对地址,或者是没有盘符.如果是相对对路径,就从根目录去拼接
        if (inputDir.isRelative() || inputDir.getDevice().empty()) {

#else
        if (inputDir.isRelative()) {
#endif
            if (rel == INIT_RELATIVE::MODULE) {
                FileHelper::makeAbsolute(Path(FileHelper::getModuleDir()), inputDir); //模块目录
                this->logDirPath = inputDir.toString();
            }
            else {
                FileHelper::makeAbsolute(Path(FileHelper::getAppDir()), inputDir); //app目录
                this->logDirPath = inputDir.toString();
            }
        }
        else {
            //如果是绝对路径,就直接使用
            this->logDirPath = inputDir.toString();
        }

        if (!FileHelper::dirExists(this->logDirPath)) //如果文件夹不存在
        {
            Path wantPath(this->logDirPath);
            if (wantPath.isFile()) {
                //如果它又已经被一个文件占用了文件名,那就在这个文件夹下使用log文件夹
                this->logDirPath = wantPath.parent().append("log").toString();
            }
            FileHelper::isExistsAndCreat(this->logDirPath); //如果文件夹不存在就创建
        }

        string logFileName = format("%s.%s.log", programName, Common::secTimeStr());
        logFilePath = Path(logDirPath).append(logFileName).toString();

        //如果使用写入UTF8BOM那么就写入3个字节的bom头
        if (isUTF8BOM) {
            Poco::FileOutputStream fs(logFilePath);
            const unsigned char bom[3] = {0xef, 0xbb, 0xbf};
            fs.write((char*)bom, 3);
            fs.close();
        }

        filelogger = spdlog::basic_logger_mt(programName, logFilePath);
        filelogger->set_level(spdlog::level::trace);
        filelogger->flush_on(spdlog::level::level_enum::warn);
        spdlog::flush_every(std::chrono::seconds(3)); //每秒自动刷新一次
        isInit = true;                                //标记已经初始化了
        isInitFail = false;
    }
    catch (const std::exception& e) {
        //std::cerr << "Debug.init():初始化失败!" << e.what() << '\n';
        isInitFail = true; //标记初始化失败过了
        mt.unlock();
        clear();
        throw e;
    }

    mt.unlock();
    //检查看看日志文件是否存在了,如果filelogger指针为null,或者连文件也不存在
    if (filelogger == nullptr || !File(logFilePath).exists()) {
        string msg = "Debug.init():无法创建日志文件 -> " + logFilePath; //底下马上要clear(),所以这里先写了
        clear();
        isInitFail = true; //标记初始化失败过了
        //std::cerr << msg << '\n';
        throw std::invalid_argument(msg);
    }
    else {
        //清空老文件
        removeOldFile();
    }

    //成功初始化完毕之后清空一次内存日志(删除日志文件的多余日志)
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
        logFileThr = spdlog::level::level_enum::debug;
        logMemoryThr = spdlog::level::level_enum::debug;
        logConsoleThr = spdlog::level::level_enum::debug;
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
        spdlog::drop_all(); //这里要注意删除,否则无法再创建同名的

        programName.clear();
        logDirPath.clear();
        logFilePath.clear();
    }
    catch (const std::exception&) {
        filelogger = nullptr;
        consolelogger = nullptr;
        spdlog::drop_all();
    }
    mt.unlock();
}

void Debug::removeOldFile(long long sec)
{
    using namespace std;
    try {
        //这两个
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
                if (std::regex_search(fileName, regex(format("%s[.]\\d+[-]\\d+[.]log", programName)))) {
                    Poco::Timestamp now;
                    Poco::Timestamp t = file.getLastModified();
                    Timestamp::TimeDiff diff = now - t; //microseconds
                    Timespan ts(diff);
                    vFilesProgramLogDiffTime.push_back(ts.totalSeconds()); //记录所有文件距离现在的时间秒
                    vFilesProgramLogs.push_back(file);                     //和那个index一致的保存那个文件.
                }
            }
        }

        vector<Timestamp::TimeDiff> vFileSubTimeSort = vFilesProgramLogDiffTime;
        std::sort(vFileSubTimeSort.begin(), vFileSubTimeSort.end());
        if (vFileSubTimeSort.size() < 50) {
            LogMsg(spdlog::level::level_enum::info, format("日志启动,\"%s\" 当前有%z个日志文件", programName, vFileSubTimeSort.size()).c_str());
        }
        else {
            Timestamp::TimeDiff thr = vFileSubTimeSort[49]; //第50个文件的时间
            //删除的时间门限,如果超过50个文件那么保留50个文件,如果是3天内的文件那么时间设成3天
            if (thr < sec) {
                thr = sec;
            }
            for (size_t i = 0; i < vFilesProgramLogDiffTime.size(); i++) {
                if (vFilesProgramLogDiffTime[i] > thr) {
                    vFilesProgramLogs[i].remove();
                    LogMsg(spdlog::level::level_enum::info, (format("移除过早的日志文件%s", vFilesProgramLogs[i].path())).c_str());
                }
            }
        }
    }
    catch (const std::exception& e) {
        LogMsg(spdlog::level::level_enum::err, format("移除过早的日志文件异常:%s", std::string(e.what())).c_str());
    }
}

void Debug::setIsConsoleEnable(bool enable)
{
    isConsoleEnable = enable;

    //如果是设置为不使能,那么就直接退出
    if (isConsoleEnable == false) {
        return;
    }

    //设置控制台为UTF8输出
#if defined(_WIN32) || defined(_WIN64)
    system("chcp 65001"); //测试了在windows下有效
#endif

    //如果是设置为使能,如果指针还没有初始化过
    if (consolelogger == nullptr) {
        consolelogger = spdlog::stdout_color_mt("console");

        consolelogger->set_level(spdlog::level::trace);
        auto console_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(consolelogger->sinks().back().get());

#if defined(_WIN32) || defined(_WIN64)
        //win下好像可以选择的颜色不多
        console_sink->set_color(spdlog::level::trace, console_sink->BOLD);
        console_sink->set_color(spdlog::level::debug, console_sink->CYAN);
        console_sink->set_color(spdlog::level::info, console_sink->WHITE);
#else
        console_sink->set_color(spdlog::level::info, console_sink->white);
#endif
        //console_sink->set_color(spdlog::level::warn, console_sink->YELLOW);
        //console_sink->set_color(spdlog::level::err, console_sink->RED);
    }
}

} // namespace dlog
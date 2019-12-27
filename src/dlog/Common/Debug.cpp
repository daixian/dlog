﻿#include "Debug.h"
#include "FileHelper.h"

#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>
#include <Poco/Format.h>
#include "Poco/Glob.h"

#include "Common.h"
#include <iostream>
#include <regex>

using namespace std;
using namespace Poco;

namespace dxlib {

Debug* Debug::m_pInstance = NULL;

void Debug::init(const char* logDir, const char* program, INIT_RELATIVE rel)
{
    mt.lock();
    if (isInit) {
        mt.unlock();
        return;
    }

    try {
        //设置日志输出格式
        spdlog::set_pattern(logPattern);

        programName = program; //记录程序名
        logDirPath.clear();
        logFilePath.clear();

        //初始化控制台
        setIsConsoleEnable(isConsoleEnable);

        Path inputDir(logDir);

        //如果是绝对路径,就直接使用
        if (inputDir.isAbsolute()) {
            this->logDirPath = logDir;
        }
        else { //如果是相对对路径,就从根目录去拼接
            if (rel == INIT_RELATIVE::MODULE) {
                Path mdDir = FileHelper::getModuleDir(); //模块目录
                this->logDirPath = mdDir.append(logDir).toString();
            }
            else {
                Path appDir = FileHelper::getAppDir(); //app目录
                this->logDirPath = appDir.append(inputDir).toString();
            }
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

        string logFileName = format("%s.%s.log", programName, secTimeStr());
        logFilePath = Path(logDirPath).append(logFileName).toString();

        filelogger = spdlog::basic_logger_mt(programName, logFilePath);
        filelogger->set_level(spdlog::level::trace);
        filelogger->flush_on(spdlog::level::level_enum::warn);
        spdlog::flush_every(std::chrono::seconds(3)); //每秒自动刷新一次
        isInit = true;                                //标记已经初始化了
        isInitFail = false;
        mt.unlock();
    }
    catch (const std::exception& e) {
        std::cerr << "Debug.init():初始化失败!" << e.what() << '\n';
        isInitFail = true; //标记初始化失败过了
        mt.unlock();
        throw e;
    }

    //检查看看日志文件是否存在了
    if (filelogger == nullptr || !File(logFilePath).exists()) {
        string msg = "Debug.init():无法创建日志文件! -> " + logFilePath; //底下马上要clear(),所以这里先写了
        clear();
        isInitFail = true; //标记初始化失败过了
        std::cerr << msg << '\n';
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

void Debug::removeOldFile(long sec)
{
    using namespace std;
    try {
        vector<long> vFileSubTime;
        vector<string> vFiles;
        File logDir(logDirPath);

        logDir.list(vFiles);
        for (size_t i = 0; i < vFiles.size(); i++) {
            Path filePath = vFiles[i];
            if (filePath.isFile()) {
                if (std::regex_search(filePath.getFileName(), regex(format("%s[0-9]+[-]{1}[0-9]+[.]{1}log", programName)))) {
                    File file(filePath);
                    Poco::Timestamp now;
                    Poco::Timestamp t = file.getLastModified();
                    vFileSubTime.push_back(now - t); //记录所有文件的时间
                }
            }
        }

        vector<long> vFileSubTimeSort = vFileSubTime;
        std::sort(vFileSubTimeSort.begin(), vFileSubTimeSort.end());
        if (vFileSubTimeSort.size() < 50) {
            LogMsg(spdlog::level::level_enum::info, format("dlog启动,\"%s\" 当前有%z个日志文件", programName, vFileSubTimeSort.size()).c_str());
        }
        else {
            int thr = vFileSubTimeSort[49]; //删除的时间门限
            if (thr < sec) {
                thr = sec;
            }
            //for (size_t i = 0; i < vFileSubTime.size(); i++) {
            //    if (vFileSubTime[i] > thr) {
            //        fs::remove(vFilePath[i]);
            //        LogMsg(spdlog::level::level_enum::info, (boost::format("dlog移除过早的日志文件%s") % vFilePath[i].string()).str().c_str());
            //    }
            //}
        }
    }
    catch (const std::exception& e) {
        LogMsg(spdlog::level::level_enum::err, format("dlog移除过早的日志文件异常:%s", std::string(e.what())).c_str());
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

} // namespace dxlib
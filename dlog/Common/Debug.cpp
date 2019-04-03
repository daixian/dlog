#pragma once
#include "Debug.h"
#include "FileHelper.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "Common.h"

namespace fs = boost::filesystem;
using namespace std;

namespace dxlib {

Debug* Debug::m_pInstance = NULL;

void Debug::init(const char* program, const char* logDir, INIT_RELATIVE rel)
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

        fs::path inputDir = fs::path(logDir);

        //如果是绝对路径,就直接使用
        if (inputDir.is_absolute()) {
            this->logDirPath = logDir;
        }
        else { //如果是相对对路径,就从根目录去拼接
            if (rel == INIT_RELATIVE::MODULE) {
                fs::path mdDir = FileHelper::getModuleDir(); //模块目录
                this->logDirPath = (mdDir / inputDir).string();
            }
            else {
                fs::path appDir = FileHelper::getAppDir(); //模块目录
                this->logDirPath = (appDir / inputDir).string();
            }
        }
        FileHelper::isExistsAndCreat(logDirPath); //如果文件夹不存在就创建

        string logFileName = (boost::format("%s.%s.log") % program % secTimeStr()).str();
        logFilePath = (fs::path(logDirPath) / logFileName).string();

        filelogger = spdlog::basic_logger_mt(programName, logFilePath);
        filelogger->flush_on(spdlog::level::level_enum::warn);
        isInit = true; //标记已经初始化了
        isInitFail = false;
        mt.unlock();
    }
    catch (const std::exception& e) {

        isInitFail = true; //标记初始化失败过了
        mt.unlock();
        throw e;
    }

    //检查看看日志文件是否存在了
    if (filelogger == nullptr || !fs::is_regular_file(logFilePath)) {
        string msg = "dlog creat log file fail! :" + logFilePath; //底下马上要clear(),所以这里先写了
        clear();
        isInitFail = true; //标记初始化失败过了
        throw std::invalid_argument(msg);
    }
    else {
        //清空老文件
        removeOldFile();
    }
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
        logFileThr = spdlog::level::level_enum::info;
        logMemoryThr = spdlog::level::level_enum::info;
        logConsoleThr = spdlog::level::level_enum::info;
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
        vector<fs::path> vFilePath;

        fs::directory_iterator diR(logDirPath);
        for (auto& de : diR) {
            if (fs::is_regular_file(de.status())) { //如果这个是文件
                string fname = de.path().filename().string();
                string ext = de.path().extension().string();
                std::vector<std::string> strs;
                boost::split(strs, fname, boost::is_any_of("."));
                if (strs[0] == programName && //起头是自己的名字
                    ext == ".log") {
                    auto t = fs::last_write_time(de.path());
                    std ::time_t now = std::time(nullptr);
                    vFileSubTime.push_back(now - t); //记录所有文件的时间
                    vFilePath.push_back(de.path());
                }
            }
        }
        vector<long> vFileSubTimeSort = vFileSubTime;
        std::sort(vFileSubTimeSort.begin(), vFileSubTimeSort.end());
        if (vFileSubTimeSort.size() < 50) {
            LogI("dlog启动,\"%s\" 当前有%d个日志文件", programName.c_str(), vFileSubTimeSort.size());
        }
        else {
            int thr = vFileSubTimeSort[49]; //删除的时间门限
            if (thr < sec) {
                thr = sec;
            }
            for (size_t i = 0; i < vFileSubTime.size(); i++) {
                if (vFileSubTime[i] > thr) {
                    fs::remove(vFilePath[i]);
                    LogI("dlog移除过早的日志文件%s", vFilePath[i].string().c_str());
                }
            }
        }
    }
    catch (const std::exception& e) {
        LogE("dlog移除过早的日志文件异常:%s", e.what());
    }
}

void Debug::setIsConsoleEnable(bool enable)
{
    isConsoleEnable = enable;

    if (isConsoleEnable && consolelogger == nullptr) {
        consolelogger = spdlog::stdout_color_mt("console");

        auto console_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(consolelogger->sinks().back().get());
        //console_sink->set_color(spdlog::level::debug, console_sink->CYAN);
#if defined(_WIN32) || defined(_WIN64)
        console_sink->set_color(spdlog::level::info, console_sink->WHITE);
#else
        console_sink->set_color(spdlog::level::info, console_sink->white);
#endif
        //console_sink->set_color(spdlog::level::warn, console_sink->YELLOW);
        //console_sink->set_color(spdlog::level::err, console_sink->RED);
    }
}

} // namespace dxlib
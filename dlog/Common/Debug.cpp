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

void Debug::init(const char* program, const char* logDir)
{
    mt.lock();
    try {
        //设置日志输出格式
        spdlog::set_pattern(logPattern);

        programName = program; //记录程序名
        logDirPath.clear();
        logFilePath.clear();

        if (isConsoleEnable && consolelogger == nullptr) {
            consolelogger = spdlog::stdout_color_mt("console");

            auto console_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(consolelogger->sinks().back().get());
            //console_sink->set_color(spdlog::level::debug, console_sink->CYAN);
            console_sink->set_color(spdlog::level::info, console_sink->WHITE);
            //console_sink->set_color(spdlog::level::warn, console_sink->YELLOW);
            //console_sink->set_color(spdlog::level::err, console_sink->RED);
        }

        string md = FileHelper::getModuleDir();
        fs::path mdDir = md; //模块目录
        fs::path inputDir = fs::path(logDir);

        //如果是绝对路径
        if (inputDir.is_absolute()) {
            this->logDirPath = logDir;
        }
        else { //如果是相对对路径,就从根目录去拼接
            this->logDirPath = (mdDir / inputDir).string();
        }
        if (!FileHelper::dirExists(logDirPath)) { //如果文件夹路径不存在
            string cmd = std::string("mkdir \"") + logDirPath + std::string("\"");
            system(cmd.c_str()); //创建文件夹
        }

        string logFileName = (boost::format("%s.%s.log") % program % secTimeStr()).str();
        logFilePath = (fs::path(logDirPath) / logFileName).string();

        filelogger = spdlog::basic_logger_mt(programName, logFilePath);

        isInit = true; //标记已经初始化了
    }
    catch (const std::exception&) {
    }
    mt.unlock();

    //如果文件不存在
    if (!fs::is_regular_file(logFilePath)) {
        string tp = logFilePath;
        clear();
        throw "dlog未能创建日志文件" + tp;
    }

    //清空老文件
    removeOldFile();
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
            LogI("\"%s\" 当前有%d个日志文件", programName.c_str(), vFileSubTimeSort.size());
        }
        else {
            int thr = vFileSubTimeSort[49]; //删除的时间门限
            if (thr < sec) {
                thr = sec;
            }
            for (size_t i = 0; i < vFileSubTime.size(); i++) {
                if (vFileSubTime[i] > thr) {
                    fs::remove(vFilePath[i]);
                    LogI("dlog移除过早的日志文件%s", vFilePath[i].c_str());
                }
            }
        }
    } // namespace dxlib
    catch (const std::exception&) {
    }
}

} // namespace dxlib
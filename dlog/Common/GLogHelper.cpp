#include <stdlib.h>
#include "GLogHelper.h"
#include <io.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <regex>

#ifndef GOOGLE_GLOG_DLL_DECL
#define GOOGLE_GLOG_DLL_DECL //如果是静态库就不需要内容,否则需要定义成__declspec(dllimport)
#endif                       // !GOOGLE_GLOG_DLL_DECL
//另外如果和windows.h冲突,那么还需要定义GLOG_NO_ABBREVIATED_SEVERITIES

#include "glog/logging.h"
#include "glog/raw_logging.h"

#include "FileHelper.h"

#pragma comment(lib, "glog.lib")
#pragma comment(lib, "shlwapi.lib")

//配置输出日志的目录：
//#define LOGDIR "log"
//#define MKDIR "mkdir -p " LOGDIR

#define CSIDL_APPDATA 0x001a // <user name>\Application Data

using boost::filesystem::path;
namespace fs = boost::filesystem;

namespace dxlib {

//将信息输出到单独的文件和 LOG(ERROR)  windowd平台好像不行
void GLogHelper::SignalHandle(const char* data, int size)
{
    std::string str = std::string(data, size);
    /*
    std::ofstream fs("glog_dump.log",std::ios::app);
    fs<<str;
    fs.close();
    */
    LOG(ERROR) << str;
    //也可以直接在这里发送邮件或短信通知，不过这个方法是被回调多次的（每次回调只输出一行错误信息，所以如上面的记录到文件，也需要>以追加模式方可），所以这里发邮件或短信不是很适合，不过倒是可以调用一个 SHELL 或 PYTHON 脚本，而此脚本会先 sleep 3秒左右，然后将错
    //误信息通过邮件或短信发送出去，这样就不需要监控脚本定时高频率执行，浪费效率了。
}

//GLOG配置：
GLogHelper::GLogHelper(const char* program, const char* logDir)
{
    programName = program; //记录程序名

    google::InitGoogleLogging(program);

    std::string md = FileHelper::getModuleDir();

    logDirPath.clear();
    path dir = path(logDir);

    //如果是绝对路径
    if (dir.is_absolute()) {
        logDirPath = logDir;
    }
    else { //如果是相对对路径,就从根目录去拼接
        logDirPath = md;
        if (logDir[0] != '\\') {
            logDirPath.append("\\");
        }
        logDirPath.append(logDir);
    }
    if (!FileHelper::dirExists(logDirPath)) { //如果文件夹路径不存在
        std::string cmd = std::string("mkdir \"") + logDirPath + std::string("\"");
        system(cmd.c_str()); //创建文件夹
    }

    FLAGS_log_dir = logDirPath; // 设置日志文件路径

    //google::SetLogFilenameExtension("91_");     //设置文件名扩展，如平台？或其它需要区分的信息(这一句实际并不是文件的扩展名)

    //FLAGS_log_dir = "log";// 设置日志文件路径 "./log"
    //google::SetLogDestination(google::GLOG_INFO, "./log/info");// 设置某个级别的日志的缓存文件路径

    //FLAGS_stderrthreshold = google::GLOG_WARNING; // 当日志级别大于等于此级别时，自动将此日志输出到标准错误中
    FLAGS_stderrthreshold = google::GLOG_INFO; // 当日志级别大于等于此级别时，自动将此日志输出到标准错误(终端窗口)中

    FLAGS_logbuflevel = google::GLOG_INFO;   //当小于等于会缓存,当日志级别大于此级别时会马上输出，而不缓存
    FLAGS_colorlogtostderr = true;           //设置彩色控制台输出
    FLAGS_logbufsecs = 1;                    // 缓存最久长时间为多久
    FLAGS_max_log_size = 50;                 // 当日志文件达到多少时，进行文件分割，以M为单位
    FLAGS_stop_logging_if_full_disk = false; // 当磁盘已满时,停止输出日志文件

    //windows下可能不支持这个
    //google::InstallFailureSignalHandler();      //捕捉 core dumped
    //google::InstallFailureWriter(SignalHandle); //默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出>方式：

    LOG(INFO) << "APPDATA_DiR=" << md.c_str();
    LOG(INFO) << "log_dir=" << FLAGS_log_dir;

    removeOldFile();
}

//GLOG内存清理：
GLogHelper::~GLogHelper()
{
    try {
        google::FlushLogFiles(google::GLOG_INFO);
        google::ShutdownGoogleLogging();
    }
    catch (const std::exception&) {
    }
}

bool isNumber(std::string token)
{
    using namespace std;
    return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

void GLogHelper::removeOldFile(long sec)
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
                    isNumber(strs.back())) {  //末尾是一串数字
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
            LOG(INFO) << "\"" << programName << "\" 当前有" << vFileSubTimeSort.size() << "个日志文件";
        }
        else {
            int thr = vFileSubTimeSort[49]; //删除的时间门限
            if (thr < sec) {
                thr = sec;
            }
            for (size_t i = 0; i < vFileSubTime.size(); i++) {
                if (vFileSubTime[i] > thr) {
                    fs::remove(vFilePath[i]);
                    LOG(INFO) << "dlog移除过早的日志文件" << vFilePath[i];
                }
            }
        }
    }
    catch (const std::exception&) {
    }
}

} // namespace dxlib
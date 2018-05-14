#include <stdlib.h>
#include "GLogHelper.h"
#include <io.h>
#include <windows.h>
#include <ShlObj.h> //SHGetSpecialFolderPath
#include <experimental\filesystem>

#ifdef _DEBUG
#pragma comment(lib, "glogd.lib")
#elif NDEBUG
#pragma comment(lib, "glog.lib")
#endif // _DEBUG

//配置输出日志的目录：
//#define LOGDIR "log"
//#define MKDIR "mkdir -p " LOGDIR
//#define MKDIR_WIN "mkdir " LOGDIR //windows的不需要-p命令

#define CSIDL_APPDATA 0x001a        // <user name>\Application Data

using std::experimental::filesystem::path;

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

bool GLogHelper::dirExists(const std::string& dirName_in)
{
    int ftyp = _access(dirName_in.c_str(), 0);

    if (0 == ftyp)
        return true;   // this is a directory!
    else
        return false;    // this is not a directory!
}

bool GLogHelper::dirExists(const std::wstring& dirName_in)
{
    int ftyp = _access(ws2s(dirName_in).c_str(), 0);

    if (0 == ftyp)
        return true;   // this is a directory!
    else
        return false;    // this is not a directory!
}

std::string GLogHelper::getAppDir()
{
    return getModuleDir();
}

std::string GLogHelper::isExistsAndCreat(std::wstring dirPath)
{
    std::string sDir = ws2s(dirPath);
    if (!dirExists(sDir))//如果文件夹路径不存在
    {
        std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");//这个命令可以创建一串文件夹
        system(cmd.c_str()); //创建文件夹
    }
    return sDir;
}

std::string GLogHelper::isExistsAndCreat(std::string sDir)
{
    if (!dirExists(sDir))//如果文件夹路径不存在
    {
        std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");//这个命令可以创建一串文件夹
        system(cmd.c_str()); //创建文件夹
    }
    return sDir;
}

//GLOG配置：
GLogHelper::GLogHelper(char* program, char* logDir)
{
    programName = program;//记录程序名

    google::InitGoogleLogging(program);

    //std::wstring path = getMyDirectory();//path = L"D:\\Work\\F3DSys\\F3DSystem"
    wchar_t *szPath = new wchar_t[512];

    //这个函数不能用，由于管理员模式，路径不是当前文件夹
    BOOL bRet = SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);//L"C:\\Users\\f3d\\AppData\\Roaming"

    logDirPath.clear();
    path dir = path(logDir);

    if (dir.is_absolute())
    {
        logDirPath = logDir;
    }
    else
    {
        logDirPath = getModuleDir();
        if (logDir[0] != '\\')
        {
            logDirPath.append("\\");
        }
        logDirPath.append(logDir);
    }
    if (!dirExists(logDirPath))//如果文件夹路径不存在
    {
        std::string cmd = std::string("mkdir \"") + logDirPath + std::string("\"");
        system(cmd.c_str()); //创建文件夹
    }

    FLAGS_log_dir = logDirPath;// 设置日志文件路径

    //google::SetLogFilenameExtension("91_");     //设置文件名扩展，如平台？或其它需要区分的信息(这一句实际并不是文件的扩展名)

    //FLAGS_log_dir = "log";// 设置日志文件路径 "./log"
    //google::SetLogDestination(google::GLOG_INFO, "./log/info");// 设置某个级别的日志的缓存文件路径

    //FLAGS_stderrthreshold = google::GLOG_WARNING; // 当日志级别大于等于此级别时，自动将此日志输出到标准错误中
    FLAGS_stderrthreshold = google::GLOG_INFO; // 当日志级别大于等于此级别时，自动将此日志输出到标准错误(终端窗口)中

    FLAGS_logbuflevel = google::GLOG_INFO;//当小于等于会缓存,当日志级别大于此级别时会马上输出，而不缓存
    FLAGS_colorlogtostderr = true;//设置彩色控制台输出
    FLAGS_logbufsecs = 1;  // 缓存最久长时间为多久
    FLAGS_max_log_size = 50;// 当日志文件达到多少时，进行文件分割，以M为单位
    FLAGS_stop_logging_if_full_disk = false; // 当磁盘已满时,停止输出日志文件

    google::InstallFailureSignalHandler();      //捕捉 core dumped
    google::InstallFailureWriter(SignalHandle);    //默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出>方式：

    std::wstring wsPath(szPath);

    LOG(INFO) << "APPDATA_DiR=" << ws2s(wsPath);
    LOG(INFO) << "log_dir=" << FLAGS_log_dir;
    delete[]szPath;
}

//GLOG内存清理：
GLogHelper::~GLogHelper()
{
    try {
        google::FlushLogFiles(google::GLOG_INFO);
        google::ShutdownGoogleLogging();
    }
    catch (std::string *caught)
    {
    }
}
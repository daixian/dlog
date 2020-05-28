#include "FileHelper.h"

#include <stdlib.h>
#include "iostream"
#if defined(_WIN32) || defined(_WIN64)
#    include <windows.h>
#    include <ShlObj.h> //SHGetSpecialFolderPath
#elif defined(__linux__)
#    include <unistd.h>
#    include <sys/types.h>
#    include <pwd.h>
#endif

#include <Poco/Path.h>
#include <Poco/File.h>

#include "Common.h"
#include <ctime>

////这个脚本可能会报错 语言->符合模式 "combaseapi.h(229): error C2187: syntax error: 'identifier' was unexpected here" when using /permissive-
namespace dlog {

//只需要使用api获得一次就行了
std::string moduleDir;

#if defined(_WIN32) || defined(_WIN64)

//得到模块目录末尾不带斜杠"D:\\Work\\F3DSys\\F3DSystem"
std::string FileHelper::getModuleDir()
{
    if (!moduleDir.empty()) {
        return moduleDir; //只要有记录了就直接使用
    }
    else {
        char exeFullPath[MAX_PATH]; // Full path
        std::string strPath = "";

        GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);

        strPath = std::string(exeFullPath); // Get full path of the file
        size_t pos = strPath.find_last_of('\\', strPath.length());
        moduleDir = strPath.substr(0, pos); // Return the directory without the file name
        return moduleDir;
    }
}

std::string FileHelper::getAppDir()
{
    char szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
        return std::string(szPath);
    }
    return std::string();
}
#elif defined(__linux__)

std::string FileHelper::getModuleDir()
{
    if (!moduleDir.empty()) {
        return moduleDir; //只要有记录了就直接使用
    }
    char arg1[32];
    char exepath[512 + 1] = {0};
    snprintf(arg1, sizeof(arg1), "/proc/%d/exe", getpid());
    ssize_t len = readlink(arg1, exepath, sizeof(exepath));
    if (len > 0) {
        std::string exeStr = std::string(exepath);
        size_t pos = exeStr.find_last_of('/', exeStr.length());
        moduleDir = exeStr.substr(0, pos); // Return the directory without the file name
        return moduleDir;
    }
    //如果len=-1那么就是出错了
    return "";
}

std::string FileHelper::getAppDir()
{
    struct passwd* pw = getpwuid(getuid());
    const char* homedir = pw->pw_dir;
    return std::string(homedir);
}
#else
std::string FileHelper::getModuleDir()
{
    if (!moduleDir.empty()) {
        return moduleDir; //只要有记录了就直接使用
    }
    moduleDir = Poco::Path::current();
    return moduleDir;
}
std::string FileHelper::getAppDir()
{
    return Poco::Path::dataHome();
}
#endif

void FileHelper::isExistsAndCreat(const std::wstring& dirPath)
{
    std::string sDir = ws2s(dirPath);
    isExistsAndCreat(sDir);
}

void FileHelper::isExistsAndCreat(const std::string& sDir)
{
    if (!dirExists(sDir)) { //如果文件夹路径不存在
        try {
            Poco::File dir(sDir);
            dir.createDirectories();
        }
        catch (const std::exception& ex) {
            std::cerr << "FileHelper.isExistsAndCreat():" << ex.what() << '\n';
            throw ex;
        }
    }
}

bool FileHelper::dirExists(const std::string& dirName_in)
{
    Poco::File dir(dirName_in);
    //如果存在
    if (dir.exists()) {
        if (dir.isDirectory()) {
            return true;
        }
        //它还是存在一种是一个文件的可能
    }
    return false;
}

bool FileHelper::dirExists(const std::wstring& dirName_in)
{
    //如果存在
    Poco::File dir(ws2s(dirName_in));
    if (dir.exists()) {
        if (dir.isDirectory()) {
            return true;
        }
        //它还是存在一种是一个文件的可能
    }
    return false;
}

void FileHelper::makeAbsolute(const Poco::Path& base, Poco::Path& path)
{
#if WIN32
    //在windows下如果是相对地址,或者是没有盘符
    if (path.isRelative() || path.getDevice().empty()) {
        std::string strPath = path.toString();
        //如果是windows下,没有盘符,那么这个路径应该就是相对地址.
        //如果开头存在/,\\,那么移除掉这开头的
        if (strPath.front() == '/' ||
            strPath.front() == '\\') {
            strPath.erase(0, 1);
            path = Poco::Path(strPath);
        }
#else
    if (root.isRelative()) {
#endif
        //这个函数会忽略相对路径的
        path.makeAbsolute(base);
    }
}

} // namespace dlog

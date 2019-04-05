#include "FileHelper.h"

#include <stdlib.h>
#include "iostream"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <ShlObj.h> //SHGetSpecialFolderPath
#elif defined(__linux__)
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

#include <boost/filesystem.hpp>

#include "Common.h"
#include <ctime>

namespace fs = boost::filesystem;

////这个脚本可能会报错 语言->符合模式 "combaseapi.h(229): error C2187: syntax error: 'identifier' was unexpected here" when using /permissive-
namespace dxlib {

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
    char arg1[32];
    char exepath[512 + 1] = {0};
    snprintf(arg1, sizeof(arg1), "/proc/%d/exe", getpid());
    readlink(arg1, exepath, sizeof(exepath));
    std::string exeStr = std::string(exepath);
    size_t pos = exeStr.find_last_of('/', exeStr.length());
    moduleDir = exeStr.substr(0, pos); // Return the directory without the file name
    return moduleDir;
}

std::string FileHelper::getAppDir()
{
    struct passwd* pw = getpwuid(getuid());
    const char* homedir = pw->pw_dir;
    return std::string(homedir);
}
#endif

void FileHelper::isExistsAndCreat(const std::wstring& dirPath)
{
    std::string sDir = ws2s(dirPath);
    isExistsAndCreat(sDir);
}

void FileHelper::isExistsAndCreat(const std::string& sDir)
{
    // #if defined(_WIN32) || defined(_WIN64)

    // #elif defined(__linux__)
    //     if (sDir.back() != '/') {
    //         sDir.append('/');
    //     }
    // #endif
    if (!dirExists(sDir)) { //如果文件夹路径不存在
        try {
            fs::create_directories(sDir);
        }
        catch (const std::exception& ex) {
            std::cerr << "FileHelper.isExistsAndCreat():" << ex.what() << '\n';
            throw ex;
        }

        //#if defined(_WIN32) || defined(_WIN64)
        //        //有查资料windows的cmd不支持-p命令
        //        std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");
        //#elif defined(__linux__)
        //        std::string cmd = std::string("mkdir -p \"") + sDir + std::string("\"");
        //#endif
        //        system(cmd.c_str()); //创建文件夹
    }
}

bool FileHelper::dirExists(const std::string& dirName_in)
{
    //如果存在
    if (fs::exists(dirName_in)) {
        if (fs::is_directory(dirName_in)) {
            return true;
        }
        //它还是存在一种是一个文件的可能
    }
    return false;
}

bool FileHelper::dirExists(const std::wstring& dirName_in)
{
    //如果存在
    if (fs::exists(dirName_in)) {
        if (fs::is_directory(dirName_in)) {
            return true;
        }
        //它还是存在一种是一个文件的可能
    }
    return false;
}

} // namespace dxlib

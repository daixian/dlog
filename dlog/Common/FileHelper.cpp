#include "FileHelper.h"

#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <ShlObj.h> //SHGetSpecialFolderPath
#include "Common.h"
#include <ctime>

////这个脚本可能会报错 语言->符合模式 "combaseapi.h(229): error C2187: syntax error: 'identifier' was unexpected here" when using /permissive-

using namespace std;

namespace dxlib {

//配置输出日志的目录：
#define LOGDIR "log"
#define MKDIR "mkdir -p " LOGDIR
#define MKDIR_WIN "mkdir " LOGDIR //有查资料windows的cmd不支持-p命令

#define CSIDL_APPDATA 0x001a // <user name>\Application Data

bool FileHelper::dirExists(const std::string& dirName_in)
{
    int ftyp = _access(dirName_in.c_str(), 0);

    if (0 == ftyp)
        return true; // this is a directory!
    else
        return false; // this is not a directory!
}

bool FileHelper::dirExists(const std::wstring& dirName_in)
{
    int ftyp = _access(ws2s(dirName_in).c_str(), 0);

    if (0 == ftyp)
        return true; // this is a directory!
    else
        return false; // this is not a directory!
}

//只需要使用api获得一次就行了
std::string moduleDir;

//得到模块目录末尾不带斜杠"D:\\Work\\F3DSys\\F3DSystem"
std::string FileHelper::getModuleDir()
{
    if (!moduleDir.empty()) {
        return moduleDir; //只要有记录了就直接使用
    }
    else {
        wchar_t exeFullPath[MAX_PATH]; // Full path
        std::string strPath = "";

        GetModuleFileName(NULL, exeFullPath, MAX_PATH);

        char CharString[MAX_PATH];
        size_t convertedChars = 0;
        wcstombs_s(&convertedChars, CharString, MAX_PATH, exeFullPath, _TRUNCATE);
        strPath = (std::string)CharString; // Get full path of the file
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

void FileHelper::isExistsAndCreat(std::wstring dirPath)
{
    std::string sDir = ws2s(dirPath);
    if (!dirExists(sDir)) { //如果文件夹路径不存在
        std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");
        system(cmd.c_str()); //创建文件夹
    }
}

void FileHelper::isExistsAndCreat(std::string sDir)
{
    if (!dirExists(sDir)) { //如果文件夹路径不存在
        std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");
        system(cmd.c_str()); //创建文件夹
    }
}
} // namespace dxlib
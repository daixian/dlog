#pragma once
#include <string>
#include <locale.h>
#include "Common.h"
#include <windows.h>
#include <ShlObj.h> //SHGetSpecialFolderPath

using namespace std;

string ws2s(const wstring& ws)
{
    string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

    setlocale(LC_ALL, "chs");

    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char* _Dest = new char[_Dsize];
    memset(_Dest, 0, _Dsize);
    //wcstombs(_Dest, _Source, _Dsize);
    size_t res = 0;
    wcstombs_s(&res, _Dest, _Dsize, _Source, _Dsize); //换了个安全函数，未测
    string result = _Dest;
    delete[] _Dest;

    setlocale(LC_ALL, curLocale.c_str());

    return result;
}

///-------------------------------------------------------------------------------------------------
/// <summary> string2wstring. </summary>
///
/// <remarks> Dx, 2017/7/18. </remarks>
///
/// <param name="s"> The string to process. </param>
///
/// <returns> A wstring. </returns>
///-------------------------------------------------------------------------------------------------
wstring s2ws(const string& s)
{
    setlocale(LC_ALL, "chs");

    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t* _Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    //mbstowcs(_Dest, _Source, _Dsize);
    size_t res = 0;
    mbstowcs_s(&res, _Dest, _Dsize, _Source, _Dsize); //换了个安全函数，未测
    wstring result = _Dest;
    delete[] _Dest;

    setlocale(LC_ALL, "C");
    return result;
}

///-------------------------------------------------------------------------------------------------
/// <summary> Byte数据转成十六进制文本. </summary>
///
/// <remarks> Dx, 2018/1/8. </remarks>
///
/// <param name="data">   字节数组起始位置. </param>
/// <param name="length"> 长度. </param>
///
/// <returns> 十六进制字符串. </returns>
///-------------------------------------------------------------------------------------------------
std::string byte2str(const void* data, int length)
{
    unsigned char* pChar = (unsigned char*)data;
    std::string msg;
    for (int i = 0; i < length; i++) {
        char b[8];
        sprintf_s(b, 8, "%02X ", pChar[i]);
        msg.append(b);
    }
    return msg;
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// path = L"D:\\Work\\F3DSys\\F3DSystem",
/// 它可以变成system32,所以一般应该使用下面的getModuleDir().
/// </summary>
///
/// <remarks> Dx, 2018/1/11. </remarks>
///
/// <returns> The application directory. </returns>
///-------------------------------------------------------------------------------------------------
std::wstring getAppDirectory()
{
    std::wstring wstr;
    unsigned long size = GetCurrentDirectory(0, NULL);
    wchar_t* path = new wchar_t[size];
    if (GetCurrentDirectory(size, path) != 0) {
        wstr = path;
    }
    delete[] path;
    return wstr;
}

///-------------------------------------------------------------------------------------------------
/// <summary> Gets module dir. </summary>
///
/// <remarks> Dx, 2018/1/11. </remarks>
///
/// <returns> The module dir. </returns>
///-------------------------------------------------------------------------------------------------
std::string getModuleDir()
{
    wchar_t exeFullPath[MAX_PATH]; // Full path
    std::string strPath = "";

    GetModuleFileName(NULL, exeFullPath, MAX_PATH);

    char CharString[MAX_PATH];
    size_t convertedChars = 0;
    wcstombs_s(&convertedChars, CharString, MAX_PATH, exeFullPath, _TRUNCATE);
    strPath = (std::string)CharString; // Get full path of the file
    int pos = strPath.find_last_of('\\', strPath.length());
    return strPath.substr(0, pos); // Return the directory without the file name
}
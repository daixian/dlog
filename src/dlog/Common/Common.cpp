﻿#include <string>
#include <locale.h>
#include "Common.h"
#include <stdlib.h>

#include "Poco/LocalDateTime.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"
#include "Poco/Format.h"

#if defined(_WIN32) || defined(_WIN64)
#    include <windows.h>

//一个windows下的实验的函数,设置控制台彩色的
//void setConsoleMode()
//{
//    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//    if (hOut != INVALID_HANDLE_VALUE) {
//        DWORD dwMode = 0;
//        GetConsoleMode(hOut, &dwMode);
//
//        dwMode |= 0x0004;
//        SetConsoleMode(hOut, dwMode);
//    }
//}

#elif defined(__linux__)
/* Linux. --------------------------------------------------- */

#endif

using namespace std;

namespace dlog {

//string Common::ws2s(const wstring& ws)
//{
//    string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
//
//    setlocale(LC_ALL, "chs");
//
//    const wchar_t* _Source = ws.c_str();
//    size_t _Dsize = 2 * ws.size() + 1;
//    char* _Dest = new char[_Dsize];
//    memset(_Dest, 0, _Dsize);
//
//#if defined(_WIN32) || defined(_WIN64)
//    size_t res = 0;
//    wcstombs_s(&res, _Dest, _Dsize, _Source, _Dsize); //换了个安全函数，未测
//#else
//    wcstombs(_Dest, _Source, _Dsize);
//#endif
//
//    string result = _Dest;
//    delete[] _Dest;
//
//    setlocale(LC_ALL, curLocale.c_str());
//
//    return result;
//}

///-------------------------------------------------------------------------------------------------
/// <summary> string2wstring. </summary>
///
/// <remarks> Dx, 2017/7/18. </remarks>
///
/// <param name="s"> The string to process. </param>
///
/// <returns> A wstring. </returns>
///-------------------------------------------------------------------------------------------------
//wstring Common::s2ws(const string& s)
//{
//    setlocale(LC_ALL, "chs");
//
//    const char* _Source = s.c_str();
//    size_t _Dsize = s.size() + 1;
//    wchar_t* _Dest = new wchar_t[_Dsize];
//    wmemset(_Dest, 0, _Dsize);
//#if defined(_WIN32) || defined(_WIN64)
//    size_t res = 0;
//    mbstowcs_s(&res, _Dest, _Dsize, _Source, _Dsize); //换了个安全函数，未测
//#else
//    mbstowcs(_Dest, _Source, _Dsize);
//#endif
//    wstring result = _Dest;
//    delete[] _Dest;
//
//    setlocale(LC_ALL, "C");
//    return result;
//}

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
//std::string Common::byte2str(const void* data, int length)
//{
//    unsigned char* pChar = (unsigned char*)data;
//    std::string msg;
//    for (int i = 0; i < length; i++) {
//        char b[8];
//
//#if defined(_WIN32) || defined(_WIN64)
//        sprintf_s(b, 8, "%02X ", pChar[i]);
//#else
//        snprintf(b, 8, "%02X ", pChar[i]);
//#endif
//        msg.append(b);
//    }
//    return msg;
//}

///-------------------------------------------------------------------------------------------------
/// <summary> 得到当前时间戳. </summary>
///
/// <remarks> Dx, 2018/12/14. </remarks>
///
/// <returns> The time. </returns>
///-------------------------------------------------------------------------------------------------
std::string Common::secTimeStr()
{
    //最后文件的形式如init_close.20191228-023549.log
    using namespace Poco;
    LocalDateTime now;
    std::string str = DateTimeFormatter::format(now, "%Y%m%d-%H%M%S");
    return str;
}

} // namespace dlog

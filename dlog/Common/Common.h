#pragma once
#include <string>
#include <locale.h>

///-------------------------------------------------------------------------------------------------
/// <summary> wstring2string. </summary>
///
/// <remarks> Dx, 2017/7/18. </remarks>
///
/// <param name="ws"> The ws. </param>
///
/// <returns> A string. </returns>
///-------------------------------------------------------------------------------------------------
std::string ws2s(const std::wstring& ws);

///-------------------------------------------------------------------------------------------------
/// <summary> string2wstring. </summary>
///
/// <remarks> Dx, 2017/7/18. </remarks>
///
/// <param name="s"> The string to process. </param>
///
/// <returns> A wstring. </returns>
///-------------------------------------------------------------------------------------------------
std::wstring s2ws(const std::string& s);

///-------------------------------------------------------------------------------------------------
/// <summary> Byte数据转成文本. </summary>
///
/// <remarks> Dx, 2018/1/8. </remarks>
///
/// <param name="data">   字节数组起始位置. </param>
/// <param name="length"> 长度. </param>
///
/// <returns> 十六进制字符串. </returns>
///-------------------------------------------------------------------------------------------------
std::string byte2str(const void* data, int length);

///-------------------------------------------------------------------------------------------------
/// <summary> 得到当前时间戳. </summary>
///
/// <remarks> Dx, 2018/12/14. </remarks>
///
/// <returns> The time. </returns>
///-------------------------------------------------------------------------------------------------
std::string secTimeStr();

void setConsoleMode();
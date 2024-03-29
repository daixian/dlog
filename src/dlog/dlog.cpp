﻿// dlog.cpp : 定义 DLL 应用程序的导出函数。
//
#pragma execution_character_set("utf-8")
#include "dlog.h"
#include "./Common/JsonHelper.h"

#include "./Common/Debug.h"
#include "./Common/MemoryLog.h"
#include "./Common/FileHelper.h"

#include <Poco/UTF8Encoding.h>
#include <Poco/Windows936Encoding.h>
#include <Poco/TextConverter.h>

using namespace dlog;

extern "C" DLOG_EXPORT void* __cdecl dlog_global_ptr()
{
    return Debug::GetInst();
}

extern "C" DLOG_EXPORT int __cdecl dlog_init(const char* logDir, const char* program, dlog_init_relative dir_relatvie,
                                             bool isForceInit)
{
    // 这个函数要主动清空初始化错误标志
    Debug::GetInst()->isInitFail = false;

    if (isForceInit == false) {
        if (!Debug::GetInst()->isInit) {
            Debug::GetInst()->Init(logDir, program, (INIT_RELATIVE)dir_relatvie);
            if (Debug::GetInst()->isInitFail)
                return -1;
            return 0; // 第一次初始化
        }
        return 1; // 成功复用
    }
    else {
        // 如果设置了强制初始化
        if (Debug::GetInst()->programName.compare(program) != 0 && // 如果两次设置的程序名不一致，那么才删除
            strcmp(program, "dlog") != 0) {                        // 同时第二次设置的这个程序名不能等于默认名字
            Debug::GetInst()->clear();
            Debug::GetInst()->Init(logDir, program, (INIT_RELATIVE)dir_relatvie);
            if (Debug::GetInst()->isInitFail)
                return -1;
            return 2; // 强制重设了一次glog
        }
        return 3; // 强制重设了一次glog
    }
}
extern "C" DLOG_EXPORT int __cdecl dlog_init_wchar_filename(const wchar_t* wLogDir,
                                                            const wchar_t* wProgram,
                                                            dlog_init_relative dir_relatvie,
                                                            bool isForceInit)
{
    std::string logDir = JsonHelper::utf16To8(std::wstring(wLogDir));
    std::string program = JsonHelper::utf16To8(std::wstring(wProgram));
    return dlog_init(logDir.c_str(), program.c_str(), dir_relatvie, isForceInit);
}

extern "C" DLOG_EXPORT int __cdecl dlog_close()
{
    if (Debug::GetInst()->isInit && Debug::GetInst()->isPrintInternalMessage)
        LogI("dlog关闭!");
    Debug::GetInst()->clear();
    return 0;
}

extern "C" DLOG_EXPORT bool __cdecl dlog_is_initialized()
{
    return Debug::GetInst()->isInit;
}

extern "C" DLOG_EXPORT bool __cdecl dlog_is_init_fail()
{
    return Debug::GetInst()->isInitFail;
}

extern "C" DLOG_EXPORT void __cdecl dlog_enable(bool enable)
{
    Debug::GetInst()->isEnable = enable;
}

extern "C" DLOG_EXPORT void __cdecl dlog_console_log_enable(bool enable)
{
    if (!Debug::GetInst()->isInit)
        Debug::GetInst()->isConsoleEnable = enable;
    else
        Debug::GetInst()->setIsConsoleEnable(enable);
}

extern "C" DLOG_EXPORT void __cdecl dlog_file_log_enable(bool enable)
{
    Debug::GetInst()->isFileEnable = enable;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_file_thr(dlog_level usualThr)
{
    Debug::GetInst()->logFileThr = static_cast<spdlog::level::level_enum>(usualThr);
}

extern "C" DLOG_EXPORT int __cdecl dlog_get_file_thr()
{
    return Debug::GetInst()->logFileThr;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_memory_thr(dlog_level memoryThr)
{
    Debug::GetInst()->logMemoryThr = static_cast<spdlog::level::level_enum>(memoryThr);
}

extern "C" DLOG_EXPORT int __cdecl dlog_get_memory_thr()
{
    return Debug::GetInst()->logMemoryThr;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_console_thr(dlog_level LogSeverity)
{
    Debug::GetInst()->logConsoleThr = static_cast<spdlog::level::level_enum>(LogSeverity); // 当日志级别大于等于此级别时，自动将此日志输出到标准错误(终端窗口)中
}

extern "C" DLOG_EXPORT int __cdecl dlog_get_console_thr()
{
    return (int)Debug::GetInst()->logConsoleThr;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_flush_on(dlog_level LogSeverity)
{
    Debug::GetInst()->setFlushOn(static_cast<spdlog::level::level_enum>(LogSeverity));
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_flush_every(int second)
{
    spdlog::flush_every(std::chrono::seconds(second)); // 每秒自动刷新一次
}

extern "C" DLOG_EXPORT void __cdecl dlog_flush()
{
    Debug::GetInst()->flush();
}

extern "C" DLOG_EXPORT void __cdecl LogI(const char* strFormat, ...)
{
    if (Debug::GetInst()->isNeedLog(spdlog::level::level_enum::info)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE, '\0');
        int ret = 0;
        va_list arg_ptr;
        va_start(arg_ptr, strFormat);
        // 最多执行4次增加buff长度
        for (size_t count = 0; count < 4; count++) {
            // vsnprintf的返回是不包含\0的预留位置的
            ret = vsnprintf(buf.data(), buf.size(), strFormat, arg_ptr);
            if (ret <= 0) {
                // 编码失败
                break;
            }
            else if (ret >= buf.size()) {
                // buff长度不够
                buf.resize((size_t)ret * 4, '\0');
                // 在GCC平台需要重新生成一下arg_ptr
                va_end(arg_ptr);
                va_start(arg_ptr, strFormat);
            }
            else {
                // 编码成功
                break;
            }
        }
        va_end(arg_ptr);
        Debug::GetInst()->LogMsg(spdlog::level::level_enum::info, buf.data());
    }
}

extern "C" DLOG_EXPORT void __cdecl LogW(const char* strFormat, ...)
{
    if (Debug::GetInst()->isNeedLog(spdlog::level::level_enum::warn)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE, '\0');
        int ret = 0;
        va_list arg_ptr;
        va_start(arg_ptr, strFormat);
        // 最多执行4次增加buff长度
        for (size_t count = 0; count < 4; count++) {
            // vsnprintf的返回是不包含\0的预留位置的
            ret = vsnprintf(buf.data(), buf.size(), strFormat, arg_ptr);
            if (ret <= 0) {
                // 编码失败
                break;
            }
            else if (ret >= buf.size()) {
                // buff长度不够
                buf.resize((size_t)ret * 4, '\0');
                // 在GCC平台需要重新生成一下arg_ptr
                va_end(arg_ptr);
                va_start(arg_ptr, strFormat);
            }
            else {
                // 编码成功
                break;
            }
        }
        va_end(arg_ptr);
        Debug::GetInst()->LogMsg(spdlog::level::level_enum::warn, buf.data());
    }
}

extern "C" DLOG_EXPORT void __cdecl LogE(const char* strFormat, ...)
{
    if (Debug::GetInst()->isNeedLog(spdlog::level::level_enum::err)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE, '\0');
        int ret = 0;
        va_list arg_ptr;
        va_start(arg_ptr, strFormat);
        // 最多执行4次增加buff长度
        for (size_t count = 0; count < 4; count++) {
            // vsnprintf的返回是不包含\0的预留位置的
            ret = vsnprintf(buf.data(), buf.size(), strFormat, arg_ptr);
            if (ret <= 0) {
                // 编码失败
                break;
            }
            else if (ret >= buf.size()) {
                // buff长度不够
                buf.resize((size_t)ret * 4, '\0');
                // 在GCC平台需要重新生成一下arg_ptr
                va_end(arg_ptr);
                va_start(arg_ptr, strFormat);
            }
            else {
                // 编码成功
                break;
            }
        }
        va_end(arg_ptr);
        Debug::GetInst()->LogMsg(spdlog::level::level_enum::err, buf.data());
    }
}

extern "C" DLOG_EXPORT void __cdecl LogD(const char* strFormat, ...)
{
    if (Debug::GetInst()->isNeedLog(spdlog::level::level_enum::debug)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE, '\0');
        int ret = 0;
        va_list arg_ptr;
        va_start(arg_ptr, strFormat);
        // 最多执行4次增加buff长度
        for (size_t count = 0; count < 4; count++) {
            // vsnprintf的返回是不包含\0的预留位置的
            ret = vsnprintf(buf.data(), buf.size(), strFormat, arg_ptr);
            if (ret <= 0) {
                // 编码失败
                break;
            }
            else if (ret >= buf.size()) {
                // buff长度不够
                buf.resize((size_t)ret * 4, '\0');
                // 在GCC平台需要重新生成一下arg_ptr
                va_end(arg_ptr);
                va_start(arg_ptr, strFormat);
            }
            else {
                // 编码成功
                break;
            }
        }
        va_end(arg_ptr);
        Debug::GetInst()->LogMsg(spdlog::level::level_enum::debug, buf.data());
    }
}

extern "C" DLOG_EXPORT void __cdecl wLogI(const wchar_t* strFormat, ...)
{
    if (Debug::GetInst()->isNeedLog(spdlog::level::level_enum::info)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        // 这个函数原来使用的是vswprintf，但是跨平台有问题，如果包含中文则失败。
        rapidjson::StringBuffer buffUtf8;
        if (!JsonHelper::utf16To8(strFormat, buffUtf8)) {
            return; // 如果转码失败就直接返回
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE);
        int ret = 0;
        va_list arg_ptr;
        va_start(arg_ptr, strFormat);
        // vsnprintf的返回是不包含\0的预留位置的
        while ((ret = vsnprintf(buf.data(), buf.size(), buffUtf8.GetString(), arg_ptr)) >= buf.size()) {
            buf.resize((size_t)ret + 1, '\0');
            // 在GCC平台需要重新生成一下arg_ptr
            va_end(arg_ptr);
            va_start(arg_ptr, strFormat);
        }
        va_end(arg_ptr);

        Debug::GetInst()->LogMsg(spdlog::level::level_enum::info, buf.data());
    }
}

extern "C" DLOG_EXPORT void __cdecl wLogW(const wchar_t* strFormat, ...)
{
    if (Debug::GetInst()->isNeedLog(spdlog::level::level_enum::warn)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        // 这个函数原来使用的是vswprintf，但是跨平台有问题，如果包含中文则失败。
        rapidjson::StringBuffer buffUtf8;
        if (!JsonHelper::utf16To8(strFormat, buffUtf8)) {
            return; // 如果转码失败就直接返回
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE, '\0');
        int ret = 0;
        va_list arg_ptr;
        va_start(arg_ptr, strFormat);
        // 最多执行4次增加buff长度
        for (size_t count = 0; count < 4; count++) {
            // vsnprintf的返回是不包含\0的预留位置的
            ret = vsnprintf(buf.data(), buf.size(), buffUtf8.GetString(), arg_ptr);
            if (ret <= 0) {
                // 编码失败
                break;
            }
            else if (ret >= buf.size()) {
                // buff长度不够
                buf.resize((size_t)ret * 4, '\0');
                // 在GCC平台需要重新生成一下arg_ptr
                va_end(arg_ptr);
                va_start(arg_ptr, strFormat);
            }
            else {
                // 编码成功
                break;
            }
        }
        va_end(arg_ptr);
        Debug::GetInst()->LogMsg(spdlog::level::level_enum::warn, buf.data());
    }
}

extern "C" DLOG_EXPORT void __cdecl wLogE(const wchar_t* strFormat, ...)
{
    if (Debug::GetInst()->isNeedLog(spdlog::level::level_enum::err)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        // 这个函数原来使用的是vswprintf，但是跨平台有问题，如果包含中文则失败。
        rapidjson::StringBuffer buffUtf8;
        if (!JsonHelper::utf16To8(strFormat, buffUtf8)) {
            return; // 如果转码失败就直接返回
        }

        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE, '\0');
        int ret = 0;
        va_list arg_ptr;
        va_start(arg_ptr, strFormat);
        // 最多执行4次增加buff长度
        for (size_t count = 0; count < 4; count++) {
            // vsnprintf的返回是不包含\0的预留位置的
            ret = vsnprintf(buf.data(), buf.size(), buffUtf8.GetString(), arg_ptr);
            if (ret <= 0) {
                // 编码失败
                break;
            }
            else if (ret >= buf.size()) {
                // buff长度不够
                buf.resize((size_t)ret * 4, '\0');
                // 在GCC平台需要重新生成一下arg_ptr
                va_end(arg_ptr);
                va_start(arg_ptr, strFormat);
            }
            else {
                // 编码成功
                break;
            }
        }
        va_end(arg_ptr);
        Debug::GetInst()->LogMsg(spdlog::level::level_enum::err, buf.data());
    }
}

extern "C" DLOG_EXPORT void __cdecl wLogD(const wchar_t* strFormat, ...)
{
    if (Debug::GetInst()->isNeedLog(spdlog::level::level_enum::debug)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        // 这个函数原来使用的是vswprintf，但是跨平台有问题，如果包含中文则失败。
        rapidjson::StringBuffer buffUtf8;
        if (!JsonHelper::utf16To8(strFormat, buffUtf8)) {
            return; // 如果转码失败就直接返回
        }
        std::vector<char> buf(DEBUG_LOG_BUFF_SIZE, '\0');
        int ret = 0;
        va_list arg_ptr;
        va_start(arg_ptr, strFormat);
        // 最多执行4次增加buff长度
        for (size_t count = 0; count < 4; count++) {
            // vsnprintf的返回是不包含\0的预留位置的
            ret = vsnprintf(buf.data(), buf.size(), buffUtf8.GetString(), arg_ptr);
            if (ret <= 0) {
                // 编码失败
                break;
            }
            else if (ret >= buf.size()) {
                // buff长度不够
                buf.resize((size_t)ret * 4, '\0');
                // 在GCC平台需要重新生成一下arg_ptr
                va_end(arg_ptr);
                va_start(arg_ptr, strFormat);
            }
            else {
                // 编码成功
                break;
            }
        }
        va_end(arg_ptr);
        Debug::GetInst()->LogMsg(spdlog::level::level_enum::debug, buf.data());
    }
}

extern "C" DLOG_EXPORT void __cdecl LogMsg(dlog_level level, const char* message)
{
    if (Debug::GetInst()->isNeedLog((spdlog::level::level_enum)level)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }
        Debug::GetInst()->LogMsg((spdlog::level::level_enum)level, message);
    }
}

extern "C" DLOG_EXPORT void __cdecl wLogMsg(dlog_level level, const wchar_t* message)
{

    if (Debug::GetInst()->isNeedLog((spdlog::level::level_enum)level)) {
        if (!Debug::GetInst()->isInit && !Debug::GetInst()->isInitFail) { // 如果还没有初始化过，那么就调用默认构造
            Debug::GetInst()->Init("log", "dlog", INIT_RELATIVE::APPDATA);
        }

        rapidjson::StringBuffer buffUtf8;
        if (JsonHelper::utf16To8(message, buffUtf8)) {
            Debug::GetInst()->LogMsg((spdlog::level::level_enum)level, buffUtf8.GetString());
        }
    }
}

extern "C" DLOG_EXPORT int __cdecl dlog_convert_utf8_to_gbk(const char* s_utf8, char* dst, int size)
{
    Poco::Windows936Encoding cp936;
    Poco::UTF8Encoding utf8;
    Poco::TextConverter converter(utf8, cp936);
    std::string s_gbk;
    converter.convert(s_utf8, s_gbk);
    int copyLen = (int)s_gbk.size() < (size - 1) ? (int)s_gbk.size() : (size - 1); // 这里string的size是否就等于字节的size???
    s_gbk.copy(dst, copyLen);
    dst[copyLen] = '\0';
    return copyLen;
}

extern "C" DLOG_EXPORT int __cdecl dlog_convert_gbk_to_utf8(const char* s_gbk, char* dst, int size)
{
    Poco::Windows936Encoding cp936;
    Poco::UTF8Encoding utf8;
    Poco::TextConverter converter(cp936, utf8);
    std::string s_utf8;
    converter.convert(s_gbk, s_utf8);
    int copyLen = (int)s_utf8.size() < (size - 1) ? (int)s_utf8.size() : (size - 1); // 这里string的size是否就等于字节的size???
    s_utf8.copy(dst, copyLen);
    dst[copyLen] = '\0';
    return copyLen;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_logger_function(DlogLoggerCallback fp)
{
    Debug::GetInst()->exLoggerCallback = fp;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_encrypt_function(DlogLoggerEncryptCallback fpEncrypt,
                                                              DlogLoggerEncrypDeletetCallback fpDelete)
{
    Debug::GetInst()->exLoggerEncryptCallback = fpEncrypt;
    Debug::GetInst()->exLoggerEncrypDeletetCallback = fpDelete;
    Debug::GetInst()->isEncryptFile = true;
    Debug::GetInst()->isEncryptConsole = false;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_is_encrypt_file(bool isEncryptFile)
{
    Debug::GetInst()->isEncryptFile = isEncryptFile;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_is_encrypt_console(bool isEncryptConsole)
{
    Debug::GetInst()->isEncryptConsole = isEncryptConsole;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_is_chcp65001(bool ischcp65001)
{
    Debug::GetInst()->ischcp65001 = ischcp65001;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_is_print_internal_message(bool isPrint)
{
    Debug::GetInst()->isPrintInternalMessage = isPrint;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_is_use_utf8_bom(bool isUTF8BOM)
{
    Debug::GetInst()->isUTF8BOM = isUTF8BOM;
}

#pragma region memory log

extern "C" DLOG_EXPORT void __cdecl dlog_memory_log_enable(bool enable)
{
    Debug::GetInst()->isMemLogEnable = enable;
}

extern "C" DLOG_EXPORT int __cdecl dlog_get_memlog(char* buff, int offset, int length)
{
    std::string msg;
    int copyLen = 0;
    if (MemoryLog::GetInst()->getLog(msg)) {
        copyLen = (int)msg.size() < (length - 1) ? (int)msg.size() : (length - 1); // 这里string的size是否就等于字节的size???
        msg.copy(buff + offset, copyLen);
    }
    else {
        return 0;
    }
    buff[copyLen] = '\0';
    return copyLen;
}

extern "C" DLOG_EXPORT void __cdecl dlog_set_memlog_save_limit(unsigned int maxCount)
{
    MemoryLog::GetInst()->maxLen = maxCount;
}

#pragma endregion

#pragma region 其他辅助功能

extern "C" DLOG_EXPORT int __cdecl dlog_get_appdata_dir(char* buff, int size)
{
    std::string dir = FileHelper::getAppDir();
    int copyLen = (int)dir.size() < (size - 1) ? (int)dir.size() : (size - 1); // 这里string的size是否就等于字节的size???
    dir.copy(buff, copyLen);
    buff[copyLen] = '\0';
    return copyLen;
}

extern "C" DLOG_EXPORT int __cdecl dlog_get_module_dir(char* buff, int size)
{
    std::string dir = FileHelper::getModuleDir();
    int copyLen = (int)dir.size() < (size - 1) ? (int)dir.size() : (size - 1); // 这里string的size是否就等于字节的size???
    dir.copy(buff, copyLen);
    buff[copyLen] = '\0';
    return copyLen;
}

extern "C" DLOG_EXPORT int __cdecl dlog_get_log_dir(char* buff, int size)
{
    std::string dir = Debug::GetInst()->logDirPath;
    int copyLen = (int)dir.size() < (size - 1) ? (int)dir.size() : (size - 1); // 这里string的size是否就等于字节的size???
    dir.copy(buff, copyLen);
    buff[copyLen] = '\0';
    return copyLen;
}

extern "C" DLOG_EXPORT int __cdecl dlog_get_log_file_path(char* buff, int size)
{
    std::string dir = Debug::GetInst()->logFilePath;
    int copyLen = (int)dir.size() < (size - 1) ? (int)dir.size() : (size - 1); // 这里string的size是否就等于字节的size???
    dir.copy(buff, copyLen);
    buff[copyLen] = '\0';
    return copyLen;
}
#pragma endregion

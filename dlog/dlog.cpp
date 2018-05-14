// dlog.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "dlog.h"

#include "Common/GLogHelper.h"
#include "Common/Debug.h"
#include "Common/MemoryLog.h"
#include <mutex>

using namespace dxlib;

//�����������ڳ�ʼ���͹رյ�ʱ��
std::mutex mt;

//ȫ�ֶ���������dll�ͷŵ�ʱ���Զ�����
static GLogHelper* volatile inst = NULL;

///-------------------------------------------------------------------------------------------------
/// <summary> ģ���ʼ��. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="program"> [in]������. </param>
/// <param name="logDir">  [in]��־�ļ���·���������ģ��Ŀ¼��. </param>
///
/// <returns> ���֮ǰδ����ʼ������0�����򷵻�1. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_init(char* logDir, char* program)
{
    mt.lock();
    if (inst == NULL)
    {
        inst = new GLogHelper(program, logDir);
        mt.unlock();
        return 0;
    }
    else
    {
        if (inst->programName.compare(program) != 0 &&//����������õĳ�������һ�£���ô��ɾ��
            strcmp(program, "dlog") != 0)//ͬʱ�ڶ������õ�������������ܵ���Ĭ������
        {
            delete inst;
            inst = new GLogHelper(program, logDir);
        }
        mt.unlock();
        return 1;
    }
}

///-------------------------------------------------------------------------------------------------
/// <summary> �ر�ģ��. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <returns> An int. </returns>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT int __stdcall dlog_close()
{
    mt.lock();
    if (inst != NULL)
    {
        delete inst;
        inst = NULL;
        mt.unlock();
        return 0;
    }
    mt.unlock();
    return 1;
}

///-------------------------------------------------------------------------------------------------
/// <summary> �õ���ǰ���õ���־Ŀ¼. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="result"> [out] If non-null, the result. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_get_log_dir(char* result)
{
    if (inst != NULL)
    {
        inst->logDirPath.copy(result, inst->logDirPath.size());
    }
}

///-------------------------------------------------------------------------------------------------
/// <summary> ��������logʹ��. </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="enable"> ����Ϊfalse֮��Log������ֱ�ӷ��ز����κβ���. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_enable(bool enable)
{
    Debug::GetInst()->isLog = enable;
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// ���ô��ڵ���ĳһ����־���������̨. ������DLOG_INFO,DLOG_WARNING,DLOG_ERROR,
/// DLOG_FATAL.
/// </summary>
///
/// <remarks> Dx, 2018/4/23. </remarks>
///
/// <param name="LogSeverity"> ���ڵ�����һ������־�������������̨. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall dlog_FLAGS_stderrthreshold(int LogSeverity)
{
    FLAGS_stderrthreshold = LogSeverity; // ����־������ڵ��ڴ˼���ʱ���Զ�������־�������׼����(�ն˴���)��
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an info. </summary>
///
/// <remarks> Dx, 2018/4/22. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogI(const char * strFormat, ...)
{
    if (inst == NULL)//�����û�г�ʼ��������ô�͵���Ĭ�Ϲ���
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogI_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a w. </summary>
///
/// <remarks> Dx, 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogW(const char * strFormat, ...)
{
    if (inst == NULL)//�����û�г�ʼ��������ô�͵���Ĭ�Ϲ���
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogW_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs an e. </summary>
///
/// <remarks> Dx, 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogE(const char * strFormat, ...)
{
    if (inst == NULL)//�����û�г�ʼ��������ô�͵���Ĭ�Ϲ���
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogE_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

///-------------------------------------------------------------------------------------------------
/// <summary> Logs a fatal. </summary>
///
/// <remarks> Dx                                                                                                                                                                                                                     , 2018/5/10. </remarks>
///
/// <param name="strFormat"> Describes the format to
///                          use. </param>
/// <param name="...">       Variable arguments
///                          providing additional
///                          information. </param>
///-------------------------------------------------------------------------------------------------
extern "C" DLOG_EXPORT void __stdcall LogFATAL(const char * strFormat, ...)
{
    if (inst == NULL)//�����û�г�ʼ��������ô�͵���Ĭ�Ϲ���
    {
        dlog_init();
    }

    va_list arg_ptr = NULL;
    va_start(arg_ptr, strFormat);
    Debug::LogFATAL_va(strFormat, arg_ptr);
    va_end(arg_ptr);
}

#pragma region memory log

///-------------------------------------------------------------------------------------------------
/// <summary> �����ڴ�log�Ƿ�ʹ��. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="enable"> ����Ϊfalse֮��Log������ֱ�ӷ��ز����κβ���. </param>
///-------------------------------------------------------------------------------------------------
DLOG_EXPORT void dlog_memory_log_enable(bool enable)
{
    Debug::GetInst()->isLogMemory = enable;
}


///-------------------------------------------------------------------------------------------------
/// <summary> ��ȡһ���ڴ���־. </summary>
///
/// <remarks> Dx, 2018/5/11. </remarks>
///
/// <param name="buff">   [in,out] ����buffer. </param>
/// <param name="offset"> The offset. </param>
/// <param name="count">  Number of. </param>
///
/// <returns> ��ȡ������־���ȣ����Ϊ0��ʾû�������־��. </returns>
///-------------------------------------------------------------------------------------------------
DLOG_EXPORT int dlog_get_memlog(char * buff, int offset, int count)
{
    std::string msg;
    int copyLen = 0;
    if (MemoryLog::GetInst()->getLog(msg))
    {
        copyLen = msg.size() < count ? msg.size() : count;
        msg.copy(buff, copyLen);
    }
    buff[copyLen] = 0;
    return copyLen;
}

#pragma endregion
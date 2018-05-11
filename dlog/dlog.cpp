// dlog.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "dlog.h"

#include "Common/GLogHelper.h"
#include "Common/Debug.h"

using namespace dxlib;

//ȫ�ֶ���������dll�ͷŵ�ʱ���Զ�����
GLogHelper * inst = NULL;

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
    if (inst == NULL)
    {
        inst = new GLogHelper(program, logDir);
        return 0;
    }

    return 1;
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
    if (inst != NULL)
    {
        delete inst;
        inst = NULL;
        return 0;
    }
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
        inst->logDirPath.copy(result, FLAGS_log_dir.size(), 0);
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

extern "C" DLOG_EXPORT void __stdcall LogTest(int i)
{
    if (inst == NULL)//�����û�г�ʼ��������ô�͵���Ĭ�Ϲ���
    {
        dlog_init();
    }

    LOG(INFO) << "������־" << i;
}
#pragma once

#include "../glog/logging.h"
#include "../glog/raw_logging.h"

#include "Common.h"

class GLogHelper
{
public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary> ���죬ͬʱ����glog����. </summary>
    ///
    /// <remarks> Dx, 2018/4/22. </remarks>
    ///
    /// <param name="program"> [in]������. </param>
    /// <param name="logDir">  [in]��־�ļ���·���������ģ��Ŀ¼��. </param>
    ///-------------------------------------------------------------------------------------------------
    GLogHelper(char* program, char* logDir = "\\log");
    //GLOG�ڴ�����
    ~GLogHelper();

    //����ص�����δ���ã�����Ϣ������������ļ��� LOG(ERROR)
    void static SignalHandle(const char* data, int size);

 
    bool static dirExists(const std::string& dirName_in);
    bool static dirExists(const std::wstring& dirName_in);

    //�õ�ģ��Ŀ¼ĩβ����б��"D:\\Work\\F3DSys\\F3DSystem"
    std::string static getAppDir();

    std::string static isExistsAndCreat(std::string sDir);
    std::string static isExistsAndCreat(std::wstring dirPath);

    //��־�ļ��еļ�¼(����·��) 
    std::string logDirPath;

    //��־�������ļ�¼(glog��û��api����¼������Ҫ�Լ���¼)
    std::string programName;
};
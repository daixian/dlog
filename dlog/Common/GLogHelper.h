#pragma once

#include "glog/logging.h"
#include "glog/raw_logging.h"

#include "Common.h"

class GLogHelper
{
public:
    ///-------------------------------------------------------------------------------------------------
    /// <summary> 构造，同时进行glog配置. </summary>
    ///
    /// <remarks> Dx, 2018/4/22. </remarks>
    ///
    /// <param name="program"> [in]程序名. </param>
    /// <param name="logDir">  [in]日志文件夹路径名（相对模块目录）. </param>
    ///-------------------------------------------------------------------------------------------------
    GLogHelper(char* program, char* logDir = "\\log");
    //GLOG内存清理：
    ~GLogHelper();

    //错误回调：（未能用）将信息输出到单独的文件和 LOG(ERROR)
    void static SignalHandle(const char* data, int size);

 
    bool static dirExists(const std::string& dirName_in);
    bool static dirExists(const std::wstring& dirName_in);

    //得到模块目录末尾不带斜杠"D:\\Work\\F3DSys\\F3DSystem"
    std::string static getAppDir();

    std::string static isExistsAndCreat(std::string sDir);
    std::string static isExistsAndCreat(std::wstring dirPath);

    //日志文件夹的记录(绝对路径) 
    std::string logDirPath;

    //日志程序名的记录(glog中没有api来记录，所以要自己记录)
    std::string programName;
};
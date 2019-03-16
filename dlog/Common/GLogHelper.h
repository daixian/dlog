#pragma once
#include <string>

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
    GLogHelper(const char* program, const char* logDir = "\\log");
    //GLOG内存清理：
    ~GLogHelper();

    //错误回调：（未能用）将信息输出到单独的文件和 LOG(ERROR)
    void static SignalHandle(const char* data, int size);

    //日志文件夹的记录(绝对路径)
    std::string logDirPath;

    //日志程序名的记录(glog中没有api来记录，所以要自己记录)
    std::string programName;
};
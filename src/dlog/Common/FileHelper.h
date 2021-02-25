#pragma once

#include <string>
#include <vector>
#include <Poco/Path.h>

namespace dlog {

class FileHelper
{
  public:
    static bool dirExists(const std::string& dirName_in);
    //static bool dirExists(const std::wstring& dirName_in);

    //得到模块目录末尾不带斜杠"D:\\Work\\F3DSys\\F3DSystem"
    static std::string getModuleDir();

    //得到AppData目录末尾不带斜杠"C:\\Users\\dx\\AppData\\Roaming"
    static std::string getAppDir();

    //如果文件夹不存在就直接创建文件夹（确保创建文件夹）
    static void isExistsAndCreat(const std::string& sDir);
    //static void isExistsAndCreat(const std::wstring& dirPath);

    /**
     * 判断一个目录是否是绝对路径,如果不是绝对路径就用base和它拼接.
     *
     * @author daixian
     * @date 2020/4/22
     *
     * @param          base base路径.
     * @param [in,out] path 要拼接的路径.
     */
    static void makeAbsolute(const Poco::Path& base, Poco::Path& path);
};
} // namespace dlog
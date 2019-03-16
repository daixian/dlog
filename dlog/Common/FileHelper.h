#pragma once

#include <string>
#include <vector>

namespace dxlib {

class FileHelper
{
  public:
    static bool dirExists(const std::string& dirName_in);
    static bool dirExists(const std::wstring& dirName_in);

    //得到模块目录末尾不带斜杠"D:\\Work\\F3DSys\\F3DSystem"
    static std::string getModuleDir();

    //如果文件夹不存在就直接创建文件夹（确保创建文件夹）
    static void isExistsAndCreat(std::string sDir);
    static void isExistsAndCreat(std::wstring dirPath);
};
} // namespace dxlib
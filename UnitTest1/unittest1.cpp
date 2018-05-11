#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include "../dlog/dlog.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod_dlogCPP)
        {           
            dlog_init("D:\\临时测试\\log", "测试日志");
            dlog_memory_log_enable(true);

            //LogI打印10W条，异步的，只要942毫秒
            //LogW打印10W条，也只要1秒的样子
            for (size_t i = 0; i < 100000; i++)
            {
                LogW("测试日志%d !", i);
            }

            char msg[512];
            char msgCorr[512];
            for (size_t i = 0; i < 100000; i++)
            {
                if (dlog_get_memlog(msg, 0, 512) > 0)
                {
                    sprintf(msgCorr, "测试日志%d !", i);//正确的消息应该是
                    Assert::IsTrue(strcmp(msg, msgCorr) == 0);//比对提取的消息是否正确
                }
                else
                {
                    Assert::Fail();//检测数量是否一致
                }

           
            }

 
        }

        //TEST_METHOD(TestMethod2)
        //{
        //    std::string sDir = "D:\\实验";
        //    std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");
        //    int res = system(cmd.c_str()); //创建文件夹

        //}
       
	};
}
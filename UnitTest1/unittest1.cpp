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
            dlog_init("D:\\��ʱ����\\log", "������־");

            for (size_t i = 0; i < 10000; i++)
            {
                LogI("������־%d !", i);
            }
 
        }

        //TEST_METHOD(TestMethod2)
        //{
        //    std::string sDir = "D:\\ʵ��";
        //    std::string cmd = std::string("mkdir \"") + sDir + std::string("\"");
        //    int res = system(cmd.c_str()); //�����ļ���

        //}
       
	};
}
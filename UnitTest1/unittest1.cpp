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
            dlog_memory_log_enable(true);

            //LogI��ӡ10W�����첽�ģ�ֻҪ942����
            //LogW��ӡ10W����ҲֻҪ1�������
            for (size_t i = 0; i < 100000; i++)
            {
                LogW("������־%d !", i);
            }

            char msg[512];
            char msgCorr[512];
            for (size_t i = 0; i < 100000; i++)
            {
                if (dlog_get_memlog(msg, 0, 512) > 0)
                {
                    sprintf(msgCorr, "������־%d !", i);//��ȷ����ϢӦ����
                    Assert::IsTrue(strcmp(msg, msgCorr) == 0);//�ȶ���ȡ����Ϣ�Ƿ���ȷ
                }
                else
                {
                    Assert::Fail();//��������Ƿ�һ��
                }

           
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
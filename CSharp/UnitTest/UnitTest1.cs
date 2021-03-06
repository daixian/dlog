﻿using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using xuexue;

//Copy "$(SolutionDir)x64\Release\dlog.dll" "$(ProjectDir)dlog.dll"


namespace UnitTest
{
    [TestClass]
    public class UnitTest1
    {

        /// <summary>
        /// 测试内存日志
        /// </summary>
        [TestMethod]
        public void TestMethodDLogMemlog()
        {
            ThreadPool.SetMinThreads(32, 32);

            int ThreadStartCount = 0;
            int DoneCount = 0;

            DLog.Close();//关闭

            string logDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");
            if (Directory.Exists(logDir))
            {
                Directory.Delete(logDir, true);
            }


            DLog.ConsoleLogEnable(false);//禁用控制台,如果不禁用的话时间可能很长
            DLog.Init(logDir);
            Assert.IsTrue(DLog.IsInitialized());

            Thread.Sleep(500);
            int testMsgNum = 500;
            //DLog.dlog_set_usual_thr(DLog.DLOG_ERROR + 1);

            DLog.MemoryLogEnable(true);

            for (int threadCount = 0; threadCount < 20; threadCount++)//20个线程一起写
            {
                Task.Run(() =>
                {
                    Interlocked.Increment(ref ThreadStartCount);
                    //要注意条数不能太多了，否则超出内存日志的缓存上限，会造成后面Assert条数失败
                    for (int i = 0; i < testMsgNum; i++)
                    {
                        xuexue.DLog.LogI($"测试日志{i}");

                    }
                    Interlocked.Increment(ref DoneCount);
                });
            }

            while (true)
            {
                Thread.Sleep(100);
                if (DoneCount == ThreadStartCount)
                {
                    break;
                }
            }


            int msgCount = 0;
            while (true)
            {
                string msg = DLog.GetMemorylog();
                if (!String.IsNullOrEmpty(msg))
                {
                    msgCount++;
                    Assert.IsTrue(msg.ToString().Contains("测试日志"));//检查文本内容是否有大问题
                }
                else
                {
                    break;
                }
            }

            Assert.IsTrue(msgCount == 20 * testMsgNum);//检查消息条数是否漏了

            DLog.Close();//必须要关闭，否则有线程在后台还

        }

        /// <summary>
        /// 测试初始化和关闭
        /// </summary>
        [TestMethod]
        public void TestMethodDLogInitClose()
        {
            DLog.Close();
            Assert.IsFalse(DLog.IsInitialized());
            string logDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");
            if (Directory.Exists(logDir))
            {
                Directory.Delete(logDir, true);
            }

            int count = 0;
            int res = 0;
            for (int i = 0; i < 50; i++)
            {
                res = DLog.Init(logDir, $"openclose{count}");
                Assert.IsTrue(DLog.IsInitialized());
                Assert.IsTrue(res == 0);
                DLog.LogI($"开关测试log！{count}");
                DLog.Close();
                count++;
            }

            for (int i = 0; i < 50; i++)
            {
                res = DLog.Init(logDir, $"开关测试{count}");
                Assert.IsTrue(res == 0);
                DLog.LogI($"开关测试log！{count}");
                res = DLog.Init(logDir, $"开关测试{count}");
                Assert.IsTrue(res == 1);
                DLog.LogI($"开关测试log！{count}");
                DLog.Close();
                count++;
            }


            for (int i = 0; i < 50; i++)
            {
                DLog.Init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.Init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.Init(logDir, $"开关测试{count}");
                DLog.Close();
                count++;
            }

            for (int i = 0; i < 50; i++)
            {
                DLog.Init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.Init(logDir, $"开关测试二{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.Init(logDir, $"开关测试二{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.Close();
                DLog.Close();
                count++;
            }

            for (int i = 0; i < 50; i++)
            {
                DLog.Init(logDir, $"开关测试{count}");
                DLog.LogI($"开关测试log！{count}");
                DLog.Close();
                DLog.Close();
                count++;
            }

            string[] logfiles = Directory.GetFiles(logDir);
            Assert.IsTrue(logfiles.Length == count);

            DLog.Close();//关闭
        }

        /// <summary>
        /// 测试多线程调用log
        /// </summary>
        [TestMethod]
        public void TestMethodDLogMT()
        {
            DLog.Close();//关闭

            string logDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "log");
            if (Directory.Exists(logDir))
            {
                Directory.Delete(logDir, true);
            }
            xuexue.DLog.Init(logDir, "线程测试");

            //设置一个一般的控制变量，多个线程对它读写
            int controlFlag = 0;

            int doneCount = 0;

            Interlocked.Increment(ref doneCount);
            Task.Run(() =>
            {
                int count = 0;
                while (count < 1000)
                {
                    //Thread.Sleep(1);
                    if (controlFlag == 0)
                    {
                        Interlocked.Increment(ref controlFlag);
                        DLog.LogI($"开始工作!{count}");
                        //Thread.Sleep(1);
                        count++;
                        DLog.LogI($"工作结束!{count}");
                        Interlocked.Decrement(ref controlFlag);
                    }
                    else
                    {
                        DLog.LogI($"等待... {controlFlag}");
                    }
                }
                Interlocked.Decrement(ref doneCount);
            });

            Interlocked.Increment(ref doneCount);
            Task.Run(() =>
            {
                int count = 0;
                while (count < 1000)
                {
                    //Thread.Sleep(1);
                    if (controlFlag == 0)
                    {
                        Interlocked.Increment(ref controlFlag);
                        DLog.LogI($"开始工作!{count}");
                        //Thread.Sleep(1);
                        count++;
                        DLog.LogI($"工作结束!{count}");
                        Interlocked.Decrement(ref controlFlag);
                    }
                    else
                    {
                        DLog.LogI($"等待... {controlFlag}");
                    }
                }
                Interlocked.Decrement(ref doneCount);
            });


            Interlocked.Increment(ref doneCount);
            Task.Run(() =>
            {
                int count = 0;
                while (count < 1000)
                {
                    //Thread.Sleep(1);
                    if (controlFlag == 0)
                    {
                        Interlocked.Increment(ref controlFlag);//这里++还是不行
                        DLog.LogI($"开始工作!{count}");
                        //Thread.Sleep(1);
                        count++;
                        DLog.LogI($"工作结束!{count}");
                        Interlocked.Decrement(ref controlFlag);//这里--还是不行
                    }
                    else
                    {
                        DLog.LogI($"等待... {controlFlag}");
                    }
                }
                Interlocked.Decrement(ref doneCount);
            });


            Interlocked.Increment(ref doneCount);
            Task.Run(() =>
            {
                int count = 0;
                while (count < 1000)
                {
                    //Thread.Sleep(1);
                    if (controlFlag == 0)
                    {
                        Interlocked.Increment(ref controlFlag);
                        DLog.LogI($"开始工作!{count}");
                        //Thread.Sleep(1);
                        count++;
                        DLog.LogI($"工作结束!{count}");
                        Interlocked.Decrement(ref controlFlag);
                    }
                    else
                    {
                        DLog.LogI($"等待... {controlFlag}");
                    }
                }
                Interlocked.Decrement(ref doneCount);
            });

            while (true)
            {
                Thread.Sleep(1);
                if (doneCount == 0)
                {
                    break;
                }
            }

            DLog.Close();
        }


        [DllImport("Dll1")]
        public static extern int Fun1();

        [DllImport("Dll2")]
        public static extern int Fun2();

        /// <summary>
        /// cpp和c#部分融合使用
        /// </summary>
        [TestMethod]
        public void TestMethodDLogCPPCSharp()
        {
            //测试50次
            for (int i = 0; i < 20; i++)
            {
                //DLog.Close();

                //int res;
                //res = DLog.Init("log", "MRSystem");
                //Assert.IsTrue(res == 0);//c#部分成功复用

                ////调用DLL1里的函数
                //res = Fun1();//先在cpp部分init,实际调用了一句 dlog_init("\\log", "MRSystem", false);
                //Assert.IsTrue(res == 1);//cpp部分创建成功了

                //res = Fun2();//
                //Assert.IsTrue(res == 1);//dll2应该是成功复用

                //DLog.(true);//使能内存日志(库默认不使能)
                //int res2 = DLog.Init("log", "MRSystem");
                //Assert.IsTrue(res2 == 1);//c#部分成功复用

                //DLog.LogI("这是第二条日志");//内存日志的第一条
                //StringBuilder sb = new StringBuilder();

                //int success = DLog.dlog_get_memlog(sb, 0, 1024);
                //Assert.IsTrue(success == 14);//第一条内存日志应该提取成功
                //Assert.IsTrue(sb.ToString() == "这是第二条日志");


                //success = DLog.dlog_get_memlog(sb, 0, 1024);
                //Assert.IsTrue(success == 0);

                //int res3 = DLog.Init()("\\log", "MRSystem2", true);


                //Assert.IsTrue(res3 == 2);
            }
        }


        /// <summary>
        /// 测试内存日志
        /// </summary>
        [TestMethod]
        public void TestPathGet()
        {
            string ad = DLog.GetAppdataDir();
            string md = DLog.GetModuleDir();
            Assert.IsTrue(ad != md);
        }

        //[TestMethod]
        //public void Test_INIT_RELATIVE_MODULE()
        //{

        //string md = DLog.dlog_get_module_dir();
        //DLog.Close();
        //StringBuilder sb = new StringBuilder(128);
        //sb.Append("123");
        //DLog.Init()("\\LOG", "MRSystem", DLog.INIT_RELATIVE.MODULE, false);

        //string logDir = DLog.dlog_get_log_dir();
        //Assert.IsTrue(logDir == Path.Combine(md, "LOG"));
        //}

        /// <summary>
        /// 测试修改日志等级
        /// </summary>
        [TestMethod]
        public void TestMethod_thrTest()
        {
            DLog.Close();
            DLog.SetConsoleThr(DLog.LOG_THR.err);//设置只有err级别的日志才输出到控制台
            Assert.IsTrue(DLog.GetConsoleThr() == (int)DLog.LOG_THR.err);

            DLog.SetFileThr(DLog.LOG_THR.err);
            Assert.IsTrue(DLog.GetFileThr() == (int)DLog.LOG_THR.err);

            DLog.SetMemoryThr(DLog.LOG_THR.err);
            Assert.IsTrue(DLog.GetMemoryThr() == (int)DLog.LOG_THR.err);
        }

        [TestMethod]
        public void TestMethod_flush()
        {
            DLog.Close();
            for (int i = 0; i < 10; i++)
            {
                DLog.Flush();
            }
            for (int i = 0; i < 10; i++)
            {
                DLog.LogI("123");
                DLog.Flush();
            }
            DLog.Close();
        }
    }
}

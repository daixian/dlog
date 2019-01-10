#pragma once

#include "../../concurrentqueue/concurrentqueue.h"
#include "../../concurrentqueue/blockingconcurrentqueue.h"

namespace dxlib {
    ///-------------------------------------------------------------------------------------------------
    /// <summary>
    /// 内存中缓存的日志，它一般用来给C#部分的程序轮询提取，然后在UI界面上显示.
    /// </summary>
    ///
    /// <remarks> Dx, 2018/5/11. </remarks>
    ///-------------------------------------------------------------------------------------------------
    class MemoryLog
    {
    public:
        MemoryLog() {}

        static MemoryLog* GetInst()
        {
            if (m_pInstance == NULL)  //判断是否第一次调用
                m_pInstance = new MemoryLog();
            return m_pInstance;
        }

        virtual ~MemoryLog() {}

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 写入一条日志. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <param name="msg"> [in,out] If non-null, the
        ///                    message. </param>
        ///-------------------------------------------------------------------------------------------------
        void addLog(std::string msg)
        {
            if (_memLogQueue.size_approx() > maxLen) { //如果它实在是太长了，大概占用了20M
                std::string first;
                _memLogQueue.try_dequeue(first);
            }
            _memLogQueue.enqueue(msg);
        }

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 获取一条日志. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <param name="msg"> [in,out] The message. </param>
        ///
        /// <returns>
        /// True if it succeeds, false if it fails.
        /// </returns>
        ///-------------------------------------------------------------------------------------------------
        bool getLog(std::string& msg)
        {
            return _memLogQueue.try_dequeue(msg);
        }

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 当前内存中缓存的日志条数. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        size_t count()
        {
            return _memLogQueue.size_approx();
        }

        ///-------------------------------------------------------------------------------------------------
        /// <summary>
        /// 清空内存日志.
        /// </summary>
        ///
        /// <remarks> Dx, 2018/11/15. </remarks>
        ///-------------------------------------------------------------------------------------------------
        void clear()
        {
            std::string first;
            while (_memLogQueue.try_dequeue(first)) {
            }
        }

        /// <summary> Queue of memory logs. </summary>
        moodycamel::ConcurrentQueue<std::string > _memLogQueue;

        /// <summary> 队列的最大长度. </summary>
        size_t maxLen = 1024 * 1024 / 5;

    private:

        /// <summary> The MemoryLog object. </summary>
        static MemoryLog* m_pInstance;
    };

}
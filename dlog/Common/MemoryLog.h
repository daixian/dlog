#pragma once

#include "../../concurrentqueue/concurrentqueue.h"
#include "../../concurrentqueue/blockingconcurrentqueue.h"

namespace dxlib
{
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
        MemoryLog();

        static MemoryLog* GetInst();

        virtual ~MemoryLog();

        ///-------------------------------------------------------------------------------------------------
        /// <summary> Adds a log. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <param name="msg"> [in,out] If non-null, the
        ///                    message. </param>
        ///-------------------------------------------------------------------------------------------------
        void addLog(char* msg);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> Gets a log. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <param name="msg"> [in,out] The message. </param>
        ///
        /// <returns>
        /// True if it succeeds, false if it fails.
        /// </returns>
        ///-------------------------------------------------------------------------------------------------
        bool getLog(std::string &msg);

        ///-------------------------------------------------------------------------------------------------
        /// <summary> 当前内存中缓存的日志条数. </summary>
        ///
        /// <remarks> Dx, 2018/5/11. </remarks>
        ///
        /// <returns> An int. </returns>
        ///-------------------------------------------------------------------------------------------------
        size_t count();

        
        /// <summary> Queue of memory logs. </summary>
        moodycamel::ConcurrentQueue<std::string > _memLogQueue;

    private:

        /// <summary> The MemoryLog object. </summary>
        static MemoryLog* m_pInstance;

   

    };

}
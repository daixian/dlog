#pragma once

#include "../../concurrentqueue/concurrentqueue.h"
#include "../../concurrentqueue/blockingconcurrentqueue.h"

namespace dxlib
{
    ///-------------------------------------------------------------------------------------------------
    /// <summary>
    /// �ڴ��л������־����һ��������C#���ֵĳ�����ѯ��ȡ��Ȼ����UI��������ʾ.
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
        /// <summary> ��ǰ�ڴ��л������־����. </summary>
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
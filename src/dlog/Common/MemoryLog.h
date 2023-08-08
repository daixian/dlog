#pragma once

#include "../concurrentqueue/concurrentqueue.h"
#include "../concurrentqueue/blockingconcurrentqueue.h"

#include <string>

namespace dlog {

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
    void addLog(const std::string& msg)
    {
        if (_memLogQueue.size_approx() > maxLen) { // 如果它实在是太长了
            std::string first;
            _memLogQueue.try_dequeue(first); // 移出最前的
        }
        _memLogQueue.enqueue(msg);
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 写入一条日志. </summary>
    ///
    /// <remarks> Dx, 2018/5/11. </remarks>
    ///
    /// <param name="msg"> [in,out] If non-null, the
    ///                    message. </param>
    ///-------------------------------------------------------------------------------------------------
    void addLog(const std::wstring& msg)
    {
        if (_memLogQueueW.size_approx() > maxLen) { // 如果它实在是太长了
            std::wstring first;
            _memLogQueueW.try_dequeue(first); // 移出最前的
        }
        _memLogQueueW.enqueue(msg);
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 获取一条日志. </summary>
    ///
    /// <remarks> Dx, 2018/5/11. </remarks>
    ///
    /// <param name="msg"> [out] 一条日志. </param>
    ///
    /// <returns> 如果成功提取返回true,提取失败返回false. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool getLog(std::string& msg)
    {
        return _memLogQueue.try_dequeue(msg);
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 获取一条日志. </summary>
    ///
    /// <remarks> Dx, 2018/5/11. </remarks>
    ///
    /// <param name="msg"> [out] 一条日志. </param>
    ///
    /// <returns> 如果成功提取返回true,提取失败返回false. </returns>
    ///-------------------------------------------------------------------------------------------------
    bool getLog(std::wstring& msg)
    {
        return _memLogQueueW.try_dequeue(msg);
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
    /// <summary> 当前内存中缓存的日志条数. </summary>
    ///
    /// <remarks> Dx, 2018/5/11. </remarks>
    ///
    /// <returns> An int. </returns>
    ///-------------------------------------------------------------------------------------------------
    size_t countW()
    {
        return _memLogQueueW.size_approx();
    }

    ///-------------------------------------------------------------------------------------------------
    /// <summary> 清空内存日志. </summary>
    ///
    /// <remarks> Dx, 2018/11/15. </remarks>
    ///-------------------------------------------------------------------------------------------------
    void clear()
    {
        std::string first;
        while (_memLogQueue.size_approx() != 0) {
            _memLogQueue.try_dequeue(first);
        }

        std::wstring firstw;
        while (_memLogQueueW.size_approx() != 0) {
            _memLogQueueW.try_dequeue(firstw);
        }
    }

    /// <summary> Queue of memory logs. </summary>
    moodycamel::ConcurrentQueue<std::string> _memLogQueue;

    /// <summary> Queue of memory logs. </summary>
    moodycamel::ConcurrentQueue<std::wstring> _memLogQueueW;

    /// <summary> 队列的最大长度. </summary>
    size_t maxLen = 1024 * 1024 / 64;

  private:
    /// <summary> The MemoryLog object. </summary>
    static MemoryLog* m_pInstance;
};

} // namespace dlog

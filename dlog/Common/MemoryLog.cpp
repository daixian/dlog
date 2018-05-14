#include "MemoryLog.h"

using namespace std;

namespace dxlib
{
    MemoryLog* MemoryLog::m_pInstance = NULL;

    MemoryLog::MemoryLog()
    {
    }

    MemoryLog * MemoryLog::GetInst()
    {
        if (m_pInstance == NULL)  //�ж��Ƿ��һ�ε���
            m_pInstance = new MemoryLog();
        return m_pInstance;
    }

    MemoryLog::~MemoryLog()
    {
    }

    void MemoryLog::addLog(char * msg)
    {
        if (_memLogQueue.size_approx() > 1024 * 1024 / 5)//�����ʵ����̫���ˣ����ռ����20M
        {
            string first;
            _memLogQueue.try_dequeue(first);
        }
        _memLogQueue.enqueue(msg);
    }

    bool MemoryLog::getLog(std::string & msg)
    {
        return _memLogQueue.try_dequeue(msg);
    }

    size_t MemoryLog::count()
    {
        return _memLogQueue.size_approx();
    }
}
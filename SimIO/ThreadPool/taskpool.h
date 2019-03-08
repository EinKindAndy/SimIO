#ifndef _TASKPOOL_H_
#define _TASKPOOL_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include "../cg"
#include <queue>
#include <mutex>

using namespace SIMIO::CG;
using namespace std;
namespace SIMIO {
namespace TP {

    template<typename VAR>
    class TaskPool {
    public:
        TaskPool() {}

        void add(VAR dp) {
            m_lock.lock();
            m_tasks.push(dp);
            m_lock.unlock();
        }

        bool pop(VAR &dp) {
            bool st = false;
            lock_guard<mutex> sf(m_lock);
            if(!m_tasks.empty()) {
                dp = m_tasks.front();
                m_tasks.pop();
                return true;
            }
            return st;
        }

    private:
        mutex m_lock;
        queue<VAR> m_tasks;
    };
    

}
}

#endif
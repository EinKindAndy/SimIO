#ifndef _CALCWORKER_H_
#define _CALCWORKER_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */


#include "../cg"
#include "taskpool.h"
#include "resultmgr.h"
#include <mutex>

using namespace SIMIO::CG;
using namespace std;
namespace SIMIO {
namespace TP {
    
    template<typename VAR, typename VAR2>
    class CalcWorker
    {
    public:
        CalcWorker(TaskPool<VAR> *pool, ResultMgr<VAR> *res, uint dp_size, uint obj_size, Runnable<VAR, VAR2>* model) :
            m_stop_state{ false },
            m_tasks{ pool },
            m_res{ res },
            m_dp_size{ dp_size },
            m_obj_size{ obj_size },
            m_model{ model }
        {

        }

        void set_stop_state(bool state) {
            m_stop_state = state;
        }

        void run() {
            std::mutex lock;
            bool go_on;
            while(!m_stop_state) {
                CalcResult<VAR>* result  = new CalcResult<VAR>();
                VAR dp;
                go_on = m_tasks->pop(dp);
                if(!go_on)
                {
                    return;
                }

                for(auto i = 0; i < m_dp_size; i++)
                {
                    result->dp[i] = dp.at(i);
                }

                lock_guard<mutex> sf(lock);
                result->state = m_model(result->dp, result->obj);
                m_res->add(result);
            }
        }
    
    private:
        bool m_stop_state;
        TaskPool<VAR>* m_tasks;
        ResultMgr<VAR>* m_res;
        uint m_dp_size;
        uint m_obj_size;
        Runnable<VAR, VAR2>* m_model;
    };


}
}

#endif
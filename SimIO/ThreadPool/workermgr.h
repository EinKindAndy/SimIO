#ifndef _WORKERMGR_H_
#define _WORKERMGR_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include "../cg"
#include <queue>
#include <mutex>
#include "calcworker.h"

using namespace SIMIO::CG;
using namespace std;
namespace SIMIO {
namespace TP {

    template<typename VAR, typename VAR2>
    class WorkerMgr {
    public:
        WorkerMgr(uint nthreads, uint dp_size, uint obj_size, Runnable<VAR, VAR2>* runner) :
            m_to_run{ true },
            m_nthreads{ nthreads },
            m_tasks{},
            m_res{},
            m_dp_size{ dp_size },
            m_obj_size{ obj_size },
            m_model{ runner }
        {
            for(auto i = 0; i < m_nthreads; i++) {
                CalcWorker<VAR, VAR2> worker(&m_tasks, &m_res, m_dp_size, m_obj_size, m_model);
                m_workers.push_back(worker);
            }
        }
        void add_task(VAR dp) {
            m_tasks.add(dp);
        }

        void get_res(vector<CalcResult<VAR>*> &res) {
            res = m_res.get_res();
        }
        
        //void stop(); // kill the running process!
        void run() {
            if(m_to_run) {
                m_runners.clear();
                for(auto i = 0; i < m_nthreads; i++) {
                    m_runners.push_back(thread(&CalcWorker<VAR, VAR2>::run, ref(m_workers[i])));
                }
                for(auto i = 0; i < m_nthreads; i++) {
                    m_runners.at(i).join();
                }
            }
        }

    private:
        bool m_to_run;
        TaskPool<VAR> m_tasks;
        ResultMgr<VAR> m_res;
        uint m_nthreads;
        uint m_dp_size;
        uint m_obj_size;
        Runnable<VAR, VAR2>* m_model;
        vector<CalcResult<VAR>> m_workers;
        vector<thread> m_runners;
    };

}
}

#endif
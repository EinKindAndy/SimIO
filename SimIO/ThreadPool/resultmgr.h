#ifndef _RESULTMGR_H_
#define _RESULTMGR_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */


#include "../cg"

using namespace SIMIO::CG;
using namespace std;
namespace SIMIO {
namespace TP {
    enum CalcStatus { Normal, CalcFailed, Timeout, ConstraintInvalid };

    template<typename IN, typename OUT>
    class Runnable {
    public:
        Runnable() {}
        virtual CalcStatus run(const IN& in, OUT& out) { // const reference
        //virtual CalcStatus run(IN in, OUT& out) { // copy waste
            CalcStatus st = Normal;
            return st;
        }
    };

    template<typename VAR>
    struct CalcResult {
        CalcStatus state;
        VAR dp;
        double obj;
        CalcResult() {}
    };

    template<typename VAR>
    class ResultMgr {
    public:
        ResultMgr() {}
        ~ResultMgr() {}

        void add(CalcResult<VAR>* res) {
            m_lock.lock();
            m_res.push_back(res);
            m_lock.unlock();
        }

        vector<CalcResult<VAR>*> get_res() { return m_res; }

    private:
        mutex m_lock;
        vector<CalcResult<VAR>*> m_res;
    };


}
}

#endif
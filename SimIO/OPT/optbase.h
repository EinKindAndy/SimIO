#ifndef _OPT_H_
#define _OPT_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */


#include "../cg"
#include "../tp"
#include <chrono>
#include <random>
#include <ctime>

using namespace SIMIO::CG;
using namespace SIMIO::TP;
using namespace std;
namespace SIMIO {
namespace OPT {

    template<typename VAR>
    class OPTBase {
    public:
        OPTBase(
            uint dp_size,
            uint pop_size,
            uint calc_num,
            VAR dp_upper,
            VAR dp_lower,
            double obj_upper,
            double obj_lower,
            Runnable<VAR, double>* model) :
            m_dp_size{ dp_size },
            m_pop_size{ pop_size },
            m_calc_num{ calc_num },
            m_dp_upper{ dp_upper },
            m_dp_lower{ dp_lower },
            m_obj_upper{ obj_upper },
            m_obj_lower{ obj_lower },
            m_model{ model }
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            srand(seed);
        }

        virtual void run_opt() {}

        VAR get_opt_dp() {
            return m_best_dp;
        }

        double get_opt_obj() {
            return m_best_obj;
        }
    
    protected:
        uint m_dp_size;
        uint m_pop_size;
        uint m_calc_num;
        VAR m_dp_upper;
        VAR m_dp_lower;
        double m_obj_upper;
        double m_obj_lower;
        Runnable<VAR, double>* m_model;
        VAR m_best_dp;
        double m_best_obj;

    };
}
}

#endif
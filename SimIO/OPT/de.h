#ifndef _DE_H_
#define _DE_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include "../cg"
#include "unordered_set"
#include "optbase.h"

using namespace SIMIO::CG;
using namespace std;
namespace SIMIO {
namespace OPT {

    template<typename Scalar> struct lessThanOp {
        EIGEN_EMPTY_STRUCT_CTOR(lessThanOp)
        typedef Scalar result_type;
        Scalar operator()(const Scalar& a, const Scalar& b) const {
            if (a < b) {
                return 1;
            } else {
                return 0;
            }
        }
    };

    class DE : public OPTBase<VVAL> {
    public:
        DE(
            uint dp_size,
            uint pop_size,
            uint calc_num,
            VVAL dp_upper,
            VVAL dp_lower,
            double obj_upper,
            double obj_lower,
            Runnable<VVAL, double>* model,
            double cr = 0.9,
            double f = 1.2,
            uint times = 1):
            OPTBase<VVAL>(
                dp_size,
                pop_size,
                calc_num,
                dp_upper,
                dp_lower,
                obj_upper,
                obj_lower,
                model),
            m_real_distribution{ 0.0, 1.0 },
            m_cr{ cr },
            m_f{ f },
            m_times{ times },
            m_population(pop_size)
        {

        }

        ~DE() {
            for(auto elm : m_population) { delete elm; }
            m_population.clear();
        }

        void run_opt() {
            init();
            for(auto i = 0; i < m_calc_num; i++) {
                update();
                cout << "\nOpt step " << i+1 << " current loss is " << m_population[0]->obj << endl;
            }
            this->m_best_dp = m_population[0]->dp;
            this->m_best_obj = m_population[0]->obj;
        }

        void set_params(double cr, double f) {
            m_cr = cr;
            m_f = f;
        }
        
    private:
        

        vector<uint> select_random_elm(uint id, uint len, uint num) {
            // note len > num > 1
            uniform_int_distribution<uint> int_distribution(0, len - 1);
            unordered_set<uint> set;
            while (set.size() < num) {
                uint n = int_distribution(m_rand_gen);
                if( n != id ) {
                    set.insert(n);
                }
            }
            vector<uint> res;
            for(auto elm : set) { res.push_back(elm); }
            return res;
        }

        VVAL get_rand_dp() {
            VVAL tmp = (VVAL::Random(this->m_dp_size) + VVAL::Ones(this->m_dp_size)) * 0.5;
            VVAL tmp_ = tmp.array() * (this->m_dp_upper - this->m_dp_lower).array();
            return (this->m_dp_lower) + tmp_;
        }

        void init() {
            for(auto i = 0; i < this->m_pop_size; i++) {
                ElmPoint* mp = new ElmPoint;
                mp->dp = get_rand_dp();
                this->m_model->run(mp->dp, mp->obj);
                m_population[i] = mp;
            }
        }

        void update() {
            uint count = 0;
            uint len = m_population.size();
            for(auto i = 0; i < len; i++) {
                vector<uint> selected_elms = select_random_elm(i, len, 3);
                VVAL coef = (VVAL::Random(this->m_dp_size) + VVAL::Ones(this->m_dp_size)) * 0.5;
                /*/
                coef = coef.binaryExpr(VVAL::Ones(this->m_dp_size)*m_cr, lessThanOp<double>());
                VVAL x = m_population[i]->dp;
                VVAL a = m_population[selected_elms[0]]->dp;
                VVAL b = m_population[selected_elms[1]]->dp;
                VVAL c = m_population[selected_elms[2]]->dp;
                VVAL tmp = coef.array() * (a - x + m_f * (b - c)).array();
                ElmPoint* mp = new ElmPoint;
                mp->dp = x + tmp;
                this->m_model->run(mp->dp, mp->obj);
                m_population.push_back(mp);
                /*/
                ElmPoint* mp = new ElmPoint;
                mp->dp = m_population[selected_elms[0]]->dp + m_f * (m_population[selected_elms[1]]->dp - m_population[selected_elms[2]]->dp);
                this->m_model->run(mp->dp, mp->obj);
                m_population.push_back(mp);
            }
            sort(m_population.begin(), m_population.end(), lessElmPoint);
            for(auto i = len; i < m_population.size(); i++) {
                delete m_population[i];
            }
            m_population.erase(m_population.begin() + len, m_population.end());
        }

    private:
        struct ElmPoint {
            VVAL dp;
            double obj;
        };
        static bool lessElmPoint(ElmPoint* a, ElmPoint* b) { return (a->obj < b->obj); }
        uint m_times;
        double m_cr;
        double m_f;
        default_random_engine m_rand_gen;
        uniform_real_distribution<double> m_real_distribution;
        vector<ElmPoint*> m_population;
    };
}
}

#endif
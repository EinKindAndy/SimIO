#ifndef _PSO_H_
#define _PSO_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */


#include <algorithm>
#include "../cg"
#include "optbase.h"

using namespace SIMIO::CG;
using namespace std;
namespace SIMIO {
namespace OPT {

    class PSO : public OPTBase<VVAL> {
    public:
        PSO(
            uint dp_size,
            uint pop_size,
            uint calc_num,
            VVAL dp_upper,
            VVAL dp_lower,
            double obj_upper,
            double obj_lower,
            Runnable<VVAL, double>* model):
            OPTBase<VVAL>(
                dp_size,
                pop_size,
                calc_num,
                dp_upper,
                dp_lower,
                obj_upper,
                obj_lower,
                model),
            m_distribution{ 0.0, 1.0 },
            m_particles(pop_size)
        {
            m_c1 = 0.8;
            m_c2 = 0.2;
        }

        ~PSO() {
            for(auto elm : m_particles) { delete elm; }
            m_particles.clear();
        }

        void run_opt() {
            init();
            update();
        }

        void set_params(double c1, double c2) {
            m_c1 = c1;
            m_c2 = c2;
        }
        
    private:
        VVAL get_rand_dp() {
            VVAL tmp = (VVAL::Random(this->m_dp_size) + VVAL::Ones(this->m_dp_size)) * 0.5;
            VVAL tmp2 = this->m_dp_upper - this->m_dp_lower;
            VVAL tmp3 = tmp.array() * tmp2.array();
            return tmp + tmp3;
        }
        void init() {
            cout << "init..." << endl;
            for(auto i = 0; i < this->m_pop_size; i++) {
                MovingPoint* mp = new MovingPoint;
                mp->cur_pos = get_rand_dp();
                mp->cur_vel = VVAL::Random(this->m_dp_size).array() * (this->m_dp_upper - this->m_dp_lower).array();
                mp->local_best_pos = get_rand_dp();
                m_particles[i] = mp;
                //cout << "cur_pos\n" << mp->cur_pos << endl;
                //cout << "pre_pos\n" << mp->local_best_pos << endl;
            }
        }

        void update() {
            double global_obj = 1e30;
            for(auto k = 0; k < this->m_calc_num; k++) {
                
                for(auto i = 0; i < this->m_pop_size; i++) {
                    double cobj, pobj;
                    this->m_model->run(m_particles[i]->cur_pos, cobj);
                    this->m_model->run(m_particles[i]->local_best_pos, pobj);
                    if(cobj < pobj) {
                        m_particles[i]->local_best_pos = m_particles[i]->cur_pos;
                        pobj = cobj;
                    }
                    if(pobj < global_obj) {
                        m_best_position = m_particles[i]->local_best_pos;
                        global_obj = cobj;
                    }
                }

                cout << "\nOpt step " << k+1 << " current loss is " << global_obj << endl;

                for(auto i = 0; i < this->m_pop_size; i++) {
                    m_particles[i]->cur_vel = (m_particles[i]->cur_vel + 
                    m_c1 * m_distribution(m_rand_gen) * (m_particles[i]->local_best_pos - m_particles[i]->cur_pos) +
                    m_c2 * m_distribution(m_rand_gen) * (m_best_position - m_particles[i]->cur_pos));
                    m_particles[i]->cur_pos = m_particles[i]->cur_pos + m_particles[i]->cur_vel;
                    //cout << "cur_pos\n" << m_particles[i]->cur_pos << endl;
                    //cout << "pre_pos\n" << m_particles[i]->local_best_pos << endl;
                }
            }
            this->m_best_dp = m_best_position;
            this->m_best_obj = global_obj;
        }

    private:
        struct MovingPoint {
            VVAL cur_pos;
            VVAL cur_vel;
            VVAL local_best_pos;
        };
        double m_c1;
        double m_c2;
        default_random_engine m_rand_gen;
        uniform_real_distribution<double> m_distribution;
        VVAL m_best_position;
        vector<MovingPoint*> m_particles;
    };
}
}

#endif
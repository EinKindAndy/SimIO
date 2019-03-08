//#pragma once

#ifndef _UTILITIES_H_
#define _UTILITIES_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */


#include <string>
#include <functional>
#include <algorithm>
#include "ann"
#include "opt"
#include "cg"

using namespace SIMIO::NN;
using namespace SIMIO::OPT;
using namespace std;
namespace SIMIO {
namespace TOOLS {

    
    class ANNTrainer : public Runnable<VVAL, double> { //note: expand it to more kinds of agent later!
    public:
        ANNTrainer(
            uint input_size,
            uint output_size,
            vector<uint> layers,
            vector<ActivationFun> activations = {},
            string name = "") :
            m_net{ input_size, output_size, name }
        {
            if(activations.size() == 0) {
                for(auto num : layers) {
                    m_net.add_layer(num);
                }
                m_net.add_output();
            }
            if(activations.size() == layers.size()) {
                uint i = 0;
                for(auto num : layers) {
                    m_net.add_layer(num, activations[i]);
                    i += 1;
                }
                m_net.add_output();
            }
            m_st = m_net.status();
        }

        ~ANNTrainer() { }

        void set_sample(
            const vector<VVAL>& xs,
            const vector<VVAL>& fxs) 
        {
            m_sample_inputs = xs;
            m_sample_outputs = fxs;
        }

        uint design_param_num() const { return m_net.coefficient_num(); }

        double l2_loss() {
            double res = 0;
            uint num = m_sample_inputs.size();
            for(auto i = 0; i < num; i++) {
                m_net.feed_input(m_sample_inputs[i]);
                VVAL y_ = m_net.evaluate();
                m_st = m_net.status();
                if(m_st.state) {
                    res += (y_ - m_sample_outputs[i]).squaredNorm();
                }
            }
            return res / ((double)num);
        }

        void update_coefficients(VVAL coef) {
            m_net.update_coefficients(coef);
            m_st = m_net.status();
        }

        Status save_net(const string& path) { return m_net.save_net(path); }
        Status status() { return m_st; }
        ANN net() { return m_net; }

        VVAL feed_test(VVAL xs) {
            m_net.feed_input(xs);
            return m_net.evaluate();
        }

        CalcStatus run(VVAL in, double& out) {
            CalcStatus st = Normal;
            update_coefficients(in);
            out = l2_loss();
            //cout << "Runner: current input\n" << in << endl << "l2 loss is " << out << endl;
            return st;
        }

    private:
        Status m_st;
        ANN m_net;
        vector<VVAL> m_sample_inputs;
        vector<VVAL> m_sample_outputs;
    };

}
}

#endif
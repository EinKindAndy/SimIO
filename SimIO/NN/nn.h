
#ifndef _NN_H_
#define _NN_H_

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
namespace NN {

    enum ActivationFun { ReLU, TANH, SIGMOID };

    class ANN {
    public:
        ANN(
        uint input_elm_size, 
        uint output_elm_size,
        string name="") :
        m_has_done{ false },
        m_st{},
        m_layer_num{ 0 },
        m_coefficient_num{ 0 },
        m_input_elm_size{ input_elm_size },
        m_output_elm_size{ output_elm_size },
        m_name{ name }
        {
            m_each_layer_elm_nums.push_back(input_elm_size);
            m_layer_num += 1;
            m_session = GraphCreate<VVAL>();
            m_layers.push_back(VFUN::Placeholder(m_st, m_session, input_elm_size, "x"));
        }

        Status status() { return m_st; }
        uint input_elm_size() const { return m_input_elm_size; }
        uint output_elm_size() const { return m_output_elm_size; }
        const string& name() const { return m_name; }
        uint coefficient_num() const { return m_coefficient_num; }

        void add_layer(uint elm_num, ActivationFun fun = ReLU) {
            if(m_has_done) { return; }
            uint current_content_size = elm_num * m_each_layer_elm_nums.at(m_layer_num - 1);
            m_coefficient_num += current_content_size;
            m_coefficients.push_back(VFUN::Constant(m_st, m_session, VVAL::Random(current_content_size)));
            m_each_layer_elm_nums.push_back(elm_num);
            if(fun == ReLU) {
                m_layers.push_back(VFUN::Relu(m_st, m_session, 
                VFUN::MatMultiplyVec(m_st, m_session, m_coefficients.at(m_layer_num - 1), m_layers.at(m_layer_num - 1))));  
            }
            m_layer_num += 1;
        }

        void add_output() {
            if(m_has_done) { return; }
            uint current_content_size = m_output_elm_size * m_each_layer_elm_nums.at(m_layer_num - 1);
            m_coefficient_num += current_content_size;
            m_coefficients.push_back(VFUN::Constant(m_st, m_session, VVAL::Random(current_content_size)));
            m_layers.push_back(VFUN::MatMultiplyVec(m_st, m_session, m_coefficients.at(m_layer_num - 1), m_layers.at(m_layer_num - 1)));
            m_layer_num += 1;
            m_has_done = true;
        }

        void feed_input(VVAL data) {
            m_layers[0]->feed(data);
            m_layers[0]->update();
        }

        VVAL evaluate() {
            if(!m_st.state) { return VVAL::Zero(1); }
            if(!m_has_done) { m_st.state = false; return VVAL::Zero(1); }
            //for(auto it : m_coefficients) {
            //    cout << it->result() << endl;
            //}
            //cout << "size: " << m_coefficient_num << endl;
            m_layers[m_layer_num - 1]->calc();
            return m_layers[m_layer_num - 1]->result();
        }

        void update_coefficients(const VVAL& coef) {
        //void update_coefficients(VVAL coef) {
            if(coef.size() != m_coefficient_num) { m_st.state = false; return; }
            ullint it = 0;
            for(auto i = 0; i < m_coefficients.size(); i++)
            {
                uint cur_size = m_coefficients[i]->elm_size();
                for(auto k = 0; k < cur_size; k++) {
                    m_coefficients[i]->set_elem_val(k, coef[it + k]);
                }
                it += cur_size;
            }
        }

        Status save_net(const string& path) { return m_session->save(path); }
    private:
        bool m_has_done;
        uint m_layer_num;
        uint m_coefficient_num;
        Status m_st;
        uint m_input_elm_size;
        uint m_output_elm_size;
        string m_name;
        shared_ptr<Graph<VVAL> > m_session;
        vector<uint> m_each_layer_elm_nums;
        vector<SFun<VVAL> > m_layers;
        vector<SFun<VVAL> > m_coefficients;
    };




}
}

#endif
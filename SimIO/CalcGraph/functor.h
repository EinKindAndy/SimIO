//#pragma once

#ifndef _FUNCTOR_H_
#define _FUNCTOR_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include <vector>
#include <unordered_map>
#include "check.h"
#include <mutex>
#include "ref_count.h"

using namespace std;
namespace SIMIO {
namespace CG {

	enum NodeType { PlaceHolder, CalcElem };
	enum NodeState { PureData, NoData, Normal_Uncalc, Normal_Calc, Failed };

	template<typename VAR>
	class Functor 
	{
	public:
		Functor(
			shared_ptr<Graph<VAR> > graph_root,
			int id, 
			uint elm_size,
			string name, 
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			m_graph_root{ graph_root },
			m_id{ id }, m_name{ name }, m_desc{ desc },
			m_input_count{ 0 }, m_output_count{ 0 },
			m_elm_size{elm_size},
			m_node_type{type},
			m_node_state{state}
		{
		}

		void set_id(int i) { m_id = i; }
		int id() const { return m_id; }
		uint input_num() const { return m_islots.size(); }
		const string& name() const { return m_name; }
		const string& desc() const { return m_desc; }
		void feed(VAR data) { m_res = data; };
		void set_elem_val(uint i, double v) { m_res[i] = v; }
		VAR result() { return m_res; }
		uint elm_size() const { return m_elm_size; }
		NodeType node_type() const { return m_node_type; }
		NodeState node_state() const { return m_node_state; }
		Status calc() {
			Status st;
			if (m_node_state != PureData and m_node_state != Normal_Calc) {
				if (m_node_state == NoData) {
					st.state = false;
					st.description = "Placeholder has not been fed! From: " + this->m_name;
					return st;
				}
				if (m_node_state == Normal_Uncalc) {
					st = adaptive_call();
					if (st.state) { m_node_state = Normal_Calc; }
				}
			}
			return st;
		}

		Status update() {
			Status st;
			if (m_node_state != PureData) {
				m_node_state = Normal_Uncalc;
				for(auto i = 1; i <= m_oslots.size(); i++) {
					int id = m_oslots[i];
					m_graph_root->elem_update(id);
				}
			}
			return st;
		}

		void input(int para_id) {
			m_input_count += 1;
			m_islots[m_input_count] = para_id;
		}

		void output(int para_id) {
			m_output_count += 1;
			m_oslots[m_output_count] = para_id;
		}

		bool is_from_node(int para_id) {
			bool res = false;
			if(m_node_type == PlaceHolder || m_node_state == PureData) { 
				if(m_id == para_id) { return true; }
				if(m_id != para_id) { return false; }
			}
			for(auto i = 1; i <= m_islots.size(); i++) {
				int id = m_islots[i];
				res = res || m_graph_root->elem_is_from_node(id, para_id);
				if(res) { return res; }
			}
			return res;
		}

	private:
		virtual Status adaptive_call() {
			Status st;
			return st;
		}

		

	protected:
		shared_ptr<Graph<VAR> > m_graph_root;
		VAR m_res;
		int m_id;
		string m_name;
		string m_desc;
		uint m_input_count;
		uint m_output_count;
		unordered_map<uint, int> m_islots;
		unordered_map<uint, int> m_oslots;
		uint m_elm_size;
		NodeType m_node_type;
		NodeState m_node_state;
	};


}
}

#endif
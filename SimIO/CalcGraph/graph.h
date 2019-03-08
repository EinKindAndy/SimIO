//#pragma once

#ifndef _GRAPH_H_
#define _GRAPH_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include "functor.h"

using namespace std;
namespace SIMIO {
namespace CG {
	
	template<typename VAR>
	class Graph {
	public:
		Graph(
			string name = "",
			string desc = ""):
			m_elm_count{ 0 },
			m_name{name},
			m_desc{desc}
		{}

		~Graph() {}

		void insert(int id, SFun<VAR> elm_ptr) {
			if (m_register.count(id) == 0) {
				m_register[id] = elm_ptr;
				m_elm_count += 1;
			}
			else {
				m_register[id] = elm_ptr;
			}
		}

		Status elem_calc(int id) { return m_register[id]->calc(); }
		VAR elem_fetch(int id) { return m_register[id]->result(); }
		void elem_update(int id) { m_register[id]->update(); }
		bool elem_is_from_node(int id, int at) { return m_register[id]->is_from_node(at); }
		int fetch_new_id() const { return m_elm_count; }
		int size() const { return m_register.size(); }
		
		//void remove(int id) {}
		void clear() {}

		Status save(const string& path) {
			Status st;
			return st;
		}

		Status import(const string& path) {
			Status st;
			return st;
		}
	
	private:
		uint m_elm_count;
		string m_name;
		string m_desc;
		unordered_map<int, SFun<VAR> > m_register;
	};

	template<typename VAR>
	shared_ptr<Graph<VAR> > GraphCreate(
		string name = "",
		string desc = "") {

		shared_ptr<Graph<VAR> > graph(new Graph<VAR>(name, desc));
		return graph;
	}


}
}

#endif
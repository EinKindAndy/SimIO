//#pragma once

#ifndef _BASE_FUNS_H_
#define _BASE_FUNS_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include "graph.h"
#include <initializer_list>
#include <iostream>

using namespace std;
namespace SIMIO {
namespace CG {
	template<typename VAR>
	class PAdd : public Functor<VAR> {
	public:
		PAdd(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") : 
			Functor<VAR>(
				graph_root,
				id, elm_size, name, 
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 2) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int lval_id = this->m_islots[1]; //This kind of implementation is not so good!
			int rval_id = this->m_islots[2]; //To refine this part;
			st = this->m_graph_root->elem_calc(lval_id);
			if (!st.state) { return st; }
			st = this->m_graph_root->elem_calc(rval_id);
			if (!st.state) { return st; }
			VVAL tmp = this->m_graph_root->elem_fetch(lval_id) + this->m_graph_root->elem_fetch(rval_id);
			this->feed(tmp);
			return st;
		}
	};

	template<typename VAR>
	class PSub : public Functor<VAR> {
	public:
		PSub(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 2) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int lval_id = this->m_islots[1];
			int rval_id = this->m_islots[2];
			st = this->m_graph_root->elem_calc(lval_id);
			if (!st.state) { return st; }
			st = this->m_graph_root->elem_calc(rval_id);
			if (!st.state) { return st; }
			VVAL tmp = this->m_graph_root->elem_fetch(lval_id) - this->m_graph_root->elem_fetch(rval_id);
			this->feed(tmp);
			return st;
		}
	};

	template<typename VAR>
	class PMultiply : public Functor<VAR> {
	public:
		PMultiply(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 2) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int lval_id = this->m_islots[1];
			int rval_id = this->m_islots[2];
			st = this->m_graph_root->elem_calc(lval_id);
			if (!st.state) { return st; }
			st = this->m_graph_root->elem_calc(rval_id);
			if (!st.state) { return st; }
			VVAL tmp = (this->m_graph_root->elem_fetch(lval_id)).array() * (this->m_graph_root->elem_fetch(rval_id)).array();
			this->feed(tmp);
			return st;
		}
	};

	template<typename VAR>
	class PMatMultiplyVec : public Functor<VAR> {
	public:
		PMatMultiplyVec(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 2) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int lval_id = this->m_islots[1];
			int rval_id = this->m_islots[2];
			st = this->m_graph_root->elem_calc(lval_id);
			if (!st.state) { return st; }
			st = this->m_graph_root->elem_calc(rval_id);
			if (!st.state) { return st; }
			VVAL mat = this->m_graph_root->elem_fetch(lval_id);
			VVAL vec = this->m_graph_root->elem_fetch(rval_id);
			uint row_num = mat.size() / vec.size();
			uint col_num = vec.size();
			MVAL tmp(row_num, col_num);
			for(auto i = 0; i < mat.size(); i++) {
				uint ix = i / col_num;
				uint iy = i % col_num;
				tmp(ix, iy) = mat[i];
			}
			this->feed(tmp * vec);
			return st;
		}
	};


	template<typename VAR>
	class PPow : public Functor<VAR> {
	public:
		PPow(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 2) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int lval_id = this->m_islots[1];
			int rval_id = this->m_islots[2];
			st = this->m_graph_root->elem_calc(lval_id);
			if (!st.state) { return st; }
			st = this->m_graph_root->elem_calc(rval_id);
			if (!st.state) { return st; }
			VVAL tmp = (this->m_graph_root->elem_fetch(lval_id)).array().pow((this->m_graph_root->elem_fetch(rval_id)).array());
			this->feed(tmp);
			return st;
		}
	};


	template<typename VAR>
	class PAbs : public Functor<VAR> {
	public:
		PAbs(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 1) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int val_id = this->m_islots[1];
			st = this->m_graph_root->elem_calc(val_id);
			if (!st.state) { return st; }
			this->feed((this->m_graph_root->elem_fetch(val_id)).array().abs());
			return st;
		}
	};

	template<typename VAR>
	class PExp : public Functor<VAR> {
	public:
		PExp(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 1) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int val_id = this->m_islots[1];
			st = this->m_graph_root->elem_calc(val_id);
			if (!st.state) { return st; }
			this->feed((this->m_graph_root->elem_fetch(val_id)).array().exp());
			return st;
		}
	};

	template<typename VAR>
	class PRelu : public Functor<VAR> {
	public:
		PRelu(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 1) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int val_id = this->m_islots[1];
			st = this->m_graph_root->elem_calc(val_id);
			if (!st.state) { return st; }
			this->feed((this->m_graph_root->elem_fetch(val_id)).array().max(VVAL::Zero(this->elm_size()).array()));
			return st;
		}
	};

	template<typename VAR>
	class PTanh : public Functor<VAR> {
	public:
		PTanh(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 1) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int val_id = this->m_islots[1];
			st = this->m_graph_root->elem_calc(val_id);
			if (!st.state) { return st; }
			this->feed((this->m_graph_root->elem_fetch(val_id)).array().tanh());
			return st;
		}
	};

	template<typename VAR>
	class PSigmoid : public Functor<VAR> {
	public:
		PSigmoid(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 1) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int val_id = this->m_islots[1];
			st = this->m_graph_root->elem_calc(val_id);
			if (!st.state) { return st; }
			this->feed(1.0 / (1.0 + (-(this->m_graph_root->elem_fetch(val_id))).array().exp()));
			return st;
		}
	};

	template<typename VAR>
	class PSquare : public Functor<VAR> {
	public:
		PSquare(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 1) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int val_id = this->m_islots[1];
			st = this->m_graph_root->elem_calc(val_id);
			if (!st.state) { return st; }
			this->feed((this->m_graph_root->elem_fetch(val_id)).array().square());
			return st;
		}
	};

	template<typename VAR>
	class PCube : public Functor<VAR> {
	public:
		PCube(
			shared_ptr<Graph<VAR> > graph_root,
			int id,
			uint elm_size,
			string name,
			NodeType type = CalcElem,
			NodeState state = Normal_Uncalc,
			string desc = "") :
			Functor<VAR>(
				graph_root,
				id, elm_size, name,
				type, state, desc) {}
	private:
		Status adaptive_call() {
			Status st;
			uint size = this->m_islots.size();
			if (size != 1) { st.state = false; st.description = "Error that input size does not match; From: " + this->m_name; return st; }
			int val_id = this->m_islots[1];
			st = this->m_graph_root->elem_calc(val_id);
			if (!st.state) { return st; }
			this->feed((this->m_graph_root->elem_fetch(val_id)).array().cube());
			return st;
		}
	};


	namespace VFUN {
		void Feed(Status &st, SFun<VVAL> elm, VVAL val) {
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, elm, "The placeholder is null!")) { return; }
			if (elm->node_type() != PlaceHolder) {
				st.state = false;
				st.description = elm->name() + " is not a placeholder!";
				return;
			}
			elm->feed(val);
			elm->update();
		}

		/*
		void Feed(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> elm, SFun<VVAL> val) {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, elm, "The placeholder is null!")) { return; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, val, "The fed data is null!")) { return; }
			if (!CHECK_ELEM_SIZE_MATCH(st, elm->elm_size(), val->elm_size(), "in feding data to placeholder")) { return; }
			if (elm->node_type() != PlaceHolder) {
				st.state = false;
				st.description = elm->name() + " is not a placeholder!";
				return;
			}
			if (val->node_state() != PureData) {
				st.state = false;
				st.description = val->name() + " is not pure data!";
				return;
			}
			elm->feed(val->result());
			elm->update();
		}
		//*/

		VVAL Grad(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> x, SFun<VVAL> y) {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return VVAL::Zero(1); }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, x, "The x element of Add is null!")) { return VVAL::Zero(1); }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, y, "The y element of Add is null!")) { return VVAL::Zero(1); }
			if (!CHECK_ELEM_SIZE_MATCH(st, x->elm_size(), y->elm_size(), "in calculating the y grad x")) { return VVAL::Zero(1); }
			if (x->node_type() != PlaceHolder) {
				st.state = false;
				st.description = x->name() + " is not a placeholder!";
				return VVAL::Zero(1);
			}
			if(!y->is_from_node(x->id())) {
				st.state = false;
				st.description = y->name() + " is not from node " + x->name();
				return VVAL::Zero(1);
			}
			uint nx = x->elm_size();
			VVAL y_grad_x = VVAL::Zero(nx);
			if(x->node_state() == PureData) { return y_grad_x; }
			VVAL o_x_data(x->result());
			VVAL o_y_data(y->result());
			for(auto i = 0; i < nx; i++) {
				VVAL tmp_x(o_x_data);
				tmp_x[i] = tmp_x[i] + D_EPS;
				x->feed(tmp_x);
				x->update();
				st = y->calc();
				if(!st.state){
					return VVAL::Zero(1);
				}
				VVAL n_y_data = y->result();
				y_grad_x[i] = (n_y_data[i] - o_y_data[i]) / D_EPS;
			}
			x->feed(o_x_data);
			x->update();
			st = y->calc();
			return y_grad_x;
		}

		SFun<VVAL> Constant(Status &st, shared_ptr<Graph<VVAL> > session, double value, uint size = 1, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }

			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new Functor<VVAL>(session, new_id, size, name, CalcElem, PureData, "Constant value"));
			node->feed(VVAL::Constant(size, value));
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Constant(Status &st, shared_ptr<Graph<VVAL> > session, initializer_list<double> values, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }

			int new_id = session->fetch_new_id();
			uint size = values.size();
			uint count = 0;
			VVAL tmp(size);
			for (auto v : values) { tmp[count] = v; count += 1; }
			SFun<VVAL> node(new Functor<VVAL>(session, new_id, size, name, CalcElem, PureData, "Constant value"));
			node->feed(tmp);
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Constant(Status &st, shared_ptr<Graph<VVAL> > session, VVAL values, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }

			int new_id = session->fetch_new_id();
			uint size = values.size();
			SFun<VVAL> node(new Functor<VVAL>(session, new_id, size, name, CalcElem, PureData, "Constant value"));
			node->feed(values);
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Placeholder(Status &st, shared_ptr<Graph<VVAL> > session, uint size = 1, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }

			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new Functor<VVAL>(session, new_id, size, name, PlaceHolder, NoData, "Placeholder"));
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Add(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> lval, SFun<VVAL> rval, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, lval, "The left element of Add is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, rval, "The right element of Add is null!")) { return nullptr; }
			if (!CHECK_ELEM_SIZE_MATCH(st, lval->elm_size(), rval->elm_size(), "in building of Add functor")) { return nullptr; }

			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PAdd<VVAL>(session, new_id, lval->elm_size(), name, CalcElem, Normal_Uncalc, "Add two parameters"));
			node->input(lval->id());
			node->input(rval->id());
			lval->output(node->id());
			rval->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Sub(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> lval, SFun<VVAL> rval, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, lval, "The left element of Add is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, rval, "The right element of Add is null!")) { return nullptr; }
			if (!CHECK_ELEM_SIZE_MATCH(st, lval->elm_size(), rval->elm_size(), "in building of Sub functor")) { return nullptr; }

			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PSub<VVAL>(session, new_id, lval->elm_size(), name, CalcElem, Normal_Uncalc, "Sub two parameters"));
			node->input(lval->id());
			node->input(rval->id());
			lval->output(node->id());
			rval->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Multiply(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> lval, SFun<VVAL> rval, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, lval, "The left element of Add is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, rval, "The right element of Add is null!")) { return nullptr; }
			if (!CHECK_ELEM_SIZE_MATCH(st, lval->elm_size(), rval->elm_size(), "in building of Multiply functor")) { return nullptr; }

			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PMultiply<VVAL>(session, new_id, lval->elm_size(), name, CalcElem, Normal_Uncalc, "Multiply two parameters"));
			node->input(lval->id());
			node->input(rval->id());
			lval->output(node->id());
			rval->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> MatMultiplyVec(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> lval, SFun<VVAL> rval, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, lval, "The left element of Add is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, rval, "The right element of Add is null!")) { return nullptr; }

			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PMatMultiplyVec<VVAL>(session, new_id, lval->elm_size() / rval->elm_size(), name, CalcElem, Normal_Uncalc, "Matrix lval Multiply Vector rval"));
			node->input(lval->id());
			node->input(rval->id());
			lval->output(node->id());
			rval->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Pow(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> lval, SFun<VVAL> rval, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, lval, "The left element of Pow is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, rval, "The right element of Pow is null!")) { return nullptr; }
			if (!CHECK_ELEM_SIZE_MATCH(st, lval->elm_size(), rval->elm_size(), "in building of Pow functor")) { return nullptr; }

			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PPow<VVAL>(session, new_id, lval->elm_size(), name, CalcElem, Normal_Uncalc, "Pow(a,b); Pow(a_i, b_i) for vector"));
			node->input(lval->id());
			node->input(rval->id());
			lval->output(node->id());
			rval->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Exp(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> val, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, val, "The element of Exp is null!")) { return nullptr; }
			
			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PExp<VVAL>(session, new_id, val->elm_size(), name, CalcElem, Normal_Uncalc, "Exp(x)"));
			node->input(val->id());
			val->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Square(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> val, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, val, "The element of Square is null!")) { return nullptr; }
			
			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PSquare<VVAL>(session, new_id, val->elm_size(), name, CalcElem, Normal_Uncalc, "Square(x)"));
			node->input(val->id());
			val->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Cube(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> val, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, val, "The element of Cube is null!")) { return nullptr; }
			
			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PCube<VVAL>(session, new_id, val->elm_size(), name, CalcElem, Normal_Uncalc, "Cube(x)"));
			node->input(val->id());
			val->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Abs(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> val, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, val, "The element of Abs is null!")) { return nullptr; }
			
			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PAbs<VVAL>(session, new_id, val->elm_size(), name, CalcElem, Normal_Uncalc, "Abs(x)"));
			node->input(val->id());
			val->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Relu(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> val, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, val, "The element of Relu is null!")) { return nullptr; }
			
			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PRelu<VVAL>(session, new_id, val->elm_size(), name, CalcElem, Normal_Uncalc, "Relu(x)"));
			node->input(val->id());
			val->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Tanh(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> val, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, val, "The element of Tanh is null!")) { return nullptr; }
			
			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PTanh<VVAL>(session, new_id, val->elm_size(), name, CalcElem, Normal_Uncalc, "Tanh(x)"));
			node->input(val->id());
			val->output(node->id());
			session->insert(new_id, node);
			return node;
		}

		SFun<VVAL> Sigmoid(Status &st, shared_ptr<Graph<VVAL> > session, SFun<VVAL> val, string name = "") {
			if (!CHECK_NULL_PTR<shared_ptr<Graph<VVAL> > >(st, session, "The root of this graph is null!")) { return nullptr; }
			if (!CHECK_NULL_PTR<SFun<VVAL> >(st, val, "The element of Sigmoid is null!")) { return nullptr; }
			
			int new_id = session->fetch_new_id();
			SFun<VVAL> node(new PSigmoid<VVAL>(session, new_id, val->elm_size(), name, CalcElem, Normal_Uncalc, "Sigmoid(x)"));
			node->input(val->id());
			val->output(node->id());
			session->insert(new_id, node);
			return node;
		}

	}


}
}


#endif
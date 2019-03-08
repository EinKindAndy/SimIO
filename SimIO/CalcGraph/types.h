//#pragma once
#ifndef _TYPES_H_
#define _TYPES_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include "Eigen/Dense"
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "ref_count.h"

using namespace Eigen;
using namespace std;
namespace SIMIO {
namespace CG {
	typedef ArrayXd AVAL;
	typedef VectorXd VVAL;
	typedef MatrixXd MVAL;
	typedef unsigned int uint;
	typedef unsigned long long ullint;

	template<typename VAR>
	class Graph;

	template<typename VAR>
	class Functor;

	template<typename VAR>
	using SFun = shared_ptr<Functor<VAR> >;

	template<typename VAR>
	using RNode_POOL = unordered_map<int, RefCount<SFun<VAR> > >;

	const double D_EPS = 1e-6;
	
}
}

#endif
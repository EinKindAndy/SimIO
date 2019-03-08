//#pragma once

#ifndef _CHECK_H_
#define _CHECK_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include <vector>
#include <unordered_map>
#include "status.h"
#include "types.h"

using namespace std;
namespace SIMIO {
namespace CG {
	
	template<typename PTR>
	inline bool CHECK_NULL_PTR(Status &st, PTR ptr, string info = "") {
		if (ptr == nullptr) {
			st.state = false;
			st.description = info;
			return false;
		}
		return true;
	}

	inline bool CHECK_ELEM_SIZE_MATCH(Status &st, uint l, uint r, string info = "") {
		if (l != r) {
			st.state = false;
			st.description = "\nThe shapes of input elements do not match with each other; at " + info;
			return false;
		}
		return true;
	}


}
}


#endif
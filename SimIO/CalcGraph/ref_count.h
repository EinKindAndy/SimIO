//#pragma once

#ifndef _REF_COUNT_H_
#define _REF_COUNT_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

namespace SIMIO {
namespace CG {
	template<typename T>
	struct RefCount {
		T& obj;
		int count;
		RefCount(T& obj) { obj = obj; count = 0; }
		void add_one() { count += 1; }
		void minus_one() {
			if (count >= 0) {
				count -= 1;
			}
		}
		bool is_void() { return (count < 0) ? true : false; }
	};

}
}


#endif
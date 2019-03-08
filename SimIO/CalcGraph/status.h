//#pragma once
#ifndef _STATUS_H_
#define _STATUS_H_

/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#include <string>
#include <functional>

using namespace std;
namespace SIMIO {
namespace CG {
	
	struct Status {
		bool state;
		string description;
		Status(bool st, string desc) { state = st; description = move(desc); }
		Status() { state = true; description = "Fine"; }
	};

}
}

#endif
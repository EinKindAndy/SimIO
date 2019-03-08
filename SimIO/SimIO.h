
/*
 * MPL2 License
 *
 * Copyright (c) 2019 Liang Zhang
 * 
 */

#ifndef _SIMIO_H_
#define _SIMIO_H_

#include <iostream>

using namespace std;
namespace  SIMIO {
	class World {
	public:
		void Build() { 
			std::cout << "It's a new world" << std::endl;
		}
	};
}

#endif
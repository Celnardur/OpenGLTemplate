#ifndef UTILS_H
#define UTILS_H
#include <iostream>

#define myAssert(a, b) if (!(a)) { std::cout << "Error:" << __FILE__ << ":" << \
	__LINE__ << std::endl << (b) << std::endl; while(1); }

#endif
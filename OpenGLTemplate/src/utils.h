#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <cassert>

#define myAssert(a, b) if (!(a)) { std::cout << "Error:" << __FILE__ << ":" << \
	__LINE__ << std::endl << (b) << std::endl; while(1); }

#define alert(a, b)\
	if (!(a)) {\
		std::cerr << "Error:" << __FILE__ << ":" << __LINE__ << std::endl; \
		std::cerr << (b) << std::endl;\
	}\
	assert(a);

#define stop(a, b)\
	if (!(a)) {\
		std::cerr << "Error:" << __FILE__ << ":" << __LINE__ << std::endl; \
		std::cerr << (b) << std::endl;\
	}\
	while(true);

#endif
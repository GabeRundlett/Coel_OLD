#pragma once

#ifdef COEL_DEBUG
#define CL_LOG(x) std::cout << x
#define CL_ASSERT(a, b) \
	if (!a)             \
	{                   \
		std::cout << b; \
		return;         \
	}
#define CL_BREAK(a, b)  \
	if (!a)             \
	{                   \
		std::cout << b; \
		return;         \
	}
#else
#define CL_LOG(x)
#define CL_ASSERT(a, b) a
#define CL_BREAK(a, b)
#endif
#pragma once

#include <iostream>
#include <cstring>

#define SNAKE_DEBUG
#define SKIP_LOG
#define SKIP_WRN
// #define SKIP_ERR

#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#if defined(SNAKE_DEBUG) && !defined(SKIP_LOG)
#define SNAKE_LOG(x) std::cout << "[LOG] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl
#else
#define SNAKE_LOG(x) do {} while(0)
#endif

#if defined(SNAKE_DEBUG) && !defined(SKIP_WRN)
#define SNAKE_WRN(x) std::cerr << "[WRN] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl
#else
#define SNAKE_WRN(x) do {} while(0)
#endif

#if defined(SNAKE_DEBUG) && !defined(SKIP_ERR)
#define SNAKE_ERR(x) std::cerr << "[ERR] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl
#else
#define SNAKE_ERR(x) do {} while(0)
#endif

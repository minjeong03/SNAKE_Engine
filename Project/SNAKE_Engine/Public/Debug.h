#pragma once
/**
 * @brief
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-11
 */
#include <iostream>
#ifdef _DEBUG_
#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define SNAKE_LOG(x) std::cout << "[LOG] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl
#define SNAKE_ERR(x) std::cerr << "[ERR] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl
#define SNAKE_WRN(x) std::cerr << "[WRN] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl
#else
#define SNAKE_LOG(x) do {} while (0)
#define SNAKE_ERR(x) do {} while (0)
#define SNAKE_WRN(x) do {} while (0)
#endif

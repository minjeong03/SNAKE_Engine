#pragma once

/**
 * @file Debug.h
 * @brief Simple logging macros for debug output.
 *
 * Defines macros for logging messages to the console.
 * These macros automatically include the filename and line number,
 * and are only enabled in debug builds (`_DEBUG` is defined).
 *
 * In release mode, all macros are compiled out with no overhead.
 *
 * Usage:
 * @code
 * SNAKE_LOG("Initialized OK");
 * SNAKE_ERR("Failed to load texture");
 * SNAKE_WRN("FPS dropped below 30");
 * @endcode
 *
 * Example output:
 * [LOG] MyFile.cpp:42 - Initialized OK
 */

#include <iostream>

#ifdef _DEBUG_

 /// Returns the current source filename (without full path).
#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

/**
 * @brief Prints a log message to std::cout in debug mode.
 * Format: [LOG] filename:line - message
 */
#define SNAKE_LOG(x) std::cout << "[LOG] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl

 /**
  * @brief Prints an error message to std::cerr in debug mode.
  * Format: [ERR] filename:line - message
  */
#define SNAKE_ERR(x) std::cerr << "[ERR] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl

  /**
   * @brief Prints a warning message to std::cerr in debug mode.
   * Format: [WRN] filename:line - message
   */
#define SNAKE_WRN(x) std::cerr << "[WRN] " << FILENAME << ":" << __LINE__ << " - " << x << std::endl

#else

 // In release mode, these do nothing.
#define SNAKE_LOG(x) do {} while (0)
#define SNAKE_ERR(x) do {} while (0)
#define SNAKE_WRN(x) do {} while (0)

#endif

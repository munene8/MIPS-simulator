#ifndef INCLUDE_DEFINE_HPP
#define INCLUDE_DEFINE_HPP

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "evensgn_string.hpp"
#include "global_class.hpp"
#include "utility.hpp"

typedef unsigned char byte;

const int maxMemoryByte = 4 * 1024 * 1024; // 4MB
const int registerNum = 32 + 2;

/*#define DEBUG
#define DEBUG_REGISTER_NAME
#define DEBUG_READ_TEXT
#define DEBUG_ENTRY_SPLIT
#define DEBUG_TEXT_SPLIT
#define DEBUG_ENTRY_INDEX
#define DEBUG_ENTRY_TYPE
#define DEBUG_ENTRY_STATIC_DATA
#define DEBUG_ENTRY_TO_INSTRUCTION
#define DEBUG_STORE_INSTRUCTION
#define DEBUG_PIPELINE*/
//#define DEBUG_PAUSE

#endif // INCLUDE_DEFINE_HPP

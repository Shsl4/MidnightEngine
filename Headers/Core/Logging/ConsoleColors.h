#pragma once

#include <string>

class ConsoleColors {

public:

	inline static std::string getLightRed() { return "\x1b[31;1m"; }
	inline static std::string getDarkRed() { return "\x1b[31m"; }
	inline static std::string getLightGreen() { return "\x1b[32;1m"; }
	inline static std::string getDarkGreen() { return "\x1b[32m"; }
	inline static std::string getWhite() { return "\x1b[97m"; }
	inline static std::string getGrey() { return "\x1b[37m"; }
	inline static std::string getDarkGrey() { return "\x1b[90m"; }
	inline static std::string getLightYellow() { return "\x1b[93;1m"; }
	inline static std::string getDarkYellow() { return "\x1b[33m"; }
	inline static std::string getReset() { return "\x1b[0m"; }

};
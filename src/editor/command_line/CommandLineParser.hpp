#pragma once

#include <curses.h>

#include "ParsedCommand.hpp"

namespace Tedit {
class CommandLineParser {
public:
	CommandLineParser(std::string_view source);

	std::expected<ParsedCommand, std::string> parse();

private:
	struct RawCommand {
		std::string cmd {};
		std::vector<std::string> args {};
	};

private:
	RawCommand create_raw();

	void skip_whitespace();
	void skip_non_whitespace();

	char current();
	char advance();
	bool is_at_end();
	std::string substr();

private:
private:
	std::string m_source {};
	size_t m_current {};
	size_t m_start {};
};

}

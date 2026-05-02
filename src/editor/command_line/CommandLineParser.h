#pragma once

#include "ParsedCommand.h"

namespace Tedit {
class CommandLineParser {
public:
	static ParsedCommand parse(std::string_view input) {
		input = trim(input);

		if (input.empty()) {
			return std::unexpected("empty command");
		}

		auto cmd = to_lower(input);

		if (cmd == "write" || cmd == "w") {
			return CommandType::Write;
		}

		if (cmd == "quit" || cmd == "q" || cmd == "exit") {
			return CommandType::Quit;
		}

		return std::unexpected("unknown command: " + std::string(input));
	}

private:
	static std::string_view trim(std::string_view s) {
		auto is_space = [](unsigned char c) { return std::isspace(c); };

		while (!s.empty() && is_space(s.front()))
			s.remove_prefix(1);
		while (!s.empty() && is_space(s.back()))
			s.remove_suffix(1);

		return s;
	}

	static std::string to_lower(std::string_view s) {
		std::string out(s);
		std::transform(out.begin(), out.end(), out.begin(),
		    [](unsigned char c) { return std::tolower(c); });
		return out;
	}
};

}

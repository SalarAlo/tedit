#include <stdexcept>

#include "CommandLineParser.hpp"

namespace Tedit {

CommandLineParser::CommandLineParser(std::string_view source)
    : m_source(source) {
}

std::expected<ParsedCommand, std::string> CommandLineParser::parse() {
	auto input_raw { create_raw() };

	if (input_raw.cmd.empty()) {
		return std::unexpected("empty command");
	}

	if (input_raw.cmd == "w" || input_raw.cmd == "write") {
		return ParsedCommand {
			.type = CommandType::Write,
			.args = {}
		};
	}

	if (input_raw.cmd == "q" || input_raw.cmd == "quit") {
		return ParsedCommand {
			.type = CommandType::Quit,
		};
	}

	if (input_raw.cmd == "exp" || input_raw.cmd == "explorer") {
		return ParsedCommand {
			.type = CommandType::OpenExplorer,
		};
	}

	if (input_raw.cmd == "o" || input_raw.cmd == "open") {
		if (input_raw.args.size() < 1) {
			return std::unexpected("open command requires one argument.");
		}

		return ParsedCommand {
			.type = CommandType::Open,
			.args = input_raw.args
		};
	}

	return std::unexpected("unknown command: " + std::string(input_raw.cmd));
}

CommandLineParser::RawCommand CommandLineParser::create_raw() {
	RawCommand cmd {};

	bool first { true };

	while (!is_at_end()) {
		skip_whitespace();
		m_start = m_current;
		skip_non_whitespace();

		if (first)
			cmd.cmd = substr();
		else
			cmd.args.push_back(substr());

		first = false;
	}

	return cmd;
}

void CommandLineParser::skip_whitespace() {
	while (std::isspace(current()) && !is_at_end()) {
		advance();
	}
}

void CommandLineParser::skip_non_whitespace() {
	while (!std::isspace(current()) && !is_at_end()) {
		advance();
	}
}

char CommandLineParser::current() {
	if (is_at_end())
		return 0;

	return m_source[m_current];
}

char CommandLineParser::advance() {
	auto c { current() };
	m_current++;
	return c;
}

bool CommandLineParser::is_at_end() {
	return m_current >= m_source.length();
}

std::string CommandLineParser::substr() {
	return m_source.substr(m_start, m_current - m_start);
}

}

#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

namespace Tedit {

class MacroController {
public:
	void start_recording(char key);
	void stop_recording();

	void record_key(int key);
	bool is_recording() const;

	std::optional<std::vector<int>> get_register(char key);

private:
	std::unordered_map<char, std::vector<int>> m_macros {};
	std::vector<int> m_active_combination {};

	char m_active_key {};
};

}

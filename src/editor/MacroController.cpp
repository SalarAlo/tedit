#include "MacroController.hpp"

namespace Tedit {

void MacroController::start_recording(char key) {
	m_active_key = key;
}

void MacroController::stop_recording() {
	m_macros[m_active_key] = std::move(m_active_combination);
	m_active_key = 0;
	m_active_combination.clear();
}

std::optional<std::vector<int>> MacroController::get_register(char key) {
	auto it { m_macros.find(key) };

	if (it == m_macros.end())
		return std::nullopt;

	return it->second;
}

void MacroController::record_key(int key) {
	if (!m_active_key)
		return;

	m_active_combination.push_back(key);
}

bool MacroController::is_recording() const { return m_active_key != 0; }

}

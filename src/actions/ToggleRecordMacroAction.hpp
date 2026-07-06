#pragma once

#include "Action.hpp"
#include "Editor.hpp"

namespace Tedit {
class ToggleRecordMacroAction : public IAction {
public:
	ToggleRecordMacroAction(int key);

	void execute(Editor& e) override;

	constexpr bool is_macro_register();

private:
	int m_key {};
};

}

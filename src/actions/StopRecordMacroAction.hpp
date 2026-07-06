#pragma once

#include "Action.hpp"

namespace Tedit {

class StopRecordMacroAction final : public IAction {
public:
	void execute(Editor& e) override;
	bool records_macro_key() const override;
};

}

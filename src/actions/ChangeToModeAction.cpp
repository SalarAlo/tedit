#include "ChangeToModeAction.hpp"

namespace Tedit {

ChangeToModeAction::ChangeToModeAction(std::unique_ptr<IMode> mode)
    : m_mode(std::move(mode)) {
}

void ChangeToModeAction::execute(Editor& e) {
	e.change_mode(std::move(m_mode));
	m_mode.release();
}

}

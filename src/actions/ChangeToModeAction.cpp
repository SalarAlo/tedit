#include "ChangeToModeAction.h"

namespace Tedit {

ChangeToModeAction::ChangeToModeAction(std::unique_ptr<Mode> mode)
    : m_mode(std::move(mode)) {
}

void ChangeToModeAction::execute(Editor& e) {
	e.change_mode(std::move(m_mode));
	m_mode.release();
}

}

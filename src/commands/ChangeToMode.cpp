#include "ChangeToMode.h"

namespace Tedit {

ChangeToMode::ChangeToMode(std::unique_ptr<Mode> mode)
    : m_mode(std::move(mode)) {
}

void ChangeToMode::execute(Editor& e) {
	e.change_mode(std::move(m_mode));
	m_mode.release();
}

}

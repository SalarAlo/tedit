#include <cctype>

#include <memory>
#include <utility>

#include "NormalMode.hpp"

#include "ChangeToCommandMode.hpp"
#include "ChangeToModeAction.hpp"
#include "CommandLine.hpp"
#include "DeleteCharAction.hpp"
#include "EditOperator.hpp"
#include "InsertMode.hpp"
#include "MoveEndLineAction.hpp"
#include "MoveLineStartAction.hpp"
#include "SelectAction.hpp"
#include "SequenceAction.hpp"

#include "actions/MoveDownAction.hpp"
#include "actions/MoveLeftAction.hpp"
#include "actions/MoveRightAction.hpp"
#include "actions/MoveUpAction.hpp"
#include "actions/NewlineAction.hpp"

namespace Tedit {

Bimap<char, EditOperator> NormalMode::m_operators {
	[] {
	        Bimap<char, EditOperator> bimap;
	        bimap.insert('d', EditOperator::Delete);
	        bimap.insert('y', EditOperator::Yank);
	        bimap.insert('c', EditOperator::Change);
	        return bimap;
	}()
};

std::string NormalMode::get_name() {
	return "normal";
}

std::unique_ptr<IAction> NormalMode::map_action(int key) {
	if (is_count_key(key)) {
		m_count = m_count * 10 + (key - '0');
		return nullptr;
	}

	if (is_operator_key(key)) {
		m_pending_edit_operator = *m_operators.get(static_cast<char>(key));
		return nullptr;
	}

	if (m_pending_edit_operator != EditOperator::None) {
		return nullptr;
	}

	auto motion_action { map_motion(key) };

	if (!motion_action) {
		m_count = 0;
		m_pending_edit_operator = EditOperator::None;
		return nullptr;
	}

	size_t count { std::max<size_t>(1, m_count) };
	std::vector<std::unique_ptr<IAction>> actions;

	for (size_t i {}; i < count; ++i)
		actions.push_back(map_motion(key));

	auto sequence { std::make_unique<SequenceAction>(std::move(actions)) };
	m_count = 0;
	m_pending_edit_operator = EditOperator::None;

	return sequence;
}

std::unique_ptr<IAction> NormalMode::map_motion(int key) {
	switch (key) {
	case 'i':
		return std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>());

	case 'a':
		return sequence(
		    std::make_unique<MoveRightAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case 'o':
		return sequence(
		    std::make_unique<MoveEndLineAction>(),
		    std::make_unique<NewlineAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case 'O':
		return sequence(
		    std::make_unique<MoveLineStartAction>(),
		    std::make_unique<NewlineAction>(),
		    std::make_unique<MoveUpAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case 'h':
		return std::make_unique<MoveLeftAction>();
	case 'j':
		return std::make_unique<MoveDownAction>();
	case 'k':
		return std::make_unique<MoveUpAction>();
	case 'l':
		return std::make_unique<MoveRightAction>();

	case '0':
		return std::make_unique<MoveLineStartAction>();
	case '_':
		return std::make_unique<MoveLineStartAction>();
	case '$':
		return std::make_unique<MoveEndLineAction>();

	case KEY_LEFT:
		return std::make_unique<MoveLeftAction>();
	case KEY_RIGHT:
		return std::make_unique<MoveRightAction>();
	case KEY_UP:
		return std::make_unique<MoveUpAction>();
	case KEY_DOWN:
		return std::make_unique<MoveDownAction>();
	case KEY_ENTER:
	case '\n':
	case '\r':
		return std::make_unique<SelectAction>();

	case 'x':
		return std::make_unique<DeleteCharAction>();

	case 's':
		return sequence(
		    std::make_unique<DeleteCharAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case CommandLine::COMMAND_LINE_KEY:
		return std::make_unique<ChangeToCommandMode>();

	case KEY_BACKSPACE:
	case 127:
	case 8:
		return std::make_unique<MoveLeftAction>();

	default: {
		return nullptr;
	}
	}
}

std::string NormalMode::get_mode_details() {
	auto mode_details { std::to_string(m_count) };

	if (mode_details == "0")
		mode_details.clear();

	if (m_pending_edit_operator != EditOperator::None)
		mode_details += m_operators.get(m_pending_edit_operator);

	return mode_details;
}

bool NormalMode::is_count_key(int key) const {
	return key >= '1' && key <= '9';
}

bool NormalMode::is_operator_key(int key) const {
	return m_operators.contains(static_cast<char>(key));
}

}

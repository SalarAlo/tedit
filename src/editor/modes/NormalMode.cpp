#include <memory>
#include <utility>

#include "NormalMode.hpp"

#include "ChangeToCommandMode.hpp"
#include "ChangeToModeAction.hpp"
#include "CommandLine.hpp"
#include "DeleteCharAction.hpp"
#include "DeleteMotionAction.hpp"
#include "EditOperator.hpp"
#include "InsertMode.hpp"
#include "MoveMotionAction.hpp"
#include "SelectAction.hpp"
#include "SequenceAction.hpp"

#include "actions/NewlineAction.hpp"

#include "motions/DownMotion.h"
#include "motions/LeftMotion.h"
#include "motions/LineEndMotion.h"
#include "motions/LineStartMotion.h"
#include "motions/RightMotion.h"
#include "motions/UpMotion.h"

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
		auto action { get_operator_action(key) };
		reset_state();
		return action;
	}

	auto action { map_key_action(key) };

	if (!action) {
		m_count = 0;
		m_pending_edit_operator = EditOperator::None;
		return nullptr;
	}

	size_t count { std::max<size_t>(1, m_count) };
	std::vector<std::unique_ptr<IAction>> actions;

	for (size_t i {}; i < count; ++i)
		actions.push_back(map_key_action(key));

	auto sequence { std::make_unique<SequenceAction>(std::move(actions)) };
	m_count = 0;
	m_pending_edit_operator = EditOperator::None;

	return sequence;
}

std::unique_ptr<IAction> NormalMode::map_key_action(int key) {
	if (auto motion { map_motion(key) })
		return std::make_unique<MoveMotionAction>(std::move(motion));

	switch (key) {
	case 'i':
		return std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>());

	case 'a':
		return sequence(
		    std::make_unique<MoveMotionAction>(std::make_unique<RightMotion>()),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case 'o':
		return sequence(
		    std::make_unique<MoveMotionAction>(std::make_unique<LineEndMotion>()),
		    std::make_unique<NewlineAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case 'O':
		return sequence(
		    std::make_unique<MoveMotionAction>(std::make_unique<LineStartMotion>()),
		    std::make_unique<NewlineAction>(),
		    std::make_unique<MoveMotionAction>(std::make_unique<UpMotion>()),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

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

	default:
		return nullptr;
	}
}

std::unique_ptr<IMotion> NormalMode::map_motion(int key) {
	switch (key) {
	case 'h':
		return std::make_unique<LeftMotion>();
	case 'j':
		return std::make_unique<DownMotion>();
	case 'k':
		return std::make_unique<UpMotion>();
	case 'l':
		return std::make_unique<RightMotion>();

	case '0':
	case '_':
		return std::make_unique<LineStartMotion>();
	case '$':
		return std::make_unique<LineEndMotion>();

	case KEY_LEFT:
		return std::make_unique<LeftMotion>();
	case KEY_RIGHT:
		return std::make_unique<RightMotion>();
	case KEY_UP:
		return std::make_unique<UpMotion>();
	case KEY_DOWN:
		return std::make_unique<DownMotion>();

	case KEY_BACKSPACE:
	case 127:
	case 8:
		return std::make_unique<LeftMotion>();

	default:
		return nullptr;
	}
}
std::unique_ptr<IAction> NormalMode::map_operator_action(
    EditOperator op,
    std::unique_ptr<IMotion> motion) {
	switch (op) {

	case EditOperator::Delete:
		return std::make_unique<DeleteMotionAction>(std::move(motion));
	case EditOperator::Change:
		return sequence(
		    std::make_unique<DeleteMotionAction>(std::move(motion)),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));
	default:
		return nullptr;
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

void NormalMode::reset_state() {

	m_count = 0;
	m_pending_edit_operator = EditOperator::None;
}

std::unique_ptr<IAction> NormalMode::get_operator_action(int key) {
	auto motion { map_motion(key) };

	if (!motion)
		return nullptr;

	auto operator_action { map_operator_action(m_pending_edit_operator, std::move(motion)) };

	if (!operator_action)
		return nullptr;

	return operator_action;
}

}

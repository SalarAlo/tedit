#include <memory>
#include <utility>

#include "NormalMode.hpp"

#include "ChangeToModeAction.hpp"
#include "ChangeToPromptMode.hpp"
#include "CommandPrompt.hpp"
#include "DeleteCharAction.hpp"
#include "DeleteMotionAction.hpp"
#include "EditOperator.hpp"
#include "InsertMode.hpp"
#include "LineMotion.hpp"
#include "MoveMotionAction.hpp"
#include "NextCursorSearchAction.hpp"
#include "SearchPrompt.hpp"
#include "SelectAction.hpp"
#include "SequenceAction.hpp"
#include "SwitchTabAction.hpp"
#include "UndoAction.hpp"
#include "WordMotion.hpp"

#include "actions/NewlineAction.hpp"

#include "motions/DownMotion.hpp"
#include "motions/LeftMotion.hpp"
#include "motions/LineEndMotion.hpp"
#include "motions/LineStartMotion.hpp"
#include "motions/RightMotion.hpp"
#include "motions/UpMotion.hpp"

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

std::string NormalMode::get_name() const {
	return "normal";
}

std::unique_ptr<IAction> NormalMode::map_action(int key) {
	if (is_count_key(key)) {
		m_count = m_count * 10 + (key - '0');
		return nullptr;
	}

	if (m_pending_edit_operator != EditOperator::None) {
		auto action { get_operator_action(key) };
		reset_state();
		return action;
	}

	if (is_operator_key(key)) {
		m_pending_edit_operator = *m_operators.get(static_cast<char>(key));
		return nullptr;
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

	case 'u':
		return std::make_unique<UndoAction>();

	case 'a':
		return sequence(
		    std::make_unique<MoveMotionAction>(std::make_unique<RightMotion>()),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));
	case 'n':
		return std::make_unique<NextCursorSearchAction>();

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

	case '\t':
		return std::make_unique<SwitchTabAction>();

	case 'x':
		return std::make_unique<DeleteCharAction>();

	case 's':
		return sequence(
		    std::make_unique<DeleteCharAction>(),
		    std::make_unique<ChangeToModeAction>(std::make_unique<InsertMode>()));

	case ':':
		return std::make_unique<ChangeToPromptMode>(std::make_unique<CommandPrompt>());
	case '?':
		return std::make_unique<ChangeToPromptMode>(std::make_unique<SearchPrompt>());

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

	case 'w':
		return std::make_unique<WordMotion>();
	case 'W':
		return std::make_unique<WordMotion>(Detail::WordKind::WORD);
	case 'e':
		return std::make_unique<WordEndMotion>();
	case 'E':
		return std::make_unique<WordEndMotion>(Detail::WordKind::WORD);
	case 'b':
		return std::make_unique<BackWordMotion>();
	case 'B':
		return std::make_unique<BackWordMotion>(Detail::WordKind::WORD);

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
std::unique_ptr<IAction> NormalMode::map_operator_action(EditOperator op, std::unique_ptr<IMotion> motion) {
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

std::string NormalMode::get_mode_details() const {
	auto mode_details { std::to_string(m_count) };

	if (mode_details == "0")
		mode_details.clear();

	if (m_pending_edit_operator != EditOperator::None)
		mode_details += *m_operators.get(m_pending_edit_operator);

	return mode_details;
}

bool NormalMode::is_count_key(int key) const {
	return (m_count != 0 && key >= '0' && key <= '9') || (key >= '1' && key <= '9');
}

bool NormalMode::is_operator_key(int key) const {
	return m_operators.contains(static_cast<char>(key));
}

void NormalMode::reset_state() {
	m_count = 0;
	m_pending_edit_operator = EditOperator::None;
}

std::unique_ptr<IAction> NormalMode::get_operator_action(int key) {
	if (is_operator_key(key)) {
		auto pressed_operator { *m_operators.get(static_cast<char>(key)) };

		if (pressed_operator == m_pending_edit_operator) {

			return map_operator_action(m_pending_edit_operator, std::make_unique<LineMotion>());
		}
	}

	auto motion { map_motion(key) };
	if (!motion)
		return nullptr;

	auto operator_action { map_operator_action(m_pending_edit_operator, std::move(motion)) };

	if (!operator_action)
		return nullptr;

	return operator_action;
}

}

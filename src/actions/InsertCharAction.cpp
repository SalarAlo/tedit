#include "InsertCharAction.hpp"

#include "Editor.hpp"

namespace Tedit {

InsertCharAction::InsertCharAction(char c)
    : m_char(c) { }

void InsertCharAction::execute(Editor& e) {
	e.insert_char(m_char);
}

}

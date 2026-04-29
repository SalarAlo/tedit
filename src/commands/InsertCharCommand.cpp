#include "InsertCharCommand.h"

#include "Editor.h"

namespace Tedit {

InsertCharCommand::InsertCharCommand(char c)
    : m_char(c) { }

void InsertCharCommand::execute(Editor& e) {
	e.insert_char(m_char);
}

}

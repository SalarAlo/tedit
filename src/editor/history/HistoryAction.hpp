#pragma once

#include <variant>

#include "Cursor.hpp"

namespace Tedit {

struct InsertAction {
	Cursor before {};
	Cursor after {};

	std::string text {};
};

struct DeleteAction {
	Cursor before {};
	Cursor after {};

	std::string text {};
};

using HistoryAction = std::variant<InsertAction, DeleteAction>;

}

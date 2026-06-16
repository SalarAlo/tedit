#pragma once

#include <string>
#include <string_view>

namespace Tedit {

class Editor;

class IPrompt {
public:
	virtual ~IPrompt() = default;
	virtual char activation_char() const = 0;
	virtual std::string name() const = 0;

	virtual void submit(Editor& editor, std::string_view input) = 0;
};

}

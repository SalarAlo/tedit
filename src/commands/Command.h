#pragma once

namespace Tedit {

class Editor;

class ICommand {
public:
	virtual ~ICommand() = default;
	virtual void execute(Editor&) = 0;
};

}

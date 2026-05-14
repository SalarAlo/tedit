#pragma once

namespace Tedit {

class Editor;

class IAction {
public:
	virtual ~IAction() = default;
	virtual void execute(Editor&) = 0;
	virtual bool consumes_count() const { return true; }
};

}

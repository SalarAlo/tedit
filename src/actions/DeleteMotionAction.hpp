#pragma once

#include <utility>

#include "Action.hpp"
#include "IMotion.hpp"

namespace Tedit {
class DeleteMotionAction : public IAction {
public:
	DeleteMotionAction(std::unique_ptr<IMotion> motion)
	    : m_motion(std::move(motion)) { }

	void execute(Editor& e) override {
		auto range { m_motion->resolve(e) };
		e.delete_range(range.start, range.end, range.inclusive, range.linewise);
	}

private:
	std::unique_ptr<IMotion> m_motion;
};

}

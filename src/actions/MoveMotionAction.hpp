#pragma once

#include <memory>

#include "Action.hpp"
#include "motions/IMotion.h"

namespace Tedit {

class MoveMotionAction : public IAction {
public:
	explicit MoveMotionAction(std::unique_ptr<IMotion> motion);

	void execute(Editor& editor) override;

private:
	std::unique_ptr<IMotion> m_motion;
};

}

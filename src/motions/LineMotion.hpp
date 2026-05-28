#pragma once

#include "IMotion.hpp"

namespace Tedit {
class LineMotion : public IMotion {
public:
	MotionRange resolve(Editor&) override;
};

}

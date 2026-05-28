#pragma once

#include "motions/IMotion.hpp"

namespace Tedit {

class LineStartMotion : public IMotion {
public:
	MotionRange resolve(Editor& e) override;
};

}

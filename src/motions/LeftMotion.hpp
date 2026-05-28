#pragma once

#include <algorithm>

#include "motions/IMotion.hpp"

namespace Tedit {

class LeftMotion : public IMotion {
public:
	MotionRange resolve(Editor& e) override;
};

}

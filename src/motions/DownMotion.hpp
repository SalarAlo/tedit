#pragma once

#include <algorithm>

#include "motions/IMotion.hpp"

namespace Tedit {

class DownMotion : public IMotion {
public:
	MotionRange resolve(Editor& e) override;
};

}

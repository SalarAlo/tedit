#pragma once

#include <algorithm>

#include "motions/IMotion.h"

namespace Tedit {

class RightMotion : public IMotion {
public:
	MotionRange resolve(Editor& e) override;
};

}

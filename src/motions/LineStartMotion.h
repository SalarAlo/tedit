#pragma once

#include "motions/IMotion.h"

namespace Tedit {

class LineStartMotion : public IMotion {
public:
	MotionRange resolve(Editor& e) override;
};

}

#pragma once

#include "motions/IMotion.h"

namespace Tedit {

class LineEndMotion : public IMotion {
public:
	MotionRange resolve(Editor& e) override;
};

}

#pragma once

#include "motions/IMotion.hpp"

namespace Tedit {

class LineEndMotion : public IMotion {
public:
	MotionRange resolve(Editor& e) override;
};

}

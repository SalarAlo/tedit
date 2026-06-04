#pragma once

#include "motions/IMotion.hpp"

namespace Tedit {

class DownMotion : public IMotion {
public:
	MotionRange resolve(Editor& e) override;
};

}

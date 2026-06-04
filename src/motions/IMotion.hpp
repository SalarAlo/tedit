#pragma once

#include "Cursor.hpp"
#include "Editor.hpp"

namespace Tedit {

struct MotionRange {
	Cursor start {};
	Cursor end {};

	bool inclusive {};
	bool linewise {};
};

class IMotion {
public:
	virtual ~IMotion() = default;

	virtual MotionRange resolve(Editor&) = 0;
};

}

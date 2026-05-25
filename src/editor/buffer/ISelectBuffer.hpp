#pragma once

#include "Editor.hpp"
#include "IBuffer.hpp"

namespace Tedit {
class ISelectBuffer : public IBuffer {
public:
	virtual void select(Editor& e) = 0;
};

}

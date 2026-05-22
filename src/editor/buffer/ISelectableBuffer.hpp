#pragma once

#include "Editor.hpp"
#include "IBuffer.hpp"

namespace Tedit {
class ISelectableBuffer : virtual public IBuffer {
public:
	virtual void select(Editor& editor) = 0;
};

}

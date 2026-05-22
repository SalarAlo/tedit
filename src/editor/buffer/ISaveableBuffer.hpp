#pragma once

#include "IBuffer.hpp"

namespace Tedit {
class ISaveableBuffer : virtual public IBuffer {
public:
	virtual void save() = 0;
};

}

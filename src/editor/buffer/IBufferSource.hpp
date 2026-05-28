#pragma once

#include <string>
#include <string_view>

namespace Tedit {
class IBufferSource {
public:
	virtual ~IBufferSource() = default;

	virtual void write(std::string_view data) = 0;
	virtual std::string read() = 0;
	virtual std::string get_buffer_name() = 0;
};

}

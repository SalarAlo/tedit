#pragma once

namespace Tedit {
class TextBufferSource {
public:
	virtual void write(std::string_view txt) = 0;
	virtual std::string read() = 0;
};

}

#pragma once

namespace Tedit {
class TextBufferSource {
public:
	virtual void write(std::string_view data) = 0;
	virtual std::string read() = 0;
};

}

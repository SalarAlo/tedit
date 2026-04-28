#pragma once

namespace Tedit {
class BufferSource {
public:
	virtual ~BufferSource() = default;

	virtual void write(std::string_view) = 0;
	virtual std::string read() = 0;

	virtual std::string get_display_name() const = 0;
};

}

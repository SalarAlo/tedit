#include "IBuffer.hpp"

namespace Tedit {

Cursor& IBuffer::get_cursor() { return m_cursor; }
const Cursor& IBuffer::get_cursor() const { return m_cursor; }

}

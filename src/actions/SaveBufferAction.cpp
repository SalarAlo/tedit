#include "SaveBufferAction.hpp"

namespace Tedit {

void SaveBufferAction::execute(Editor& e) { e.save_buffer(); }

}

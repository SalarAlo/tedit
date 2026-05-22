#include "SaveBufferAction.hpp"

namespace Tedit {

void SaveBufferAction::execute(Editor& e) { e.try_save_to_buffer(); }

}

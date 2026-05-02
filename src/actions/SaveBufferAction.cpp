#include "SaveBufferAction.h"

namespace Tedit {

void SaveBufferAction::execute(Editor& e) { e.save_to_buffer(); }

}

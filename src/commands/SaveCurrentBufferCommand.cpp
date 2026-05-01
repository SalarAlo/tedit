#include "SaveCurrentBufferCommand.h"

namespace Tedit {

void SaveCurrentBufferCommand::execute(Editor& e) { e.write_contents(); }

}

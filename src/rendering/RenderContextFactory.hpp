#pragma once

#include "BufferManager.h"
#include "IMode.hpp"
#include "PromptLineController.hpp"
#include "RenderContext.hpp"
#include "RenderStyle.hpp"
#include "SyntaxService.hpp"

namespace Tedit {

class RenderContextFactory {
public:
	RenderContext create(
	    const BufferManager& buffer_manager,
	    const PromptLineController& prompt_line,
	    SyntaxService& syntax_service,
	    const IMode& mode,
	    RenderStyle style,
	    int top_row) const;
};

}

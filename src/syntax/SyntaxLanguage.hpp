#pragma once

#include <tree-sitter-cpp.h>
#include <tree_sitter/api.h>

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_set>

namespace Tedit {
using TreeSitterLanguageFn = std::function<const TSLanguage*()>;

struct SyntaxLanguage {
	std::string name {};
	std::unordered_set<std::string> extensions {};
	TreeSitterLanguageFn tree_sitter_language {};
	std::filesystem::path highlights_query_path {};
};

const SyntaxLanguage CPP_LANGUAGE {
	.name = "cpp",
	.extensions = {
	    ".cpp",
	    ".cc",
	    ".cxx",
	    ".hh",
	    ".hpp",
	    ".hxx" },
	.tree_sitter_language = tree_sitter_cpp,
	.highlights_query_path = "assets/queries/cpp/highlights.scm"
};

}

#pragma once

#include <tree_sitter/api.h>

#include "syntax/HighlightSpan.hpp"
#include "syntax/SyntaxLanguage.hpp"

namespace Tedit {
class TreeSitterHighliter {
public:
	TreeSitterHighliter(const SyntaxLanguage& language);

	~TreeSitterHighliter();

	TreeSitterHighliter(const TreeSitterHighliter&) = delete;
	TreeSitterHighliter& operator=(const TreeSitterHighliter&) = delete;

	std::vector<HighlightSpan> highlight(std::string_view source_code);

private:
	static std::string read_file(const std::filesystem::path& path);

	static HighlightKind highlight_kind_from_capture(std::string_view capture_name);

	SyntaxLanguage m_language {};
	TSParser* m_parser {};
	TSQuery* m_query {};
};

}

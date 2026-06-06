#include <fstream>
#include <iterator>
#include <stdexcept>

#include "TreeSitterHighliter.hpp"

namespace Tedit {

std::vector<HighlightSpan> TreeSitterHighliter::highlight(std::string_view source_code) {
	std::vector<HighlightSpan> spans {};

	if (!m_parser || !m_query)
		return spans;

	TSTree* tree {
		ts_parser_parse_string(
		    m_parser,
		    nullptr,
		    source_code.data(),
		    static_cast<uint32_t>(source_code.size()))
	};

	if (!tree)
		return spans;

	TSNode root { ts_tree_root_node(tree) };
	TSQueryCursor* cursor { ts_query_cursor_new() };

	if (!cursor) {
		ts_tree_delete(tree);
		return spans;
	}

	ts_query_cursor_exec(cursor, m_query, root);

	TSQueryMatch match {};
	while (ts_query_cursor_next_match(cursor, &match)) {
		for (uint32_t i {}; i < match.capture_count; ++i) {
			const TSQueryCapture& capture { match.captures[i] };
			uint32_t capture_name_length {};
			const char* capture_name {
				ts_query_capture_name_for_id(
				    m_query,
				    capture.index,
				    &capture_name_length)
			};

			auto kind {
				highlight_kind_from_capture(
				    std::string_view { capture_name, capture_name_length })
			};

			if (kind == HighlightKind::Unknown)
				continue;

			TSPoint start { ts_node_start_point(capture.node) };
			TSPoint end { ts_node_end_point(capture.node) };

			if (start.row != end.row)
				continue;

			spans.push_back(HighlightSpan {
			    .kind = kind,
			    .start_col = static_cast<int>(start.column),
			    .past_end_col = static_cast<int>(end.column),
			    .row = static_cast<int>(start.row),
			});
		}
	}

	ts_query_cursor_delete(cursor);
	ts_tree_delete(tree);

	return spans;
}

TreeSitterHighliter::TreeSitterHighliter(const SyntaxLanguage& language)
    : m_language(language)
    , m_parser(ts_parser_new()) {
	ts_parser_set_language(m_parser, language.tree_sitter_language());

	std::string query_src { read_file(language.highlights_query_path) };
	uint32_t error_offset {};
	TSQueryError error_type {};

	m_query = ts_query_new(language.tree_sitter_language(), query_src.data(), query_src.size(), &error_offset, &error_type);

	if (!m_query) {
		throw std::runtime_error(
		    "failed to compile tree-sitter query at byte "
		    + std::to_string(error_offset)
		    + " with error type "
		    + std::to_string(static_cast<int>(error_type)));
	}
}

TreeSitterHighliter::~TreeSitterHighliter() {
	if (m_query)
		ts_query_delete(m_query);

	if (m_parser)
		ts_parser_delete(m_parser);
}

std::string TreeSitterHighliter::read_file(const std::filesystem::path& path) {
	std::ifstream file { path };
	if (!file.is_open() && path.is_relative()) {
		file.open(std::filesystem::current_path().parent_path() / path);
	}

	if (!file.is_open()) {
		throw std::runtime_error("failed to open tree-sitter query file: " + path.string());
	}

	return std::string {
		std::istreambuf_iterator<char> { file },
		std::istreambuf_iterator<char> {}
	};
}

HighlightKind TreeSitterHighliter::highlight_kind_from_capture(std::string_view capture_name) {
	if (capture_name == "keyword.conditional")
		return HighlightKind::KeywordConditional;
	if (capture_name == "keyword.directive")
		return HighlightKind::KeywordDirective;
	if (capture_name == "keyword.exception")
		return HighlightKind::KeywordException;
	if (capture_name == "keyword.function")
		return HighlightKind::KeywordFunction;
	if (capture_name == "keyword.modifier")
		return HighlightKind::KeywordModifier;
	if (capture_name == "keyword.operator")
		return HighlightKind::KeywordOperator;
	if (capture_name == "keyword.repeat")
		return HighlightKind::KeywordRepeat;
	if (capture_name == "keyword.return")
		return HighlightKind::KeywordReturn;
	if (capture_name == "keyword.storage")
		return HighlightKind::KeywordStorage;
	if (capture_name == "keyword")
		return HighlightKind::Keyword;
	if (capture_name == "type.builtin")
		return HighlightKind::BuiltinType;
	if (capture_name == "type")
		return HighlightKind::Type;
	if (capture_name == "function.builtin")
		return HighlightKind::BuiltinFunction;
	if (capture_name == "function.method" || capture_name == "method")
		return HighlightKind::Method;
	if (capture_name == "constructor")
		return HighlightKind::Constructor;
	if (capture_name == "destructor")
		return HighlightKind::Destructor;
	if (capture_name == "function.macro" || capture_name == "macro")
		return HighlightKind::Macro;
	if (capture_name == "function")
		return HighlightKind::Function;
	if (capture_name == "variable.parameter")
		return HighlightKind::Parameter;
	if (capture_name == "variable.member")
		return HighlightKind::Property;
	if (capture_name == "variable")
		return HighlightKind::Variable;
	if (capture_name == "variable.builtin")
		return HighlightKind::BuiltinVariable;
	if (capture_name == "constant.builtin")
		return HighlightKind::BuiltinConstant;
	if (capture_name == "constant")
		return HighlightKind::Constant;
	if (capture_name == "boolean")
		return HighlightKind::Boolean;
	if (capture_name == "module")
		return HighlightKind::Module;
	if (capture_name == "string.special")
		return HighlightKind::SpecialString;
	if (capture_name == "string")
		return HighlightKind::String;
	if (capture_name == "character")
		return HighlightKind::Character;
	if (capture_name == "comment")
		return HighlightKind::Comment;
	if (capture_name == "number.float")
		return HighlightKind::Float;
	if (capture_name == "number")
		return HighlightKind::Number;
	if (capture_name == "operator")
		return HighlightKind::Operator;
	if (capture_name == "punctuation.bracket")
		return HighlightKind::PunctuationBracket;
	if (capture_name == "punctuation.delimiter")
		return HighlightKind::PunctuationDelimiter;
	if (capture_name == "punctuation")
		return HighlightKind::Punctuation;
	if (capture_name == "property")
		return HighlightKind::Property;
	if (capture_name == "parameter")
		return HighlightKind::Parameter;
	if (capture_name == "attribute")
		return HighlightKind::Attribute;
	if (capture_name == "label")
		return HighlightKind::Label;
	if (capture_name == "preproc" || capture_name == "preprocessor")
		return HighlightKind::Preprocessor;

	return HighlightKind::Unknown;
}

}

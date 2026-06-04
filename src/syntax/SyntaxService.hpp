#pragma once

#include <vector>

#include "FileBufferSource.hpp"
#include "HighlightSpan.hpp"
#include "SyntaxLanguageRegistry.hpp"
#include "TextBuffer.hpp"
#include "TreeSitterHighliter.hpp"

namespace Tedit {

class SyntaxService {
public:
	std::vector<HighlightSpan> highlight(const TextBuffer& buffer) {
		const FileBufferSource* file_src { dynamic_cast<const FileBufferSource*>(buffer.get_source()) };
		if (!file_src)
			return {};

		auto language { m_registry.get_language_by_extension(file_src->get_path()) };
		if (!language)
			return {};

		const auto path = file_src->get_path();
		const auto revision = buffer.get_revisions();

		auto it { m_cache.find(path) };
		if (it != m_cache.end() && it->second.revision == revision) {
			return it->second.spans;
		}

		TreeSitterHighliter highlighter(*language);
		auto spans = highlighter.highlight(buffer.text());

		m_cache[path] = CacheEntry {
			.revision = revision,
			.spans = spans
		};

		return spans;
	}

private:
	struct CacheEntry {
		uint64_t revision;
		std::vector<HighlightSpan> spans;
	};

	SyntaxLanguageRegistry m_registry {};
	std::unordered_map<fs::path, CacheEntry> m_cache {};
};

}

#include <ranges>
#include <stdexcept>

#include "SyntaxLanguageRegistry.hpp"

namespace Tedit {

void SyntaxLanguageRegistry::register_syntax_language(const SyntaxLanguage& language) {
	const auto& check_same_name { [&a = language](const SyntaxLanguage& b) { return a.name == b.name; } };
	auto is_name_existent { !(m_languages | std::ranges::views::filter(check_same_name)).empty() };

	if (is_name_existent)
		throw std::runtime_error("Trying to register a language that already exists.");

	const auto& check_same_extensions {
		[&a = language](const SyntaxLanguage& b) {
		        for (const auto& ext : b.extensions) {
			        if (a.extensions.contains(ext))
				        return true;
		        }

		        return false;
		}
	};
	auto has_overlapping_extensions { !(m_languages | std::ranges::views::filter(check_same_extensions)).empty() };
	if (has_overlapping_extensions)
		throw std::runtime_error("Has overlapping extensions. Remove the file extensions that result in ambigious matches.");

	m_languages.push_back(language);
}

std::optional<std::reference_wrapper<const SyntaxLanguage>> SyntaxLanguageRegistry::get_language_by_extension(const fs::path& file) const {
	std::string ext { file.extension().string() };
	const auto& contains_extension { [&](const SyntaxLanguage& l) { return l.extensions.contains(ext); } };
	auto result { m_languages | std::ranges::views::filter(contains_extension) };

	if (result.empty()) {
		return std::nullopt;
	}

	return *result.begin();
}

}

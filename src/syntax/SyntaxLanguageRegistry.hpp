#pragma once

#include "syntax/SyntaxLanguage.hpp"

namespace Tedit {
class SyntaxLanguageRegistry {
public:
	void register_syntax_language(const SyntaxLanguage& language);

	std::optional<std::reference_wrapper<const SyntaxLanguage>> get_language_by_extension(const fs::path& file) const;

private:
	std::vector<SyntaxLanguage> m_languages { CPP_LANGUAGE };
};

}

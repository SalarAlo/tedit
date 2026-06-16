#pragma once

#include "Editor.hpp"
#include "IPrompt.hpp"

namespace Tedit {
class SearchPrompt : public IPrompt {
public:
	char activation_char() const override {
		return '?';
	}

	std::string name() const override {
		return "search-forward";
	}

	void submit(Editor& e, std::string_view s) override {
		e.execute_search_prompt(s);
	}

private:
};

}

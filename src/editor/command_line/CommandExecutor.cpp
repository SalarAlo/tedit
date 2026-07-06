#include "CommandExecutor.hpp"

#include "CommandLineParser.hpp"
#include "DirectoryBuffer.hpp"
#include "Editor.hpp"

namespace Tedit {

namespace {
	bool apply_render_style(Editor& editor, std::string_view style_name) {
		if (style_name == "default") {
			editor.set_render_style(RenderStyle::Default);
			return true;
		}

		if (style_name == "spotlight") {
			editor.set_render_style(RenderStyle::Spotlight);
			return true;
		}

		if (style_name == "minimal") {
			editor.set_render_style(RenderStyle::Minimal);
			return true;
		}

		return false;
	}

}

void CommandExecutor::execute_command_prompt(Editor& editor, std::string_view input) {
	CommandLineParser parser { input };
	auto result { parser.parse() };

	if (result.has_value()) {
		switch (result->type) {
		case CommandType::Write:
			editor.save_buffer();
			break;
		case CommandType::Quit:
			editor.close();
			break;

		case CommandType::Open: {
			const auto& file_path_arg { result->args[0] };
			editor.open_path(file_path_arg, false);

			editor.set_status_message("succesfully opened \"" + result->args[0] + "\"");
			break;
		}
		case CommandType::OpenExplorer:
			editor.open_buffer(std::make_unique<DirectoryBuffer>(fs::current_path()));
			editor.set_status_message("");
			break;
		case CommandType::RenderStyle: {
			const auto& style_name { result->args[0] };
			if (apply_render_style(editor, style_name))
				editor.set_status_message("render style: " + style_name);
			else
				editor.set_status_message("unknown render style: " + style_name);
			break;
		}
		case CommandType::WriteAll: {
			for (size_t i {}; i < editor.buffer_count(); i++)
				editor.save_buffer(i);
			break;
		}
		case CommandType::WriteQuit: {
			editor.save_buffer();
			editor.close();
			break;
		}
		case CommandType::QuitAll: {
			while (editor.buffer_count() != 0)
				editor.close();
			break;
		}
		}
	} else {
		editor.set_status_message(result.error_or(""));
	}
}

}

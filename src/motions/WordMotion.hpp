#pragma once

#include <cctype>

#include <optional>

#include "Editor.hpp"
#include "IMotion.hpp"

namespace Tedit {

namespace Detail {

	enum class WordKind {
		Word,
		WORD
	};

	enum class CharClass {
		Whitespace,
		Word,
		Punctuation
	};

	inline CharClass classify(char c, WordKind kind) {
		auto uc { static_cast<unsigned char>(c) };

		if (std::isspace(uc))
			return CharClass::Whitespace;

		if (kind == WordKind::WORD)
			return CharClass::Word;

		if (std::isalnum(uc) || c == '_')
			return CharClass::Word;

		return CharClass::Punctuation;
	}

	inline std::optional<char> char_at(Cursor const& pos, IBuffer const& buffer) {
		if (pos.row < 0 || pos.row >= buffer.line_count())
			return std::nullopt;

		auto const& line { buffer.line(pos.row) };
		if (pos.col < 0 || pos.col >= static_cast<int>(line.size()))
			return std::nullopt;

		return line[pos.col];
	}

	inline bool advance(Cursor& pos, IBuffer const& buffer) {
		if (pos.row < 0 || pos.row >= buffer.line_count())
			return false;

		auto const& line { buffer.line(pos.row) };
		if (pos.col < static_cast<int>(line.size())) {
			++pos.col;
			return true;
		}

		if (pos.row + 1 < buffer.line_count()) {
			++pos.row;
			pos.col = 0;
			return true;
		}

		return false;
	}

	inline bool retreat(Cursor& pos, IBuffer const& buffer) {
		if (pos.row < 0 || pos.row >= buffer.line_count())
			return false;

		if (pos.col > 0) {
			--pos.col;
			return true;
		}

		while (pos.row > 0) {
			--pos.row;
			pos.col = static_cast<int>(buffer.line(pos.row).size());

			if (pos.col > 0) {
				--pos.col;
				return true;
			}
		}

		return false;
	}

	inline bool is_word_char(Cursor const& pos, IBuffer const& buffer) {
		auto ch { char_at(pos, buffer) };
		return ch && classify(*ch, WordKind::WORD) != CharClass::Whitespace;
	}

	inline bool same_class(Cursor const& pos, IBuffer const& buffer, CharClass klass, WordKind kind) {
		auto ch { char_at(pos, buffer) };
		return ch && classify(*ch, kind) == klass;
	}

	inline void skip_to_next_word(Cursor& pos, IBuffer const& buffer) {
		while (!is_word_char(pos, buffer)) {
			if (!advance(pos, buffer))
				return;
		}
	}

	inline void skip_to_previous_word(Cursor& pos, IBuffer const& buffer) {
		while (!is_word_char(pos, buffer)) {
			if (!retreat(pos, buffer))
				return;
		}
	}

	inline void move_to_run_end(Cursor& pos, IBuffer const& buffer, WordKind kind) {
		auto ch { char_at(pos, buffer) };
		if (!ch)
			return;

		auto klass { classify(*ch, kind) };
		Cursor next { pos };
		while (advance(next, buffer) && same_class(next, buffer, klass, kind)) {
			pos = next;
			next = pos;
		}
	}

	inline void move_to_run_start(Cursor& pos, IBuffer const& buffer, WordKind kind) {
		auto ch { char_at(pos, buffer) };
		if (!ch)
			return;

		auto klass { classify(*ch, kind) };
		Cursor prev { pos };
		while (retreat(prev, buffer) && same_class(prev, buffer, klass, kind)) {
			pos = prev;
			prev = pos;
		}
	}

}

class WordMotion final : public IMotion {
public:
	explicit WordMotion(Detail::WordKind kind = Detail::WordKind::Word)
	    : m_kind(kind) {
	}

	MotionRange resolve(Editor& e) override {
		auto* buffer { e.get_buffer() };
		MotionRange result {
			buffer->get_cursor(),
			buffer->get_cursor()
		};

		auto ch { Detail::char_at(result.end, *buffer) };
		if (ch && Detail::classify(*ch, Detail::WordKind::WORD) != Detail::CharClass::Whitespace) {
			auto klass { Detail::classify(*ch, m_kind) };
			while (Detail::same_class(result.end, *buffer, klass, m_kind)) {
				if (!Detail::advance(result.end, *buffer))
					return result;
			}
		}

		Detail::skip_to_next_word(result.end, *buffer);
		return result;
	}

private:
	Detail::WordKind m_kind;
};

class WordEndMotion final : public IMotion {
public:
	explicit WordEndMotion(Detail::WordKind kind = Detail::WordKind::Word)
	    : m_kind(kind) {
	}

	MotionRange resolve(Editor& e) override {
		auto* buffer { e.get_buffer() };
		MotionRange result {
			.start = buffer->get_cursor(),
			.end = buffer->get_cursor(),
			.inclusive = true
		};

		auto ch { Detail::char_at(result.end, *buffer) };
		if (ch && Detail::classify(*ch, Detail::WordKind::WORD) != Detail::CharClass::Whitespace) {
			auto klass { Detail::classify(*ch, m_kind) };
			Cursor next { result.end };

			if (Detail::advance(next, *buffer) && Detail::same_class(next, *buffer, klass, m_kind)) {
				result.end = next;
				Detail::move_to_run_end(result.end, *buffer, m_kind);
				return result;
			}

			if (!Detail::advance(result.end, *buffer))
				return result;
		}

		Detail::skip_to_next_word(result.end, *buffer);
		Detail::move_to_run_end(result.end, *buffer, m_kind);
		return result;
	}

private:
	Detail::WordKind m_kind;
};

class BackWordMotion final : public IMotion {
public:
	explicit BackWordMotion(Detail::WordKind kind = Detail::WordKind::Word)
	    : m_kind(kind) {
	}

	MotionRange resolve(Editor& e) override {
		auto* buffer { e.get_buffer() };
		MotionRange result {
			buffer->get_cursor(),
			buffer->get_cursor()
		};

		auto ch { Detail::char_at(result.end, *buffer) };
		if (ch && Detail::classify(*ch, Detail::WordKind::WORD) != Detail::CharClass::Whitespace) {
			auto klass { Detail::classify(*ch, m_kind) };
			Cursor prev { result.end };

			if (Detail::retreat(prev, *buffer) && Detail::same_class(prev, *buffer, klass, m_kind)) {
				result.end = prev;
				Detail::move_to_run_start(result.end, *buffer, m_kind);
				return result;
			}
		}

		if (!Detail::retreat(result.end, *buffer))
			return result;

		Detail::skip_to_previous_word(result.end, *buffer);
		Detail::move_to_run_start(result.end, *buffer, m_kind);
		return result;
	}

private:
	Detail::WordKind m_kind;
};

}

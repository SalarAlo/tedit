#pragma once

#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace Tedit {
template <typename Left, typename Right>
        requires(!std::same_as<Left, Right>)
class Bimap {
public:
	void insert(const Left& l, const Right& r) {
		if (m_left_to_right.contains(l))
			throw std::logic_error("Bimap::insert: inserting duplicate value in left");

		if (m_right_to_left.contains(r))
			throw std::logic_error("Bimap::insert: inserting duplicate value in right");

		m_left_to_right.emplace(l, r);
		m_right_to_left.emplace(r, l);
	}

	void insert(const Right& r, const Left& l) {
		if (m_left_to_right.contains(l))
			throw std::logic_error("Bimap::insert: inserting duplicate value in left");

		if (m_right_to_left.contains(r))
			throw std::logic_error("Bimap::insert: inserting duplicate value in right");

		m_left_to_right.emplace(l, r);
		m_right_to_left.emplace(r, l);
	}

	void insert(const std::pair<Left, Right>& pair) {
		const auto& [l, r] = pair;
		insert(l, r);
	}

	bool contains(const Left& l) const {
		return m_left_to_right.contains(l);
	}

	bool contains(const Right& r) const {
		return m_right_to_left.contains(r);
	}

	const Right* get(const Left& left) const {
		auto it = m_left_to_right.find(left);

		if (it == m_left_to_right.end())
			return nullptr;

		return &it->second;
	}

	const Left* get(const Right& right) const {
		auto it = m_right_to_left.find(right);

		if (it == m_right_to_left.end())
			return nullptr;

		return &it->second;
	}

	bool try_remove(const Right& key_right) {
		auto it = m_right_to_left.find(key_right);

		if (it == m_right_to_left.end())
			return false;

		const Left& key_left = it->second;

		m_left_to_right.erase(key_left);
		m_right_to_left.erase(it);

		return true;
	}

	bool try_remove(const Left& key_left) {
		auto it = m_left_to_right.find(key_left);

		if (it == m_left_to_right.end())
			return false;

		const Right& key_right = it->second;

		m_right_to_left.erase(key_right);
		m_left_to_right.erase(it);

		return true;
	}

private:
	std::unordered_map<Left, Right> m_left_to_right {};
	std::unordered_map<Right, Left> m_right_to_left {};
};

}

#pragma once

#include "Action.hpp"
#include "Bimap.hpp"
#include "EditOperator.hpp"

#include "modes/Mode.hpp"

namespace Tedit {

class NormalMode final : public Mode {
public:
	virtual std::string get_name() override;

	std::unique_ptr<IAction> map_action(int key) override;

private:
	std::unique_ptr<IAction> map_motion(int key);
	std::string get_mode_details() override;
	bool is_count_key(int key) const;
	bool is_operator_key(int key) const;

private:
	size_t m_count {};
	static Bimap<char, EditOperator> m_operators;
	EditOperator m_pending_edit_operator { EditOperator::None };
};

}

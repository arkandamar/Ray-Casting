#pragma once

#include "Action.hpp"

Action::Action() {};

Action::Action(EnumAction actionName, EnumKeyType actionType) : m_action(actionName), m_type(actionType) {}

const EnumAction Action::actionName() const
{
	return m_action;
}

const EnumKeyType Action::actionType() const
{
	return m_type;
}
#pragma once

enum EnumAction { UknownAction = -1, Back, Right, Left, Up, Down, Jump, Shoot, Paused, ShowTexture, ShowGrid, ShowCollision, Play, ACTION_SIZE = Play + 1 };
enum EnumKeyType { UnknownType = -1, KeyPress, KeyRelease, TYPE_SIZE = KeyRelease + 1 };

class Action
{
	EnumAction m_action = UknownAction;
	EnumKeyType m_type = UnknownType;

public:
	Action();
	Action(EnumAction actionName, EnumKeyType actionType);

	const EnumAction actionName() const;
	const EnumKeyType actionType() const;
};
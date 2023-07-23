#pragma once

#include "Vec2.hpp"
#include "Animation.hpp"

#include <SFML/Graphics.hpp>

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2 pos = { 0.0f, 0.0f };
	Vec2 prevPos = { 0.0f, 0.0f };
	Vec2 velocity = { 0.0f, 0.0f };
	Vec2 scale = { 0.0f, 0.0f };
	float angle = 0;

	CTransform();
	CTransform(const Vec2& p);
};

class CShape : public Component
{
public:
	sf::CircleShape circle;

	CShape();
	CShape(float radius, int points, const sf::Color& fill);
};

class CConvex : public Component
{
public:
	sf::ConvexShape convex;

	CConvex();
	CConvex(int points, const sf::Color& fill);
};

class CCollision : public Component
{
public:
	float radius = 0;

	CCollision();
	CCollision(float rad);
};

class CScore : public Component
{

public:
	int score = 0;

	CScore();
	CScore(int s);
};

class CLifespan : public Component
{
public:
	int remaining = 0; // remaining amount of lifespan
	int total = 0; // total inital amount of lifespan

	CLifespan();
	CLifespan(int total);
};

class CInput : public Component
{
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool canShoot = true;

	CInput();
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;

	CAnimation();
	CAnimation(const Animation& animation, bool repeat);
};

class CGravity : public Component
{
public:
	float accel = 0;

	CGravity();
	CGravity(float accel);
};

enum EnumState { onDefault = -1, onGround, onAir, onShootAir, onShootWalk, onShoot, onWalk, onStand, STATE_COUNT = onStand + 1 };

class CState : public Component
{
public:
	EnumState state = onDefault;

	CState();
	CState(EnumState state);
};

class CBoundingBox : public Component
{
public:
	Vec2 size = { 0.0f, 0.0f };
	Vec2 halfSize = { 0.0f, 0.0f };

	CBoundingBox();
	CBoundingBox(Vec2 size);
};
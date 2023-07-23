#pragma once

#include "Components.hpp"

CTransform::CTransform() {};
CTransform::CTransform(const Vec2& p) : pos(p) {};

CShape::CShape(float radius, int points, const sf::Color& fill
	, const sf::Color& outline, float thickness)
	: circle(radius, points)
{
	circle.setFillColor(fill);
	circle.setOutlineColor(outline);
	circle.setOutlineThickness(thickness);
	circle.setOrigin(radius, radius);
};

CCollision::CCollision() : radius(0.0f) {};
CCollision::CCollision(float rad) : radius(rad) {};

CScore::CScore() : score(0) {};
CScore::CScore(int s) : score(s) {};

CLifespan::CLifespan() : remaining(0), total(0) {};
CLifespan::CLifespan(int total) : remaining(total), total(total) {};

CInput::CInput() {};

CAnimation::CAnimation() : animation(Animation()), repeat(false) {};
CAnimation::CAnimation(const Animation& ani, bool rep) : animation(ani), repeat(rep)
{
	animation.setRepeat(repeat);
};

CGravity::CGravity() : accel() {};
CGravity::CGravity(float accel) : accel(accel) {};

CState::CState() : state(onDefault) {};
CState::CState(EnumState state) : state(state) {};

CBoundingBox::CBoundingBox() : size(Vec2(0.0f, 0.0f)), halfSize(Vec2(0.0f, 0.0f)) {}
CBoundingBox::CBoundingBox(Vec2 size) : size(size), halfSize(size / 2.0f) {};


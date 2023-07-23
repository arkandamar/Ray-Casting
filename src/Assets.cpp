#pragma once

#include "Assets.hpp"

#include <iostream>

// setter
void Assets::addTexture(EnumTexture name, const std::string& path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		// error
		std::cout << "cannot load texture" << std::endl;
	}
	m_textures[name] = texture;
};

void Assets::addAnimation(EnumAnimation name, Animation animation)
{
	m_animations[name] = animation;
};

void Assets::addFont(EnumFont name, const std::string& path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
	{
		// error
		std::cout << "cannot load font" << std::endl;
	}
	m_fonts[name] = font;
};

void Assets::addSound(EnumSound name, const std::string& path)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path))
	{
		// error
		std::cout << "cannot load buffer sound from file" << std::endl;
	}
	sf::Sound sound(buffer);
	m_sounds[name] = sound;
};

void Assets::registerAnimation(EnumAnimation name, EnumTexture texture, int frameCount, int duration, Vec2 size)
{
	addAnimation(name, Animation(name, getTexture(texture), frameCount, duration, size));
}

// getter
sf::Texture& Assets::getTexture(EnumTexture name)
{
	return m_textures[name];
}

Animation& Assets::getAnimation(EnumAnimation name)
{
	return m_animations[name];
}

sf::Font& Assets::getFont(EnumFont name)
{
	return m_fonts[name];
}

sf::Sound& Assets::getSound(EnumSound name)
{
	return m_sounds[name];
}
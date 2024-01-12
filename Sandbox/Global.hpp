#pragma once
#include "SFML/System/Vector2.hpp"
namespace WINDOW {
	sf::Vector2f size = { 1920u, 1080u };
}

namespace TEXTURE_MAP {
	sf::Vector2i texSize = { 64,64 };
	sf::Vector2i texMapSize = {};
}

namespace PLAYER {

	float movSpeed = 5.0f;
	float rotSpeed = 1.5f;
}

inline constexpr float pi = 3.1415926535;
inline const unsigned int tileSize = 12.5;
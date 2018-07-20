#ifndef MAP_DATA_HPP
#define MAP_DATA_HPP

#include <vector>
#include "Vector2i.hpp"

namespace bot {
	enum class Entity : char {
		Bot = '1',
		Enemy = '2',
		Wall = '#',
		Floor = ' '
	};
	struct MapData {
		Vector2i size;
		std::vector<std::vector<Entity>> tile_set;
		Vector2i bot_position;
		Vector2i enemy_position;
	};
}  // namespace bot
#endif

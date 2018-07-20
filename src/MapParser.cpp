#include <MapParser.hpp>

#include <stdexcept>
#include <iostream>

namespace bot {
	MapParser::MapParser(const Vector2i & size,
						 const std::vector<std::string> & map_rows)
		: map_data{ size, {}, {}, {} } {
		build_map(map_rows);
	}

	const MapData & MapParser::get_parsed_map() const noexcept {
		return map_data;
	}

	void MapParser::build_map(const std::vector<std::string> & map_rows) {
		resize_map(map_data.size);
		parse_strings(map_rows);
	}

	void MapParser::resize_map(const Vector2i & size) {
		map_data.tile_set.resize(size.x);
		for (auto & row : map_data.tile_set) {
			row.resize(size.y);
		}
	}

	void MapParser::parse_strings(const std::vector<std::string> & map_rows) {
		assert_column_length(map_rows.size());
		for (auto y = 0; y < map_rows.size(); ++y) {
			assert_row_length(map_rows[y].length());
			parse_row(y, map_rows[y]);
		}
	}

	void MapParser::assert_column_length(int len) const {
		if (len > map_data.size.y) {
			throw std::runtime_error(
				"Input columns are longer than input size.");
		}
	}

	void MapParser::assert_row_length(int len) const {
		if (len > map_data.size.x) {
			throw std::runtime_error("Input row is longer than input size.");
		}
	}

	void MapParser::parse_row(int y, const std::string & row) {
		for (auto x = 0; x < row.length(); ++x) {
			Entity glyph{ row[x] };
			if (glyph != Entity::Bot && glyph != Entity::Enemy &&
				glyph != Entity::Wall && glyph != Entity::Floor) {
				throw std::runtime_error(
					std::string{ "Incorrect map character: " } + row[x]);
			}
			map_data.tile_set[x][y] = glyph;
			if (glyph == Entity::Bot) {
				map_data.bot_position = { x, y };
			} else if (glyph == Entity::Enemy) {
				map_data.enemy_position = { x, y };
			}
		}
	}
}  // namespace bot

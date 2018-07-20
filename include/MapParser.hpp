#ifndef MAP_PARSER_HPP
#define MAP_PARSER_HPP

#include "MapData.hpp"

#include <vector>
#include <string>

namespace bot {
	class MapParser {
	   public:
		MapParser(const Vector2i & size,
				  const std::vector<std::string> & map_rows);

		const MapData & get_parsed_map() const noexcept;

	   private:
		MapData map_data;

		void build_map(const std::vector<std::string> & map_rows);
		void resize_map(const Vector2i & size);
		void parse_strings(const std::vector<std::string> & map_rows);
		void assert_column_length(int len) const;
		void assert_row_length(int len) const;
		void parse_row(int y, const std::string & row);
	};

}  // namespace bot

#endif

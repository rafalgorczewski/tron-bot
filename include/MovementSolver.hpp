#ifndef MOVEMENT_SOLVER_HPP
#define MOVEMENT_SOLVER_HPP

#include "Vector2i.hpp"
#include "MapData.hpp"
#include "MapParser.hpp"
#include "Bfs.hpp"
#include "Direction.hpp"
#include "WallRunner.hpp"

#include <optional>
#include <vector>
#include <string>
#include <unordered_map>

namespace bot {
	class MovementSolver {
	   public:
		MovementSolver(const Vector2i & size,
					   const std::vector<std::string> & map_rows);

		Direction get_optimal_direction();

	   private:
		MapParser map_parser;
		MapData map_data;
		Bfs bfs;
		WallRunner wall_runner;
	};
}  // namespace bot

#endif

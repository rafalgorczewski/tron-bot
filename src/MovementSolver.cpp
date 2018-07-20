#include <MovementSolver.hpp>
#include <Bfs.hpp>
#include <Direction.hpp>
#include <WallRunner.hpp>

#include <utility>
#include <queue>
#include <random>
#include <unordered_map>
#include <cstddef>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <utility>
#include <cmath>

namespace bot {
	MovementSolver::MovementSolver(const Vector2i & size,
								   const std::vector<std::string> & map_rows)
		: map_parser(size, map_rows),
		  map_data(map_parser.get_parsed_map()),
		  bfs(map_data),
		  wall_runner(map_data) {
	}

	Direction MovementSolver::get_optimal_direction() {
		static constexpr float DEPTH = 5;

		auto bfs_result = bfs.get_optimal_direction();
		if (bfs_result) {
			return *bfs_result;
		} else {
			return wall_runner.get_best_direction(DEPTH);
		}
	}
}  // namespace bot

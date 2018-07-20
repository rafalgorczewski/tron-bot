#include "MovementSolver.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

namespace bot {
	Vector2i parse_map_size();
	std::vector<std::string> parse_map_rows();
	void print_map_info(const bot::Vector2i & size,
						const std::vector<std::string> & map_rows);
}  // namespace bot

int main() {
	try {
		auto size = bot::parse_map_size();
		auto rows = bot::parse_map_rows();
		print_map_info(size, rows);
		bot::MovementSolver solver{ size, rows };
		int dir = static_cast<int>(solver.get_optimal_direction());
		std::cout << dir << "\n";
		return 0;
	} catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
}

namespace bot {
	Vector2i parse_map_size() {
		std::string row{};
		std::getline(std::cin, row);
		std::istringstream row_isstr{ row };
		std::string width{};
		std::getline(row_isstr, width, ' ');
		std::string height{};
		std::getline(row_isstr, height, ' ');
		return { std::stoi(width), std::stoi(height) };
	}
	std::vector<std::string> parse_map_rows() {
		std::vector<std::string> map_rows{};
		std::string row{};
		while (std::getline(std::cin, row)) {
			map_rows.push_back(row);
		}
		return map_rows;
	}
	void print_map_info(const bot::Vector2i & size,
						const std::vector<std::string> & map_rows) {
		std::cerr << size.x << " " << size.y << std::endl;
		for (const auto & row : map_rows) {
			std::cerr << row << "\n";
		}
	}
}  // namespace bot

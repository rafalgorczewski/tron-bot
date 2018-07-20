#include <WallRunner.hpp>
#include <Direction.hpp>
#include <MapData.hpp>

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <cmath>
#include <utility>
#include <iostream>

namespace std {
	template <>
	struct hash<bot::Vector2i> {
		size_t operator()(const bot::Vector2i & rhs) const {
			return (rhs.x * 6) * (rhs.x * 2) + (rhs.y * 3) * (rhs.y * 5);
		}
	};
}  // namespace std

namespace bot {
	WallRunner::WallRunner(const MapData & map_data)
		: m_map_data(map_data),
		  m_nodes_visit_status(create_graph<short>(0)),
		  m_parents_of_nodes(create_graph<Node>({ -1, -1 })),
		  m_nodes_profitability(create_graph<int>(0)) {
	}

	Direction WallRunner::get_best_direction(float depth) {
		add_bot_position_to_queue();
		auto most_profitable_node = get_default_most_profitable_node();
		while (not_all_nodes_in_range_visited()) {
			auto node = dequeue_pending();
			visit_node(node);
			auto surroundings = get_surroundings(node);
			auto profitability =
				calculate_surroundings_profitability(surroundings);
			save_node_profitability(node, profitability);
			auto parents_profitability = calculate_parents_profitability(node);
			if (parents_profitability > most_profitable_node.second) {
				most_profitable_node = { node, parents_profitability };
			}
			remove_unpassable_and_visited_nodes(surroundings, depth);
			give_parents(surroundings, node);
			enqueue_and_visit_nodes(surroundings);
		}
		for (auto y = 0; y < m_map_data.size.y; ++y) {
			for (auto x = 0; x < m_map_data.size.x; ++x) {
				std::cerr << "(" << m_nodes_profitability[x][y] << ")";
			}
			std::cerr << std::endl;
		}
		if (get_surroundings(m_map_data.bot_position).empty()) {
			return Direction::Left;
		}
		return trace_target(most_profitable_node.first);
	}

	std::unordered_map<Vector2i, Direction> WallRunner::m_direction_map = {
		{ { 0, -1 }, Direction::Up },
		{ { 0, 1 }, Direction::Down },
		{ { -1, 0 }, Direction::Left },
		{ { 1, 0 }, Direction::Right },
	};

	template <typename T>
	WallRunner::Graph<T> WallRunner::create_graph(const T & value) const {
		Graph<T> nodes(m_map_data.size.x,
					   std::vector<T>(m_map_data.size.y, value));
		return nodes;
	}

	void WallRunner::add_bot_position_to_queue() {
		m_pending_nodes.push_back(m_map_data.bot_position);
	}

	WallRunner::NodeScore WallRunner::get_default_most_profitable_node() {
		return { { m_map_data.bot_position.x - 1, m_map_data.bot_position.y },
				 -99999999 };
	}

	bool WallRunner::not_all_nodes_in_range_visited() const noexcept {
		return !m_pending_nodes.empty();
	}

	WallRunner::Node WallRunner::dequeue_pending() {
		auto node = m_pending_nodes.front();
		m_pending_nodes.pop_front();
		return node;
	}

	void WallRunner::visit_node(const Node & node) {
		m_nodes_visit_status[node.x][node.y] = 1;
	}

	std::vector<WallRunner::Node> WallRunner::get_surroundings(
		const Node & node) const {
		std::vector<WallRunner::Node> surroundings{ { node.x, node.y - 1 },
													{ node.x, node.y + 1 },
													{ node.x - 1, node.y },
													{ node.x + 1, node.y } };
		return surroundings;
	}

	int WallRunner::calculate_surroundings_profitability(
		const std::vector<Node> & surroundings) const {
		auto walls_count = 0;
		for (const auto & node : surroundings) {
			if (m_map_data.tile_set[node.x][node.y] == Entity::Wall) {
				++walls_count;
			}
		}
		return calculate_profitability(walls_count);
	}

	int WallRunner::calculate_profitability(int walls_count) const {
		switch (walls_count) {
			case 0:
				return 0;
				break;
			case 1:
				return 3;
				break;
			case 2:
				return 5;
				break;
			case 3:
				return -37000;
				break;
			case 4:
				return -37000;
				break;
			default:
				return 0;
				break;
		}
	}

	void WallRunner::save_node_profitability(const Node & node,
											 int profitability) {
		m_nodes_profitability[node.x][node.y] = profitability;
	}

	int WallRunner::calculate_parents_profitability(const Node & node) const {
		auto current_node = node;
		auto parent = m_parents_of_nodes[current_node.x][current_node.y];
		int profitability =
			m_nodes_profitability[current_node.x][current_node.y];
		if (parent == Node{ -1, -1 }) {
			return profitability;
		}
		while (parent != m_map_data.bot_position) {
			profitability += m_nodes_profitability[parent.x][parent.y];
			current_node = parent;
			parent = m_parents_of_nodes[current_node.x][current_node.y];
		}
		return profitability;
	}

	void WallRunner::remove_unpassable_and_visited_nodes(
		std::vector<Node> & nodes, float depth) {
		auto check_if_valid = [&](const auto & vec2i) {
			if (vec2i.x > 0 && vec2i.x < m_map_data.size.x && vec2i.y > 0 &&
				vec2i.y < m_map_data.size.y && distance_to(vec2i) < depth) {
				return m_map_data.tile_set[vec2i.x][vec2i.y] == Entity::Wall ||
					   m_map_data.tile_set[vec2i.x][vec2i.y] == Entity::Enemy ||
					   m_nodes_visit_status[vec2i.x][vec2i.y];
			} else {
				return true;
			}
		};
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(), check_if_valid),
					nodes.end());
	}

	float WallRunner::distance_to(const Vector2i & target) const noexcept {
		return std::sqrt(std::pow(target.x - m_map_data.bot_position.x, 2) +
						 std::pow(target.y - m_map_data.bot_position.y, 2));
	}

	void WallRunner::enqueue_and_visit_nodes(const std::vector<Node> & nodes) {
		for (const auto & node : nodes) {
			visit_node(node);
			m_pending_nodes.push_back(node);
		}
	}

	void WallRunner::give_parents(const std::vector<Node> & nodes,
								  const Node & parent) {
		for (const auto & node : nodes) {
			m_parents_of_nodes[node.x][node.y] = parent;
		}
	}

	Direction WallRunner::trace_target(const Node & target) const noexcept {
		auto node = target;
		auto parent = m_parents_of_nodes[node.x][node.y];

		while (parent != m_map_data.bot_position) {
			node = parent;
			parent = m_parents_of_nodes[node.x][node.y];
		}
		auto directional_vector = node - parent;
		return m_direction_map[directional_vector];
	}
}  // namespace bot
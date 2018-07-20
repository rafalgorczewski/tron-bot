#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cstddef>
#include <optional>
#include <random>

#include <Bfs.hpp>
#include <Direction.hpp>

namespace std {
	template <>
	struct hash<bot::Vector2i> {
		size_t operator()(const bot::Vector2i & rhs) const {
			return (rhs.x * 6) * (rhs.x * 2) + (rhs.y * 3) * (rhs.y * 5);
		}
	};
}  // namespace std

namespace bot {
	Bfs::Bfs(const MapData & map_data)
		: m_map_data(map_data),
		  m_nodes_visit_status(create_graph<short>(0)),
		  m_parents_of_nodes(create_graph<Node>({ -1, -1 })) {
	}

	std::optional<Direction> Bfs::get_optimal_direction() {
		add_bot_position_to_queue();
		if (is_bot_near_enemy()) {
			return std::nullopt;
		}
		while (not_all_nodes_visited()) {
			auto cuurent_node = dequeue_pending();
			if (is_enemy(cuurent_node)) {
				return { trace_enemy() };
			}
			visit_node(cuurent_node);
			auto surroundings = get_surroundings(cuurent_node);
			remove_unpassable_and_visited_nodes(surroundings);
			give_parents(surroundings, cuurent_node);
			enqueue_and_visit_nodes(surroundings);
		}
		return std::nullopt;
	}

	std::unordered_map<Vector2i, Direction> Bfs::m_direction_map = {
		{ { 0, -1 }, Direction::Up },
		{ { 0, 1 }, Direction::Down },
		{ { -1, 0 }, Direction::Left },
		{ { 1, 0 }, Direction::Right },
	};

	template <typename T>
	Bfs::Graph<T> Bfs::create_graph(const T & value) const {
		Graph<T> nodes(m_map_data.size.x,
					   std::vector<T>(m_map_data.size.y, value));
		return nodes;
	}

	bool Bfs::is_bot_near_enemy() const {
		auto surroundings = get_surroundings(m_map_data.bot_position);
		for (const auto & tile : surroundings) {
			if (tile == m_map_data.enemy_position) {
				return true;
			}
		}
		return false;
	}

	void Bfs::add_bot_position_to_queue() {
		m_pending_nodes.push_back(m_map_data.bot_position);
	}

	bool Bfs::not_all_nodes_visited() const noexcept {
		return !m_pending_nodes.empty();
	}

	Bfs::Node Bfs::dequeue_pending() {
		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(m_pending_nodes.begin(), m_pending_nodes.end(), g);
		auto node = m_pending_nodes.front();
		m_pending_nodes.pop_front();
		return node;
	}

	bool Bfs::is_enemy(const Node & node) const noexcept {
		return node == m_map_data.enemy_position;
	}

	Direction Bfs::trace_enemy() const noexcept {
		auto node = m_map_data.enemy_position;
		auto parent = m_parents_of_nodes[node.x][node.y];

		while (parent != m_map_data.bot_position) {
			node = parent;
			parent = m_parents_of_nodes[node.x][node.y];
		}
		auto directional_vector = node - parent;
		return m_direction_map[directional_vector];
	}

	void Bfs::visit_node(const Node & node) {
		m_nodes_visit_status[node.x][node.y] = 1;
	}

	std::vector<Bfs::Node> Bfs::get_surroundings(const Node & node) const {
		std::vector<Bfs::Node> surroundings{ { node.x, node.y - 1 },
											 { node.x, node.y + 1 },
											 { node.x - 1, node.y },
											 { node.x + 1, node.y } };
		return surroundings;
	}

	void Bfs::remove_unpassable_and_visited_nodes(std::vector<Node> & nodes) {
		auto check_if_valid = [&](const auto & vec2i) {
			if (vec2i.x > 0 && vec2i.x < m_map_data.size.x && vec2i.y > 0 &&
				vec2i.y < m_map_data.size.y) {
				return m_map_data.tile_set[vec2i.x][vec2i.y] == Entity::Wall ||
					   m_nodes_visit_status[vec2i.x][vec2i.y];
			} else {
				return true;
			}
		};
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(), check_if_valid),
					nodes.end());
	}

	void Bfs::give_parents(const std::vector<Bfs::Node> & nodes,
						   const Node & parent) {
		for (const auto & node : nodes) {
			m_parents_of_nodes[node.x][node.y] = parent;
		}
	}

	void Bfs::enqueue_and_visit_nodes(const std::vector<Bfs::Node> & nodes) {
		for (const auto & node : nodes) {
			visit_node(node);
			m_pending_nodes.push_back(node);
		}
	}
}  // namespace bot
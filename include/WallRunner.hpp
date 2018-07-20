#ifndef WALL_RUNNER_HPP
#define WALL_RUNNER_HPP

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cstddef>
#include <utility>
#include <deque>

#include "Direction.hpp"
#include "MapData.hpp"
#include "Bfs.hpp"

namespace bot {
	class WallRunner {
	   public:
		explicit WallRunner(const MapData & map_data);

		WallRunner(const WallRunner & blala);
		WallRunner(WallRunner && blabla);

		Direction get_best_direction(float depth);

	   private:
		template <typename T>
		using Graph = std::vector<std::vector<T>>;
		using Node = Vector2i;
		using NodeScore = std::pair<Node, int>;

		MapData m_map_data;
		Graph<short> m_nodes_visit_status;
		Graph<short> m_nodes_queue_status;
		Graph<Node> m_parents_of_nodes;
		Graph<int> m_nodes_profitability;
		std::deque<Node> m_pending_nodes{};
		static std::unordered_map<Vector2i, Direction> m_direction_map;

		template <typename T>
		Graph<T> create_graph(const T & value) const;
		void add_bot_position_to_queue();
		NodeScore get_default_most_profitable_node();
		bool not_all_nodes_in_range_visited() const noexcept;
		Node dequeue_pending();
		void visit_node(const Node & node);
		std::vector<Node> get_surroundings(const Node & node) const;
		int calculate_surroundings_profitability(
			const std::vector<Node> & surroundings) const;
		int calculate_profitability(int walls_count) const;
		void save_node_profitability(const Node & node, int profitability);
		int calculate_parents_profitability(const Node & node) const;
		void remove_unpassable_and_visited_nodes(std::vector<Node> & nodes,
												 float depth);
		float distance_to(const Vector2i & target) const noexcept;
		void enqueue_and_visit_nodes(const std::vector<Node> & nodes);
		void give_parents(const std::vector<Node> & nodes, const Node & parent);
		Direction trace_target(const Node & target) const noexcept;
	};
}  // namespace bot
#endif
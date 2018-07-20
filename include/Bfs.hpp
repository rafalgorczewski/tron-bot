#ifndef BFS_HPP
#define BFS_HPP

#include <vector>
#include <deque>
#include <unordered_map>
#include <optional>

#include "MapData.hpp"
#include "Vector2i.hpp"
#include "Direction.hpp"

namespace bot {
	class Bfs {
	   public:
		explicit Bfs(const MapData & map_data);

		std::optional<Direction> get_optimal_direction();

	   private:
		template <typename T>
		using Graph = std::vector<std::vector<T>>;
		using Node = Vector2i;

		MapData m_map_data;
		Graph<short> m_nodes_visit_status;
		Graph<short> m_nodes_queue_status;
		Graph<Node> m_parents_of_nodes;
		std::deque<Node> m_pending_nodes{};
		static std::unordered_map<Vector2i, Direction> m_direction_map;

		template <typename T>
		Graph<T> create_graph(const T & value) const;
		bool is_bot_near_enemy() const;
		void add_bot_position_to_queue();
		bool not_all_nodes_visited() const noexcept;
		Node dequeue_pending();
		bool is_enemy(const Node & node) const noexcept;
		Direction trace_enemy() const noexcept;
		void visit_node(const Node & node);
		std::vector<Node> get_surroundings(const Node & node) const;
		void remove_unpassable_and_visited_nodes(std::vector<Node> & nodes);
		void give_parents(const std::vector<Node> & nodes, const Node & parent);
		void enqueue_and_visit_nodes(const std::vector<Node> & nodes);
	};
}  // namespace bot
#endif
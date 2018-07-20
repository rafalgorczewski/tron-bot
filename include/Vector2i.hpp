#ifndef VECTOR2I_HPP
#define VECTOR2I_HPP

namespace bot {

	struct Vector2i {
		int x;
		int y;
		bool operator==(const Vector2i & rhs) const {
			return (x == rhs.x) && (y == rhs.y);
		}
		bool operator!=(const Vector2i & rhs) const {
			return !operator==(rhs);
		}

		Vector2i operator-(const Vector2i & rhs) const {
			return { x - rhs.x, y - rhs.y };
		}
	};

}  // namespace bot
#endif

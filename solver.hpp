#pragma once
#include <variant>
#include <vector>

class Solver {
public:
Solver(size_t width, size_t height, std::vector<std::vector<int>> cols, std::vector<std::vector<int>> rows);
struct BlackSpace {
	size_t min_start;
	size_t max_end;
	size_t size;
	int size_current() const {
		return static_cast<int>(min_start) + 2 * size - max_end;
	}
	BlackSpace(size_t min, size_t sz) : min_start(min),  max_end(0), size(sz) {}
};
struct WhiteSpace {
	size_t size_current;
	size_t min_start;
	size_t max_end;
	WhiteSpace(size_t min, size_t sz = 1) : size_current(sz),  min_start(min), max_end(0) {}

};


enum class COLOR {
	UNDEFINED,
	WHITE,
	BLACK
};

struct Pixel {
	enum COLOR color;
	std::variant<nullptr_t, WhiteSpace*, BlackSpace*> rowElem;
	std::variant<nullptr_t, WhiteSpace*, BlackSpace*> colElem;
};

void Solve();
void Print();

using LineContext = std::vector<std::vector<std::variant<WhiteSpace, BlackSpace>>>;

private :
	static LineContext InitializeLines(size_t dimension, std::vector<std::vector<int>>& blackSpaceSizes);
	size_t _width;
	size_t _height;
	std::vector<std::vector<Pixel>> _state;
	std::vector<std::vector<std::variant<WhiteSpace, BlackSpace>>> _rows;
	std::vector<std::vector<std::variant<WhiteSpace, BlackSpace>>> _cols;
};
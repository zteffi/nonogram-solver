#pragma once
#include <vector>

using uchar = unsigned char;

class Solver {

public:
	Solver(size_t width, size_t height, std::vector<std::vector<uchar>> cols, std::vector<std::vector<uchar>> rows);



	enum class COLOR {
		UNDEFINED,
		WHITE,
		BLACK
	};

	void Solve();
private :
	void PrepareStates();

	std::vector<std::tuple<size_t, uchar>> _combinations_and_stars_pairs;
	std::vector<std::vector<uchar>> _cols;
	std::vector<std::vector<uchar>> _rows;
	size_t _width;
	size_t _height;
};
#include "solver.hpp"

#include <iostream>

namespace {

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

// explicit deduction guide (not needed as of C++20,
// but vscode intellisense expects it regardless)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}

using LineContext = std::vector<std::vector<std::variant<Solver::WhiteSpace, Solver::BlackSpace>>>;


Solver::Solver(size_t width, size_t height, std::vector<std::vector<int>> cols, std::vector<std::vector<int>> rows)
	: _width(width)
	, _height(height)
{
	std::vector<Pixel> row{ width, {COLOR::UNDEFINED, nullptr, nullptr } };
	_state.resize(height, row);
	_rows = InitializeLines(width, rows);
	_cols = InitializeLines(height, cols);
}

LineContext Solver::InitializeLines(size_t dimension, std::vector<std::vector<int>>& blackSpaceSizes) {
	LineContext lines;
	lines.resize(blackSpaceSizes.size(), {WhiteSpace{0, 0}});
	for (size_t i = 0; i < blackSpaceSizes.size(); i++) {
		size_t sz_counter = 0;
		for (size_t j = 0; j < blackSpaceSizes[i].size(); j++) {
			size_t blackElemSize = blackSpaceSizes[i][j];
			lines[i].push_back(BlackSpace{sz_counter, blackElemSize});
			sz_counter += blackElemSize;
			lines[i].push_back(WhiteSpace{sz_counter});
			sz_counter += 1;
		}
		sz_counter = dimension;
		auto& last_white = std::get<WhiteSpace>(lines[i].back());
		last_white.size_current = 0;
		for (auto it = lines[i].rbegin(); it != lines[i].rend(); it++)
		{
			std::visit(overloaded {

				[&sz_counter](BlackSpace& b) {
					b.max_end = sz_counter;
					sz_counter -= b.size;
				},
				[&sz_counter](WhiteSpace& w) {
					w.max_end = sz_counter;
					sz_counter -= w.size_current;
				}
			}, *it);
		}
	}
	return lines;
}

void Solver::Solve() {
	int index = 0;
	for (auto& row : _rows) {
		for (auto& e : row) {
			std::visit(overloaded {
				[&](BlackSpace& b) {
					if (int sz = b.size_current(); sz > 0) {
						auto margin = b.size - sz;
						for (int x = b.min_start+margin; x < b.max_end-margin; x++) {
							_state[index][x].color = COLOR::BLACK;
							_state[index][x].rowElem = &b;
						}
					}
				},
				[&](WhiteSpace& w) {
					if (w.size_current == w.max_end - w.min_start) {
						for (int x = w.min_start; x < w.max_end; x++) {
							_state[index][x].color = COLOR::WHITE;
							_state[index][x].rowElem = &w;
						}

					}
				}
			}, e);
		}
		++ index;
	}
	index = 0;
	for (auto& col : _cols) {
		for (auto& e : col) {
			std::visit(overloaded {
				[&](BlackSpace& b) {
					if (int sz = b.size_current(); sz > 0) {
						auto margin = b.size - sz;
						for (int y = b.min_start+margin; y < b.max_end-margin; y++) {
							_state[y][index].color = COLOR::BLACK;
							_state[y][index].rowElem = &b;
						}
					}
				},
				[&](WhiteSpace& w) {
					if (w.size_current == w.max_end - w.min_start) {
						for (int y = w.min_start; y < w.max_end; y++) {
							_state[y][index].color = COLOR::WHITE;
							_state[y][index].rowElem = &w;
						}
						std::cout << index << " White(" <<w.size_current<< "): "  << w.min_start << ">> " << w.max_end  << std::endl;
					}
				}
			}, e);

		}
		++ index;
	}
}

void Solver::Print() {
	for (auto& rr : _cols) {
		for (auto e : rr) {
				std::visit(overloaded {

				[](BlackSpace& b) {
					std::cout << "B (" << b.size_current() << "/" << b.size << ") " << b.min_start << " :" << b.max_end << " | ";
				},
				[](WhiteSpace& w) {
				}
			}, e);
		}
		std::cout << std::endl;
	}

	int ridx = 0;
	for (auto& row : _state) {
		int idx = 0;
		for (auto& elem : row) {
			switch (elem.color) {
			case COLOR::UNDEFINED:
					std::cout << " /";
					break;
				 case COLOR::WHITE:
					std::cout << "  ";
				break;
				 case COLOR::BLACK:
					 std::cout << " B";
					break;

			};
			std::cout << ((++idx)%5 ? " ": "|");
		}
		std::cout << std::endl << ((++ridx%5)?"":"--------------|--------------|--------------|--------------|\n");
	}
}
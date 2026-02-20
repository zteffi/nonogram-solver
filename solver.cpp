#include "solver.hpp"

#include <iostream>
#include <numeric>
#include <bitset>
#include <limits>
#include <bit>

namespace {

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

// explicit deduction guide (not needed as of C++20,
// but vscode intellisense expects it regardless)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

size_t binomial_coefficient(size_t n, size_t r){

    size_t sum = 1;
    for (size_t i = 1; i <= r; i++) {
        sum = sum * (n - r + i) / i;
    }
    return sum;
}

void print(const std::vector<size_t>& img, size_t cols)
{
	for (auto row: img)
	{
		for (int i = 0; i < cols; i++) {
			std::cout << ((row % 2) ? "##" : "__");
			row >>= 1;
		}
		std::cout << '\n';
	}
	std::cout << "------------\n";
}

bool validate(const std::vector<size_t>& candidate,
	const std::vector<std::vector<uchar>>& cols) {
	for (uchar i = 0; i < cols.size(); i++) {
		size_t col = 0;
		for (auto& r : candidate) {
			col <<= 1;
			col += ((1<<i)&r) >> i;
		}
		auto it = cols[cols.size() - i - 1].rbegin();
		uchar oneCount = 0;
		for (;it != cols[cols.size() - i - 1].rend(); it++) {
			while (!(col % 2)) {
				col >>= 1;
				if (!col) {
					return false;
				}
			}
			while (col % 2) {
				oneCount++;
				col >>= 1;
			}
			if (oneCount != *it) {
				return false;
			}
			oneCount = 0;
		}
	}
	return true;
}


std::vector<uchar> bars_to_composition(const std::vector<uchar>& bars, int n, int k) {
    std::vector<uchar> comp(k);
    comp[0] = bars[0];
    for (int i = 1; i < k - 1; ++i) {
        comp[i] = bars[i] - bars[i - 1] - 1;
    }
    comp[k - 1] = (n + k - 1) - 1 - bars.back();
    return comp;
}

std::pair<bool, std::vector<uchar>> next_combination(std::vector<uchar>& bars, int n) {
	if (n == 0 || bars.empty()) {
		return {false, {0 , 0}}; //edgecase
	}
	int k = bars.size();
	int total = n + k;
    for (int i = k - 1; i >= 0; --i) {
        if (bars[i] < total - k + i) {
            ++bars[i];
            for (int j = i + 1; j < k; ++j)
                bars[j] = bars[j - 1] + 1;
            return {true, bars_to_composition(bars, n, k + 1)};
        }
    }
    std::iota(bars.begin(), bars.end(), 0);
	return {false, bars_to_composition(bars, n-1, k + 1)};
}
}


Solver::Solver(size_t width, size_t height, std::vector<std::vector<uchar>> cols, std::vector<std::vector<uchar>> rows)
	: _width(width)
	, _height(height)
	, _cols(cols)
	, _rows(rows)
{
	PrepareStates();

}

void Solver::PrepareStates()
{
	_combinations_and_stars_pairs.reserve(_rows.size());
	for (const auto& row : _rows)
	{
		// stars and bars
		auto bars = row.size();

		// assignable blank spaces
		// (first white space between colored spaces is not counted)
		auto stars = _width -  std::accumulate(row.begin(), row.end(), bars-1);
		auto combinations = binomial_coefficient(stars + bars, bars);
		_combinations_and_stars_pairs.push_back({combinations, stars});

	}
 }

void Solver::Solve()
{
	//bruteforce
	size_t val = std::accumulate(_combinations_and_stars_pairs.begin(), _combinations_and_stars_pairs.end(), 1,
		[](size_t acc, auto& b){ return acc* std::get<0>(b); }
	);
	std::vector<std::vector<uchar>> bars;
	for (auto row: _rows)
	{
		bars.push_back(std::vector<uchar>(row.size()));
		std::iota(bars.back().begin(), bars.back().end(), 0);
	}

	std::cout << "Trying " << val << " times\n";
	std::vector<size_t> img(_rows.size(), 0);
	for (size_t i = 0; i < val; i++) {

		auto index = 0;
		for (index = 0; index < bars.size(); index++)
		{
			auto [stop, whites] = next_combination(bars[index], std::get<1>(_combinations_and_stars_pairs[index]));
			img[index] = 0;
			for (auto j = 0u; j < _rows[index].size(); j++) {
				img[index] <<= _rows[index][j];
				img[index] +=  (1 << _rows[index][j]) - 1;
				img[index] <<= whites[j+1] + static_cast<uchar>(j + 1 < _rows[index].size());
			}
			if (stop && i != 0) {
				break;
			}
		}
		if (!(i % (4 * 1024 *1024))) {
			std::cout << i << " / " << val << std::endl;
		}
		if (validate(img, _cols)) {
			std::cout << "Solution!\n";

			print(img, _cols.size());
			return;
		}

	}
}


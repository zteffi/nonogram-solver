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

void print(const std::vector<size_t>& img)
{

	for (auto row: img)
	{

		std::cout << std::bitset<5>(row) << '\n';
	}
	std::cout << img.size()<< "------------\n";
}

bool validate(const std::vector<size_t>& candidate,
	const std::vector<std::vector<uchar>>& cols) {
	for (uchar i = 0; i < cols.size(); i++) {
		size_t col = 0;
		for (auto& r : candidate) {
			col <<= 1;
			col += ((1<<i)&r) >> i;
		}
		auto it = cols[i].rbegin();
		uchar oneCount = 0;
		for (;it != cols[i].rend(); it++) {
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

bool next_combination(std::vector<uchar>& comb, int n) {
    int k = comb.size();
    for (int i = k - 1; i >= 0; --i) {
        if (comb[i] < n - k) {
            ++comb[i];
            for (int j = i + 1; j < k; ++j) {
                comb[j] = comb[j - 1] + 1;
            }
            return true;
        }
    }
    return false;
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
	std::vector<std::vector<uchar>> vec;
	for (auto row: _rows)
	{
		vec.push_back(std::vector<uchar>(row.size() + 1));
		std::iota(vec.back().begin(), vec.back().end(), 0);
	}

	std::cout << "Trying " << val << " times\n";
	std::vector<size_t> img(_rows.size(), 0);
	for (size_t i = 0; i < val; i++) {

		auto index = 0;
		for (index = 0; index < vec.size(); index++)
		{
			bool next = next_combination(vec[index], static_cast<int>(std::get<0>(_combinations_and_stars_pairs[index]))) && i > 0;
			if (!next) {
				std::iota(vec.back().begin(), vec.back().end(), 0);
			}
			img[index] = 0;
			for (auto j = _rows[index].size() - 1; j != std::numeric_limits<size_t>::max(); j--) {
				img[index] |= (1<<_rows[index][j]) - 1;
				size_t ws = 0;
				if (j) {
					ws = vec[index][j] - vec[index][j-1] +  _rows[index][j];
				} else {
					ws = vec[index][j];
				}
				img[index] <<= ws;
			}
		}
		std::cout << i << ": "<< std::endl;
		if (validate(img, _cols)) {
			std::cout << "Solution!\n";
			print(img);
			return;
		}

	}
}


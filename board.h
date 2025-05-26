#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "ship.h"
#include <random>

// Field cells definition
enum Cell { EMPTY, SHIP };

// Board class manages ship placement and hint calculation
class Board {    
private:
    int N;
    std::vector<std::vector<Cell>> field;

public:
        // Initialize a board with given size
    Board(int n);

// Check if a ship can be placed at position (x, y) with given length and orientation
    bool can_place(int x, int y, int len, bool horizontal) const;
// Place a ship at position (x, y) with given length and orientation
    void place_ship(int x, int y, int len, bool horizontal, Cell value);
// Recursively place ships using backtracking
    bool solve(std::vector<Ship>& ships, int idx, std::mt19937& rng);
// Calculate hints for empty cell sequences in rows and columns
    void calculate_empty_hints(std::vector<std::vector<int>>& row_hints, std::vector<std::vector<int>>& col_hints) const;
// Return the board field
    const std::vector<std::vector<Cell>>& get_field() const;
};

#endif // BOARD_H

#include "board.h"
#include <random>
#include <tuple>
#include <algorithm>

Board::Board(int n) : N(n), field(n, std::vector<Cell>(n, EMPTY)) {}

bool Board::can_place(int x, int y, int len, bool horizontal) const {
    for (int i = 0; i < len; ++i) {
        int nx = x + (horizontal ? 0 : i);
        int ny = y + (horizontal ? i : 0);
        if (nx < 0 || ny < 0 || nx >= N || ny >= N) return false;
        if (field[nx][ny] != EMPTY) return false;
        // Check adjacent cells to prevent ships from touching
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                int tx = nx + dx, ty = ny + dy;
                if (tx >= 0 && ty >= 0 && tx < N && ty < N) {
                    if (field[tx][ty] == SHIP && (dx != 0 || dy != 0)) return false;
                }
            }
        }
    }
    return true;
}

void Board::place_ship(int x, int y, int len, bool horizontal, Cell value) {
    for (int i = 0; i < len; ++i) {
        int nx = x + (horizontal ? 0 : i);
        int ny = y + (horizontal ? i : 0);
        field[nx][ny] = value;
    }
}

bool Board::solve(std::vector<Ship>& ships, int idx, std::mt19937& rng) {
    if (idx == static_cast<int>(ships.size())) return true;
    // Generate all valid positions for the current ship
    std::vector<std::tuple<int, int, bool>> positions;
    for (int x = 0; x < N; ++x) {
        for (int y = 0; y < N; ++y) {
            for (int h = 0; h < 2; ++h) {
                bool horizontal = h == 0;
                if (can_place(x, y, ships[idx].size, horizontal)) {
                    positions.emplace_back(x, y, horizontal);
                }
            }
        }
    }
    // Shuffle positions to introduce randomness in layout
    std::shuffle(positions.begin(), positions.end(), rng);
    // Try placing ship in each valid position
    for (const auto& pos : positions) {
        int x, y; bool horizontal;
        std::tie(x, y, horizontal) = pos;
        place_ship(x, y, ships[idx].size, horizontal, SHIP);
        ships[idx].count--;

        if (ships[idx].count == 0) {
            if (solve(ships, idx + 1, rng)) return true;
        } else {
            if (solve(ships, idx, rng)) return true;
        }
        // Backtrack
        ships[idx].count++;
        place_ship(x, y, ships[idx].size, horizontal, EMPTY);
    }
    return false;
}

void Board::calculate_empty_hints(std::vector<std::vector<int>>& row_hints, std::vector<std::vector<int>>& col_hints) const {
    row_hints.clear();
    col_hints.clear();
    row_hints.resize(N);
    col_hints.resize(N);
    // Row hints
    for (int i = 0; i < N; ++i) {
        int cnt = 0;
        for (int j = 0; j < N; ++j) {
            if (field[i][j] == EMPTY) cnt++;
            else if (cnt > 0) { row_hints[i].push_back(cnt); cnt = 0; }
        }
        if (cnt > 0) row_hints[i].push_back(cnt);
        if (row_hints[i].empty()) row_hints[i].push_back(0);
    }
    // Column hints
    for (int j = 0; j < N; ++j) {
        int cnt = 0;
        for (int i = 0; i < N; ++i) {
            if (field[i][j] == EMPTY) cnt++;
            else if (cnt > 0) { col_hints[j].push_back(cnt); cnt = 0; }
        }
        if (cnt > 0) col_hints[j].push_back(cnt);
        if (col_hints[j].empty()) col_hints[j].push_back(0);
    }
}
// Return the board field
const std::vector<std::vector<Cell>>& Board::get_field() const {
    return field;
}

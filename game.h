#ifndef GAME_H
#define GAME_H

#include <vector>
#include "board.h"
#include "ship.h"
// Game class controls game setup and user interaction
class Game {
private:
    int N;
    std::vector<Ship> fleet;
    Board board;

public:
// Initialize game with board size and fleet
    Game(int size, const std::vector<Ship>& ships);

    Board& get_board();
// Prepare the board by placing ships using solver
    bool prepare_board();
// Print board with hints only (no ships shown)
    void print_empty_field_with_hints();
// Print board with both ships and hints
    void print_field_with_ships_and_hints();
};

#endif // GAME_H

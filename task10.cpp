#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "ship.h"
#include "game.h"

const int CONSOLE_CLEAR_LINES = 100;
constexpr int MIN_BOARD_SIZE = 5;
constexpr int MAX_BOARD_SIZE = 10;


// Setup predefined board and fleet
void get_predefined_setup(int& N, std::vector<Ship>& fleet) {
    N = 8;
    fleet.clear();
    fleet = {
        Ship(4, 1),
        Ship(3, 2),
        Ship(2, 3),
        Ship(1, 4)
    };
    std::cout << "Using predefined setup: 8x8 board with fleet {4x1, 3x2, 2x3, 1x4}\n";
}

// Entry point
int main() {
    int N;
    std::vector<Ship> fleet;
    std::string choice;
while (true) {
    std::cout << "Do you want to (1) enter board and fleet manually or (2) use predefined setup? ";
    std::cin >> choice;
    if (choice == "1" || choice == "2") break;
    std::cout << "Invalid choice! Please enter 1 or 2.\n";
}

if (choice == "2") {
    get_predefined_setup(N, fleet);
    } else {
        while (true) {
            std::cout << "Enter board size (from 5 to 10): ";
            std::cin >> N;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input! Please enter a number.\n";
                continue;
            }
            if (N >= MIN_BOARD_SIZE && N <= MAX_BOARD_SIZE) break;
            std::cout << "Invalid size! Try again.\n";
        }

        int max_ship_size = std::min(4, N);
        int max_cells = N * N;
        int total_cells = 0;

        for (int size = max_ship_size; size >= 1; --size) {
            int count;
            while (true) {
                std::cout << "How many ships of size " << size << "? ";
                std::cin >> count;
                if (count >= 0 && total_cells + count * size <= max_cells) break;
                std::cout << "Too many ships or not enough space! Try again.\n";
            }
            if (count > 0) {
                fleet.push_back(Ship(size, count));
                total_cells += count * size;
            }
        }
    }

    while (true) {
        Game game(N, fleet);
        std::cout << "--- Battleship ---\n";
        std::cout << "1 Show empty board with hints (fleet hidden)\n";
        std::cout << "2 Show board with ships and hints\n";
        std::cout << "3 Exit\n";
        std::cout << "Choose mode: ";
        std::string mode;
        std::cin >> mode;

        if (mode == "3") break;

        if (mode == "1" || mode == "2") {
            if (game.prepare_board()) {
                if (mode == "1") game.print_empty_field_with_hints();
                else game.print_field_with_ships_and_hints();
            } else {
                std::cout << "\nNo solution\n";
            }
            std::cout << "\nPress Enter to return to menu...";
            std::cin.ignore(1000000, '\n');
            std::cin.get();
        } else {
            std::cout << "\nInvalid choice!\n";
            std::cout << "\nPress Enter to return to menu...";
            std::cin.ignore(1000000, '\n');
            std::cin.get();
        }
    }

    return 0;
}

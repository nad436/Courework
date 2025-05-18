#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <random>
#include <algorithm>
#include <tuple>

using namespace std;

const int CONSOLE_CLEAR_LINES = 100;
constexpr int MIN_BOARD_SIZE = 5;
constexpr int MAX_BOARD_SIZE = 10;

// Ship class represents a type of ship with its size and count
class Ship {
public:
    int size;
    int count;

    Ship(int s, int c) : size(s), count(c) {}
};

// Field cells definition
enum Cell { EMPTY, SHIP };

// Board class manages ship placement and hint calculation
class Board {
private:
    int N;
    vector<vector<Cell>> field;

public:
    // Initialize a board with given size
    Board(int n) : N(n), field(n, vector<Cell>(n, EMPTY)) {}

    // Check if a ship can be placed at position (x, y) with given length and orientation
    bool can_place(int x, int y, int len, bool horizontal) const {
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

    // Place a ship at position (x, y) with given length and orientation
    void place_ship(int x, int y, int len, bool horizontal, Cell value) {
        for (int i = 0; i < len; ++i) {
            int nx = x + (horizontal ? 0 : i);
            int ny = y + (horizontal ? i : 0);
            field[nx][ny] = value;
        }
    }

    // Recursively place ships using backtracking
    bool solve(vector<Ship>& ships, int idx, mt19937& rng) {
        if (idx == static_cast<int>(ships.size())) return true;

        // Generate all valid positions for the current ship
        vector<tuple<int, int, bool>> positions;
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
        shuffle(positions.begin(), positions.end(), rng);

        // Try placing ship in each valid position
        for (const auto& pos : positions) {
            int x, y; bool horizontal;
            tie(x, y, horizontal) = pos;
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

    // Calculate hints for empty cell sequences in rows and columns
    void calculate_empty_hints(vector<vector<int>>& row_hints, vector<vector<int>>& col_hints) const {
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
    const vector<vector<Cell>>& get_field() const {
        return field;
    }
};

// Game class controls game setup and user interaction
class Game {
private:
    int N;
    vector<Ship> fleet;
    Board board;

public:
    // Initialize game with board size and fleet
    Game(int size, const vector<Ship>& ships) : N(size), fleet(ships), board(size) {}

    Board& get_board() {
        return board;
    }

    // Prepare the board by placing ships using solver
    bool prepare_board() {
        vector<Ship> fleet_copy = fleet;
        random_device rd;
        mt19937 rng(rd());
        shuffle(fleet_copy.begin(), fleet_copy.end(), rng);
        return board.solve(fleet_copy, 0, rng);
    }

    // Print board with hints only (no ships shown)
    void print_empty_field_with_hints() {
        vector<vector<int>> rh, ch;
        board.calculate_empty_hints(rh, ch);

        int max_row_hint = 0, max_col_hint = 0;
        for (const auto& h : rh) max_row_hint = max(max_row_hint, (int)h.size());
        for (const auto& h : ch) max_col_hint = max(max_col_hint, (int)h.size());

        // Print column hints
        for (int i = 0; i < max_col_hint; ++i) {
            cout << setw(max_row_hint * 2 + 2) << "";
            for (int j = 0; j < N; ++j) {
                int hintIdx = (int)ch[j].size() - max_col_hint + i;
                if (hintIdx >= 0) cout << setw(2) << ch[j][hintIdx];
                else cout << "  ";
            }
            cout << '\n';
        }

        // Print horizontal border
        cout << setw(max_row_hint * 2 + 2) << "" << "+-";
        for (int j = 0; j < N; ++j) cout << "--";
        cout << "+\n";

        // Print empty board with row hints
        for (int i = 0; i < N; ++i) {
            for (int k = 0; k < max_row_hint; ++k) {
                int hintIdx = (int)rh[i].size() - max_row_hint + k;
                if (hintIdx >= 0) cout << setw(2) << rh[i][hintIdx];
                else cout << "  ";
            }
            cout << " |";
            for (int j = 0; j < N; ++j) cout << " .";
            cout << '\n';
        }
    }

    // Print board with both ships and hints
    void print_field_with_ships_and_hints() {
        vector<vector<int>> rh, ch;
        board.calculate_empty_hints(rh, ch);

        int max_row_hint = 0, max_col_hint = 0;
        for (const auto& h : rh) max_row_hint = max(max_row_hint, (int)h.size());
        for (const auto& h : ch) max_col_hint = max(max_col_hint, (int)h.size());

        // Print column hints
        for (int i = 0; i < max_col_hint; ++i) {
            cout << setw(max_row_hint * 2 + 2) << "";
            for (int j = 0; j < N; ++j) {
                int hintIdx = (int)ch[j].size() - max_col_hint + i;
                if (hintIdx >= 0) cout << setw(2) << ch[j][hintIdx];
                else cout << "  ";
            }
            cout << '\n';
        }

        // Print horizontal border
        cout << setw(max_row_hint * 2 + 2) << "" << "+-";
        for (int j = 0; j < N; ++j) cout << "--";
        cout << "+\n";

        // Print board with ships and row hints
        const auto& field = board.get_field();
        for (int i = 0; i < N; ++i) {
            for (int k = 0; k < max_row_hint; ++k) {
                int hintIdx = (int)rh[i].size() - max_row_hint + k;
                if (hintIdx >= 0) cout << setw(2) << rh[i][hintIdx];
                else cout << "  ";
            }
            cout << " |";
            for (int j = 0; j < N; ++j)
                cout << (field[i][j] == SHIP ? " O" : " .");
            cout << '\n';
        }
    }
};

// Setup predefined board and fleet
void get_predefined_setup(int& N, vector<Ship>& fleet) {
    N = 8;
    fleet.clear();
    fleet = {
        Ship(4, 1),
        Ship(3, 2),
        Ship(2, 3),
        Ship(1, 4)
    };
    cout << "Using predefined setup: 8x8 board with fleet {4x1, 3x2, 2x3, 1x4}\n";
}

// Entry point
int main() {
    int N;
    vector<Ship> fleet;
    string choice;
while (true) {
    cout << "Do you want to (1) enter board and fleet manually or (2) use predefined setup? ";
    cin >> choice;
    if (choice == "1" || choice == "2") break;
    cout << "Invalid choice! Please enter 1 or 2.\n";
}

if (choice == "2") {
    get_predefined_setup(N, fleet);
    } else {
        while (true) {
            cout << "Enter board size (from 5 to 10): ";
            cin >> N;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number.\n";
                continue;
            }
            if (N >= MIN_BOARD_SIZE && N <= MAX_BOARD_SIZE) break;
            cout << "Invalid size! Try again.\n";
        }

        int max_ship_size = min(4, N);
        int max_cells = N * N;
        int total_cells = 0;

        for (int size = max_ship_size; size >= 1; --size) {
            int count;
            while (true) {
                cout << "How many ships of size " << size << "? ";
                cin >> count;
                if (count >= 0 && total_cells + count * size <= max_cells) break;
                cout << "Too many ships or not enough space! Try again.\n";
            }
            if (count > 0) {
                fleet.push_back(Ship(size, count));
                total_cells += count * size;
            }
        }
    }

    while (true) {
        Game game(N, fleet);
        cout << "--- Battleship ---\n";
        cout << "1 Show empty board with hints (fleet hidden)\n";
        cout << "2 Show board with ships and hints\n";
        cout << "3 Exit\n";
        cout << "Choose mode: ";
        string mode;
        cin >> mode;

        if (mode == "3") break;

        if (mode == "1" || mode == "2") {
            if (game.prepare_board()) {
                if (mode == "1") game.print_empty_field_with_hints();
                else game.print_field_with_ships_and_hints();
            } else {
                cout << "\nNo solution\n";
            }
            cout << "\nPress Enter to return to menu...";
            cin.ignore(1000000, '\n');
            cin.get();
        } else {
            cout << "\nInvalid choice!\n";
            cout << "\nPress Enter to return to menu...";
            cin.ignore(1000000, '\n');
            cin.get();
        }
    }

    return 0;
}

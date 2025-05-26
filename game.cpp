#include "game.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>

using namespace std;

Game::Game(int size, const vector<Ship>& ships) : N(size), fleet(ships), board(size) {}

Board& Game::get_board() {
    return board;
}

bool Game::prepare_board() {
    vector<Ship> fleet_copy = fleet;
    random_device rd;
    mt19937 rng(rd());
    shuffle(fleet_copy.begin(), fleet_copy.end(), rng);
    return board.solve(fleet_copy, 0, rng);
}

void Game::print_empty_field_with_hints() {
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

void Game::print_field_with_ships_and_hints() {
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

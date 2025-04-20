#include <iostream>
#include <vector>

using namespace std;

struct Ship
{
    int size;
    bool placed;
};

vector<Ship> fleet = {
    {4, false},
    {3, false}, {3, false}, 
    {2, false}, {2, false}, {2, false},
    {1, false}, {1, false}, {1, false}, {1, false}
};

class Board{
private:
    char grid[9][9];
    int filledCells;
};

int main(){

}

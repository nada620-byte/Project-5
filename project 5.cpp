#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

enum class Difficulty { EASY, HARD };

// ======================================================
// Board Class
// ======================================================
class Board{
    private:
    vector<vector<char>> grid;
    int size;

    public:
    Board(int s = 3) : size(s), grid(s, vector<char>(s, ' ')) {}


     bool placeMove(int row, int col, char symbol) {
        if (row >= 0 && row < size && col >= 0 && col < size && grid[row][col] == ' ') {
            grid[row][col] = symbol;
            return true;
        }
        return false;
    }

    char getCell(int row, int col) const {
        if (row >= 0 && row < size && col >= 0 && col < size) {
            return grid[row][col];
        }
        return ' ';
    }

    int getSize() const {
        return size;
    }


};





// ======================================================
// Player Class (Abstract Base)
// ======================================================
class Player







// ======================================================
// HumanPlayer Class (extends Player)
// ======================================================
class HumanPlayer






// ======================================================
// AIPlayer Class (extends Player)
// ======================================================
class AIPlayer








// ======================================================
// Game Class
// ======================================================
class Game









// ======================================================
// Main entry point
// ======================================================
int main() {





}

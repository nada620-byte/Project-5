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
class Player {
protected:
    string name;
    char symbol;

public:
    Player(const string& name, char symbol){
        this->name = name;
		this->symbol = symbol;
    }

    virtual void getMove(Board& board, int& row, int& col) = 0;

    string getName() const {
        return name;
    }

    char getSymbol() const {
        return symbol;
    }

    void setName(const string& newName) {
        name = newName;
    }

    virtual ~Player() {}
};







// ======================================================
// HumanPlayer Class (extends Player)
// ======================================================
class HumanPlayer : public Player {
public:
    HumanPlayer(string name, char symbol) : Player(name, symbol) {}

    void getMove(const Board& board, int& row, int& col) override{
        int r, c;

        while (true) {
            cout <<"\n"<< name << " (" << symbol << "), enter row (1-3) and column (1-3):\n";
			cout << "\t>> ";
			cin >> r >> c;
         
            if (cin.fail()) {
                cout << "Invalid input! Numbers only.\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            if (r < 1 || r > 3 || c < 1 || c > 3) {
                cout << "Invalid coordinates! Choose numbers between 1 and 3.\n";
                continue;
            }

            int internalRow = r - 1;
            int internalCol = c - 1;

            if (board.getCell(internalRow, internalCol) != ' ') {
                cout << "Cell already occupied! Try again.\n";
                continue;
            }

            row = internalRow;
            col = internalCol;
            break;
        }
    }
};






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

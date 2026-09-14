#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

enum class Difficulty
{
    EASY,
    HARD
};

// ======================================================
// Board Class
// ======================================================
class Board
{
private:
    vector<vector<char>> grid;
    int size;

public:
    Board(int s = 3) : size(s), grid(s, vector<char>(s, ' ')) {}

    bool placeMove(int row, int col, char symbol)
    {
        if (row >= 0 && row < size && col >= 0 && col < size && grid[row][col] == ' ')
        {
            grid[row][col] = symbol;
            return true;
        }
        return false;
    }

    char getCell(int row, int col) const
    {
        if (row >= 0 && row < size && col >= 0 && col < size)
        {
            return grid[row][col];
        }
        return ' ';
    }

    int getSize() const
    {
        return size;
    }
    // ======================================================
    // Check Win
    // ======================================================
    bool checkWin(char symbol) const
    {
        if (symbol == ' ')
        {
            return false;
        }

        // Check Rows
        for (int row = 0; row < size; row++)
        {
            bool win = true;

            for (int col = 0; col < size; col++)
            {
                if (grid[row][col] != symbol)
                {
                    win = false;
                    break;
                }
            }

            if (win)
            {
                return true;
            }
        }

        // Check Columns
        for (int col = 0; col < size; col++)
        {
            bool win = true;

            for (int row = 0; row < size; row++)
            {
                if (grid[row][col] != symbol)
                {
                    win = false;
                    break;
                }
            }

            if (win)
            {
                return true;
            }
        }

        // Check Main Diagonal
        bool win = true;

        for (int i = 0; i < size; i++)
        {
            if (grid[i][i] != symbol)
            {
                win = false;
                break;
            }
        }

        if (win)
        {
            return true;
        }

        // Check Anti-Diagonal
        win = true;

        for (int i = 0; i < size; i++)
        {
            if (grid[i][size - 1 - i] != symbol)
            {
                win = false;
                break;
            }
        }

        if (win)
        {
            return true;
        }

        return false;
    }

    // ======================================================
    // Check if Board is Full
    // ======================================================
    bool isFull() const
    {

        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {

                if (grid[row][col] == ' ')
                {
                    return false;
                }
            }
        }

        return true;
    }

    // ======================================================
    // Reset Board
    // ======================================================
    void reset()
    {

        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                grid[row][col] = ' ';
            }
        }
    }

    // ======================================================
    // Force Clear Board
    // ======================================================
    void forceClear()
    {
        grid.assign(size, vector<char>(size, ' '));
    }
};

// ======================================================
// Player Class (Abstract Base)
// ======================================================
class Player
{
protected:
    string name;
    char symbol;

public:
    Player(const string &name, char symbol)
    {
        this->name = name;
        this->symbol = symbol;
    }

    virtual void getMove(Board &board, int &row, int &col) = 0;

    string getName() const
    {
        return name;
    }

    char getSymbol() const
    {
        return symbol;
    }

    void setName(const string &newName)
    {
        name = newName;
    }

    virtual ~Player() {}
};

// ======================================================
// HumanPlayer Class (extends Player)
// ======================================================
class HumanPlayer : public Player
{
public:
    HumanPlayer(string name, char symbol) : Player(name, symbol) {}

    void getMove(const Board &board, int &row, int &col) override
    {
        int r, c;

        while (true)
        {
            cout << "\n"
                 << name << " (" << symbol << "), enter row (1-3) and column (1-3):\n";
            cout << "\t>> ";
            cin >> r >> c;

            if (cin.fail())
            {
                cout << "Invalid input! Numbers only.\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            if (r < 1 || r > 3 || c < 1 || c > 3)
            {
                cout << "Invalid coordinates! Choose numbers between 1 and 3.\n";
                continue;
            }

            int internalRow = r - 1;
            int internalCol = c - 1;

            if (board.getCell(internalRow, internalCol) != ' ')
            {
                cout << "Cell already occupied! Try again.\n";
                continue;
            }

            row = internalRow;
            col = internalCol;
            break;
        }
    }
};


class GameMenu
{
    private:
        string player1Name;
        string player2Name;
        Difficulty difficulty;
        int mode;

    public:

        void showMenu()
        {
            cout << "\n============================\n";
            cout << "       TIC TAC TOE\n";
            cout << "============================\n";
            cout << "1. Player vs Player\n";
            cout << "2. Player vs Computer\n";
            cout << "3. Exit\n";
            cout << "============================\n";
        }

        int getMode()
        {
            while (true)
            {
                showMenu();

                cout << "Choose a mode: ";
                cin >> mode;

                if (cin.fail())
                {
                    cout << "Invalid input! Please enter a number.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }

                if (mode >= 1 && mode <= 3)
                {
                    return mode;
                }

                cout << "Invalid choice! Choose 1, 2, or 3.\n";
            }
        }

        void setupPvP()
        {
            cout << "\n--- Player vs Player ---\n";

            cout << "Enter Player 1 name: ";
            cin >> player1Name;

            cout << "Enter Player 2 name: ";
            cin >> player2Name;
        }

        void setupPvC()
        {
            cout << "\n--- Player vs Computer ---\n";

            cout << "Enter your name: ";
            cin >> player1Name;

            cout << "\nChoose difficulty:\n";
            cout << "1. Easy\n";
            cout << "2. Hard\n";

            int choice;

            while (true)
            {
                cout << "Choose difficulty: ";
                cin >> choice;

                if (cin.fail())
                {
                    cout << "Invalid input! Please enter a number.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }

                if (choice == 1)
                {
                    difficulty = Difficulty::EASY;
                    break;
                }
                else if (choice == 2)
                {
                    difficulty = Difficulty::HARD;
                    break;
                }

                cout << "Invalid choice! Choose 1 or 2.\n";
            }

            player2Name = "Computer";
        }

        string getPlayer1Name() const
        {
            return player1Name;
        }

        string getPlayer2Name() const
        {
            return player2Name;
        }

        Difficulty getDifficulty() const
        {
            return difficulty;
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
{ 
    private : 
        Board board;
        Player *player1;
        Player *player2;
        Player *currentPlayer;
    
    public :
     
    void switchPlayer() //toggles between the players
    {
        if(currentPlayer == player1)
          currentPlayer = player2;
       else 
         currentPlayer = player1;   
    }

    void handleHumanMove(Player* player) // handles one human turn
    {
        int row,col;
        while(true)
        {
            player->getMove(row, col);  //gets the row and the column
            
            if(board.makeMove(row,col,player->getSymbol()))
            {
                break;
            }

            cout << "Invalid move. Please try again,\n" << endl;
        }
    }

    void handleAIMove(Player*player) //handles computer one turn
    {
        cout << "AI is thinking...\n" ;

        int row, col;
        player->getMove(row,col);
        board.makeMove(row , col , player->getSymbol());
    }

    void playRound()
    {
        while(true)
        {
            board.display();

            cout<< currentPlayer->getName()
                << "'s turn("
                << currentPlayer->getSymbol()
                << ")" << endl;
            
                if(dynamic_cast<AIPlayer*>(currentPlayer) != nullptr)
                    handleAIMove(currentPlayer);
                else 
                    handleHumanMove(currentPlayer);
                    
               if(checkGameEnd())
                    break;
                    
              switchPlayer();        
        }   
    }

    // Game end detection, result display, and the replay loop

    bool checkGameEnd() {
        return board.checkWin('X') || board.checkWin('O') || board.isFull();
    }

    void displayResult() const {
        if (board.checkWin(player1->getSymbol())) {
            cout << player1->getName() << " (" << player1->getSymbol() << ") wins!\n";
        } else if (board.checkWin(player2->getSymbol())) {
            cout << player2->getName() << " (" << player2->getSymbol() << ") wins!\n";
        } else {
            cout << "It's a draw!\n";
        }
    }

    void playRound() {
        board.reset();
        currentPlayer = player1;

        while (!checkGameEnd()) {
            board.display();
            cout << currentPlayer->getName() << "'s turn (" << currentPlayer->getSymbol() << ")\n";

            if (dynamic_cast<AIPlayer*>(currentPlayer)) {
                handleAIMove(currentPlayer);
            } else {
                handleHumanMove(currentPlayer);
            }
            switchPlayer();
        }

        board.display();
        displayResult();

        char replay;
        cout << "Play again? (y/n): ";
        cin >> replay;
        if (replay == 'y' || replay == 'Y') {
            playRound();
        }
    }
};

    // ======================================================
    // Main entry point
    // ======================================================
int main() {
    Game game;
    game.start();
    return 0;
}

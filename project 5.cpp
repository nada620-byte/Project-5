#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>

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

    Board(int s = 3) : grid(s, vector<char>(s, ' ')), size(s) {}

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
    // Reset Board (clears the whole board)
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
    // Force Clear a SINGLE cell
    // ======================================================
    void forceClear(int row, int col)
    {
        if (row >= 0 && row < size && col >= 0 && col < size)
        {
            grid[row][col] = ' ';
        }
    }

    void display() const
    {
        cout << "\n    ";
        for (int col = 0; col < size; col++)
        {
            cout << (col + 1) << "   ";
        }
        cout << "\n";

        for (int row = 0; row < size; row++)
        {
            cout << (row + 1) << " ";
            for (int col = 0; col < size; col++)
            {
                cout << "| " << grid[row][col] << " ";
            }
            cout << "|\n";

            cout << "  ";
            for (int col = 0; col < size; col++)
            {
                cout << "+---";
            }
            cout << "+\n";
        }
        cout << endl;
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

    void getMove(Board &board, int &row, int &col) override
    {
        int r, c;
        int size = board.getSize(); // FIX #10: no more hard-coded 1-3

        while (true)
        {
            cout << "\n"
                 << name << " (" << symbol << "), enter row (1-" << size
                 << ") and column (1-" << size << "):\n";
            cout << "\t>> ";
            cin >> r >> c;

            if (cin.eof()) // input stream closed - stop instead of looping forever
            {
                cout << "\nInput ended. Exiting.\n";
                exit(0);
            }

            if (cin.fail())
            {
                cout << "Invalid input! Numbers only.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (r < 1 || r > size || c < 1 || c > size)
            {
                cout << "Invalid coordinates! Choose numbers between 1 and "
                     << size << ".\n";
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

// ======================================================
// AIPlayer Class (extends Player)
// ======================================================
class AIPlayer : public Player
{
private:
    Difficulty difficulty;
    char opponentSymbol;

public:
    AIPlayer(const string &name, char symbol, char oppSymbol, Difficulty diff)
        : Player(name, symbol), difficulty(diff), opponentSymbol(oppSymbol)
    {
    }

    void setDifficulty(Difficulty diff)
    {
        difficulty = diff;
    }

    // ------------------------------------------------------
    // EASY: pick any random empty cell
    // ------------------------------------------------------
    void getRandomMove(const Board &board, int &row, int &col) const
    {
        vector<pair<int, int>> emptyCells;
        int size = board.getSize();

        for (int r = 0; r < size; r++)
        {
            for (int c = 0; c < size; c++)
            {
                if (board.getCell(r, c) == ' ')
                {
                    emptyCells.push_back(make_pair(r, c));
                }
            }
        }

        if (!emptyCells.empty())
        {
            int randomIndex = rand() % emptyCells.size();
            row = emptyCells[randomIndex].first;
            col = emptyCells[randomIndex].second;
        }
    }


    // Scores a finished board: AI win = +10, opponent win = -10, otherwise 0
    int evaluateBoard(const Board &board) const
    {
        if (board.checkWin(symbol))
        {
            return 10;
        }
        if (board.checkWin(opponentSymbol))
        {
            return -10;
        }
        return 0;
    }

    // Recursive minimax search.
    // depth is used so the AI prefers faster wins and slower losses.
    int minimax(Board &board, int depth, bool isMaximizing) const
    {
        if (board.checkWin(symbol))
        {
            return 10 - depth;
        }
        if (board.checkWin(opponentSymbol))
        {
            return depth - 10;
        }
        if (board.isFull())
        {
            return 0;
        }

        int size = board.getSize();

        if (isMaximizing)
        {
            int best = numeric_limits<int>::min();

            for (int r = 0; r < size; r++)
            {
                for (int c = 0; c < size; c++)
                {
                    if (board.getCell(r, c) == ' ')
                    {
                        board.placeMove(r, c, symbol);
                        best = max(best, minimax(board, depth + 1, false));
                        board.forceClear(r, c); // undo trial move
                    }
                }
            }
            return best;
        }
        else
        {
            int best = numeric_limits<int>::max();

            for (int r = 0; r < size; r++)
            {
                for (int c = 0; c < size; c++)
                {
                    if (board.getCell(r, c) == ' ')
                    {
                        board.placeMove(r, c, opponentSymbol);
                        best = min(best, minimax(board, depth + 1, true));
                        board.forceClear(r, c); // undo trial move
                    }
                }
            }
            return best;
        }
    }

    // Tries every empty cell and keeps the one with the highest minimax score
    void getBestMove(Board &board, int &row, int &col) const
    {
        int bestScore = numeric_limits<int>::min();
        int bestRow = -1;
        int bestCol = -1;
        int size = board.getSize();

        for (int r = 0; r < size; r++)
        {
            for (int c = 0; c < size; c++)
            {
                if (board.getCell(r, c) == ' ')
                {
                    board.placeMove(r, c, symbol);
                    int score = minimax(board, 0, false);
                    board.forceClear(r, c); // undo trial move

                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestRow = r;
                        bestCol = c;
                    }
                }
            }
        }

        row = bestRow;
        col = bestCol;
    }

    void getMove(Board &board, int &row, int &col) override
    {
        if (difficulty == Difficulty::EASY)
        {
            getRandomMove(board, row, col);
        }
        else
        {
            getBestMove(board, row, col);
        }
    }

    ~AIPlayer() {}
};

// ======================================================
// GameMenu Class
// ======================================================
class GameMenu
{
private:
    string player1Name;
    string player2Name;
    Difficulty difficulty;
    int mode;

public:
    GameMenu() : difficulty(Difficulty::EASY), mode(0) {}

    void showMenu()
    {
        cout << "\n============================\n";
        cout << "       TIC TAC TOE\n";
        cout << "============================\n";
        cout << "1. Player vs Player\n";
        cout << "2. Player vs Computer (Easy)\n";
        cout << "3. Player vs Computer (Hard)\n";
        cout << "4. Exit\n";
        cout << "============================\n";
    }

    int getMode()
    {
        while (true)
        {
            showMenu();

            cout << "Choose a mode: ";
            cin >> mode;

            if (cin.eof()) // input stream closed - stop instead of looping forever
            {
                cout << "\nInput ended. Exiting.\n";
                exit(0);
            }

            if (cin.fail())
            {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (mode >= 1 && mode <= 4)
            {
                return mode;
            }

            cout << "Invalid choice! Choose a number between 1 and 4.\n";
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

    void setupPvC(Difficulty diff)
    {
        cout << "\n--- Player vs Computer ---\n";

        cout << "Enter your name: ";
        cin >> player1Name;

        difficulty = diff;
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
// Game Class
// ======================================================
class Game
{
private:
    Board board;
    GameMenu menu;
    Player *player1;
    Player *player2;
    Player *currentPlayer;

public:
    // FIX #6: pointers are initialized so they are never garbage
    Game() : board(3), player1(nullptr), player2(nullptr), currentPlayer(nullptr) {}

    // FIX #6: destructor prevents memory leaks
    ~Game()
    {
        clearPlayers();
    }

    // FIX #5: start() now exists and drives the whole program
    void start()
    {
        while (true)
        {
            int mode = menu.getMode();

            if (mode == 4)
            {
                cout << "\nThanks for playing. Goodbye!\n";
                break;
            }

            if (mode == 1)
            {
                setupPvP();
            }
            else if (mode == 2)
            {
                setupPvC(Difficulty::EASY);
            }
            else
            {
                setupPvC(Difficulty::HARD);
            }

            playRound();
        }
    }

private:
    void clearPlayers()
    {
        delete player1;
        delete player2;
        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = nullptr;
    }

    // FIX #9: GameMenu is actually used, and the Player objects get created
    void setupPvP()
    {
        clearPlayers();
        menu.setupPvP();

        player1 = new HumanPlayer(menu.getPlayer1Name(), 'X');
        player2 = new HumanPlayer(menu.getPlayer2Name(), 'O');
        currentPlayer = player1;
    }

    void setupPvC(Difficulty diff)
    {
        clearPlayers();
        menu.setupPvC(diff);

        player1 = new HumanPlayer(menu.getPlayer1Name(), 'X');
        player2 = new AIPlayer(menu.getPlayer2Name(), 'O', 'X', menu.getDifficulty());
        currentPlayer = player1;
    }

    void switchPlayer() // toggles between the players
    {
        if (currentPlayer == player1)
            currentPlayer = player2;
        else
            currentPlayer = player1;
    }

    void handleHumanMove(Player *player) // handles one human turn
    {
        int row, col;

        while (true)
        {
            player->getMove(board, row, col);

            if (board.placeMove(row, col, player->getSymbol()))
            {
                break;
            }

            cout << "Invalid move. Please try again.\n";
        }
    }

    void handleAIMove(Player *player) // handles one computer turn
    {
        cout << player->getName() << " is thinking...\n";

        int row, col;
        player->getMove(board, row, col);
        board.placeMove(row, col, player->getSymbol());
    }

    // ======================================================
    // Game end detection, result display, and the replay loop
    // ======================================================
    bool checkGameEnd()
    {
        return board.checkWin('X') || board.checkWin('O') || board.isFull();
    }

    void displayResult() const
    {
        if (board.checkWin(player1->getSymbol()))
        {
            cout << player1->getName() << " (" << player1->getSymbol() << ") wins!\n";
        }
        else if (board.checkWin(player2->getSymbol()))
        {
            cout << player2->getName() << " (" << player2->getSymbol() << ") wins!\n";
        }
        else
        {
            cout << "It's a draw!\n";
        }
    }


    void playRound()
    {
        char replay = 'y';

        do
        {
            board.reset();
            currentPlayer = player1;

            while (!checkGameEnd())
            {
                board.display();
                cout << currentPlayer->getName() << "'s turn ("
                     << currentPlayer->getSymbol() << ")\n";

                if (dynamic_cast<AIPlayer *>(currentPlayer) != nullptr)
                {
                    handleAIMove(currentPlayer);
                }
                else
                {
                    handleHumanMove(currentPlayer);
                }

                switchPlayer();
            }

            board.display();
            displayResult();

            cout << "Play again? (y/n): ";
            cin >> replay;

        } while (replay == 'y' || replay == 'Y');
    }
};

// ======================================================
// Main entry point
// ======================================================
int main()
{
    srand(static_cast<unsigned int>(time(0)));

    Game game;
    game.start();

    return 0;
}

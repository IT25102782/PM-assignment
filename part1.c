#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Player structure
typedef struct {
    char symbol;
    bool isComputer;
    char name[20];
} Player;

// Function prototypes
char** initializeBoard(int n);
void displayBoard(char** board, int n);
void freeBoard(char** board, int n);
bool getPlayerMove(char** board, int n, Player player);
bool validateMove(char** board, int n, int row, int col);
bool checkWin(char** board, int n, char symbol);
bool checkDraw(char** board, int n);

// Main function
int main() {
    int n;
    char** board;
    Player players[2];
    int currentPlayerIndex = 0;
    int moveCount = 0;
    bool gameWon = false;
    bool gameDraw = false;

    // Display welcome message
    printf("       TIC-TAC-TOE GAME      \n");

    // Setup players
    strcpy(players[0].name, "Player 1");
    players[0].symbol = 'X';
    players[0].isComputer = false;

    strcpy(players[1].name, "Player 2");
    players[1].symbol = 'O';
    players[1].isComputer = false;

    // Get board size from user
    printf("\nEnter board size (3-10): ");
    while (scanf("%d", &n) != 1 || n < 3 || n > 10) {
        printf("Invalid input! Please enter a number between 3 and 10: ");
        while (getchar() != '\n'); // Clear input buffer
    }

    // Initialize the game board
    board = initializeBoard(n);

    // Main game loop
    while (!gameWon && !gameDraw) {
        Player currentPlayer = players[currentPlayerIndex];

        // Display current board
        displayBoard(board, n);

        // Show whose turn it is
        printf("\n%s's turn (Symbol: %c):\n", currentPlayer.name, currentPlayer.symbol);

        bool moveMade = false;

        // Get move from human player
        moveMade = getPlayerMove(board, n, currentPlayer);

        if (moveMade) {
            moveCount++;

            // Check for win
            if (checkWin(board, n, currentPlayer.symbol)) {
                displayBoard(board, n);
                printf("\n %s WINS! \n", currentPlayer.name);
                gameWon = true;
            }
            // Check for draw
            else if (checkDraw(board, n)) {
                displayBoard(board, n);
                printf("\n GAME DRAW! \n");
                gameDraw = true;
            }
            else {
                // Move to next player
                currentPlayerIndex = (currentPlayerIndex + 1) % 2;
            }
        }
    }

    // Clean up memory
    freeBoard(board, n);

    printf("\nThank you for playing!\n");
    return 0;
}

// Function implementations for Part 1
char** initializeBoard(int n) {
    char** board = (char*)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        board[i] = (char*)malloc(n * sizeof(char));
        for (int j = 0; j < n; j++) {
            board[i][j] = ' '; // Empty cell
        }
    }
    return board;
}

void displayBoard(char** board, int n) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf(" %d ", i + 1);
        for (int j = 0; j < n; j++) {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
        if (i < n - 1) {
            printf("   ");
            for (int j = 0; j < n; j++) {
                printf("----");
            }
            printf("-\n");
        }
    }
}

void freeBoard(char** board, int n) {
    for (int i = 0; i < n; i++) {
        free(board[i]);
    }
    free(board);
}

bool getPlayerMove(char** board, int n, Player player) {
    int row, col;
    printf("Enter row and column (1-%d): ", n);
    if (scanf("%d %d", &row, &col) != 2) {
        printf("Invalid input! Please enter two numbers.\n");
        while (getchar() != '\n'); // Clear input buffer
        return false;
    }

    row--;
    col--;

    if (validateMove(board, n, row, col)) {
        board[row][col] = player.symbol;
        return true;
    } else {
        printf("Invalid move! Try again.\n");
        return false;
    }
}

bool validateMove(char** board, int n, int row, int col) {
    if (row < 0 || row >= n || col < 0 || col >= n) {
        printf("Position out of bounds!\n");
        return false;
    }

    if (board[row][col] != ' ') {
        printf("Cell already occupied!\n");
        return false;
    }

    return true;
}

bool checkWin(char** board, int n, char symbol) {
    for (int i = 0; i < n; i++) {
        bool rowWin = true;
        for (int j = 0; j < n; j++) {
            if (board[i][j] != symbol) {
                rowWin = false;
                break;
            }
        }
        if (rowWin) return true;
    }

    for (int j = 0; j < n; j++) {
        bool colWin = true;
        for (int i = 0; i < n; i++) {
            if (board[i][j] != symbol) {
                colWin = false;
                break;
            }
        }
        if (colWin) return true;
    }

    bool diagWin = true;
    for (int i = 0; i < n; i++) {
        if (board[i][i] != symbol) {
            diagWin = false;
            break;
        }
    }
    if (diagWin) return true;

    diagWin = true;
    for (int i = 0; i < n; i++) {
        if (board[i][n - 1 - i] != symbol) {
            diagWin = false;
            break;
        }
    }
    if (diagWin) return true;

    return false;
}

bool checkDraw(char** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

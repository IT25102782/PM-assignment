/*
 * SE1012 Assignment - Tic-Tac-Toe
 * - Part 1: Two human players
 * - Part 2: Human vs Computer
 * - Part 3: Three players
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
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
bool getComputerMove(char** board, int n, Player player);
bool validateMove(char** board, int n, int row, int col);
bool checkWin(char** board, int n, char symbol);
bool checkDraw(char** board, int n);
void logGameState(char** board, int n, Player player, int moveNum, const char* filename);
int selectGameMode();
void setupPlayers(Player* players, int numPlayers, int gameMode);
void clearLogFile(const char* filename);

// Main function
int main() {
    int n;
    char** board;
    int gameMode;
    int numPlayers;
    Player players[3];
    int currentPlayerIndex = 0;
    int moveCount = 0;
    bool gameWon = false;
    bool gameDraw = false;
    
    // Seed random number generator for computer moves
    srand(time(NULL));
    
    // Display welcome message
    printf("       TIC-TAC-TOE GAME      \n");
    
    // Select game mode
    gameMode = selectGameMode();
    
    // Determine number of players based on mode
    if (gameMode == 3) {
        numPlayers = 3;
    } else {
        numPlayers = 2;
    }
    
    // Setup players
    setupPlayers(players, numPlayers, gameMode);
    
    // Get board size from user
    printf("\nEnter board size (3-10): ");
    while (scanf("%d", &n) != 1 || n < 3 || n > 10) {
        printf("Invalid input! Please enter a number between 3 and 10: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    
    // Initialize the game board
    board = initializeBoard(n);
    
    // Clear previous log file
    clearLogFile("game_log.txt");
    
    printf("\nGame started!\n");
    printf("Enter moves as: row column (both starting from 1)\n\n");
    
    // Main game loop
    while (!gameWon && !gameDraw) {
        Player currentPlayer = players[currentPlayerIndex];
        
        // Display current board
        displayBoard(board, n);
        
        // Show whose turn it is
        printf("\n%s's turn (Symbol: %c):\n", currentPlayer.name, currentPlayer.symbol);
        
        bool moveMade = false;
        
        // Get move based on player type
        if (currentPlayer.isComputer) {
            moveMade = getComputerMove(board, n, currentPlayer);
        } else {
            moveMade = getPlayerMove(board, n, currentPlayer);
        }
        
        if (moveMade) {
            moveCount++;
            
            // Log the game state to file
            logGameState(board, n, currentPlayer, moveCount, "game_log.txt");
            
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
                currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
            }
        }
    }
    
    // Clean up memory
    freeBoard(board, n);
    
    printf("\nThank you for playing!\n");
    printf("Game log saved to 'game_log.txt'\n");
    return 0;
}

/*
 * Select game mode
 * Returns: game mode (1, 2, or 3)
 */
int selectGameMode() {
    int mode;
    
    printf("Select Game Mode:\n");
    printf("1. Two Players (Human vs Human)\n");
    printf("2. Single Player (Human vs Computer)\n");
    printf("3. Three Players (Multi-player)\n");
    printf("Enter your choice (1-3): ");
    
    while (scanf("%d", &mode) != 1 || mode < 1 || mode > 3) {
        printf("Invalid choice! Please enter 1, 2, or 3: ");
        while (getchar() != '\n');
    }
    
    return mode;
}

/*
 * Setup players based on game mode
 */
void setupPlayers(Player* players, int numPlayers, int gameMode) {
    char symbols[] = {'X', 'O', 'Z'};
    
    if (gameMode == 1) {
        // Two human players
        strcpy(players[0].name, "Player 1");
        players[0].symbol = 'X';
        players[0].isComputer = false;
        
        strcpy(players[1].name, "Player 2");
        players[1].symbol = 'O';
        players[1].isComputer = false;
    }
    else if (gameMode == 2) {
        // Human vs Computer
        strcpy(players[0].name, "Player 1");
        players[0].symbol = 'X';
        players[0].isComputer = false;
        
        strcpy(players[1].name, "Computer");
        players[1].symbol = 'O';
        players[1].isComputer = true;
    }
    else if (gameMode == 3) {
        // Three players - let user configure
        int computerCount;
        
        printf("\nHow many computer players? (0-2): ");
        while (scanf("%d", &computerCount) != 1 || computerCount < 0 || computerCount > 2) {
            printf("Invalid! Please enter 0, 1, or 2: ");
            while (getchar() != '\n');
        }
        
        // Setup three players
        for (int i = 0; i < 3; i++) {
            players[i].symbol = symbols[i];
            
            if (i < (3 - computerCount)) {
                // Human player
                sprintf(players[i].name, "Player %d", i + 1);
                players[i].isComputer = false;
            } else {
                // Computer player
                sprintf(players[i].name, "Computer %d", i - (3 - computerCount) + 1);
                players[i].isComputer = true;
            }
        }
        
        // Display player configuration
        printf("\nPlayer Configuration:\n");
        for (int i = 0; i < 3; i++) {
            printf("  %s (Symbol: %c) - %s\n", 
                   players[i].name, 
                   players[i].symbol,
                   players[i].isComputer ? "Computer" : "Human");
        }
    }
}

/*
 * Initialize the game board with empty cells
 * Returns: 2D array (dynamically allocated)
 */
char** initializeBoard(int n) {
    char** board = (char**)malloc(n * sizeof(char*));
    
    for (int i = 0; i < n; i++) {
        board[i] = (char*)malloc(n * sizeof(char));
        for (int j = 0; j < n; j++) {
            board[i][j] = ' '; // Empty cell
        }
    }
    
    return board;
}

/*
 * Display the current state of the board
 */
void displayBoard(char** board, int n) {
    printf("\n");
    
    // Print column numbers
    printf("   ");
    for (int i = 1; i <= n; i++) {
        printf(" %2d ", i);
    }
    printf("\n");
    
    // Print top border
    printf("   ");
    for (int i = 0; i < n; i++) {
        printf("----");
    }
    printf("-\n");
    
    // Print each row
    for (int i = 0; i < n; i++) {
        printf("%2d ", i + 1); // Row number
        
        for (int j = 0; j < n; j++) {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
        
        // Print row separator
        printf("   ");
        for (int j = 0; j < n; j++) {
            printf("----");
        }
        printf("-\n");
    }
}

/*
 * Free the dynamically allocated board memory
 */
void freeBoard(char** board, int n) {
    for (int i = 0; i < n; i++) {
        free(board[i]);
    }
    free(board);
}

/*
 * Get and validate human player move
 * Returns: true if move was successful, false otherwise
 */
bool getPlayerMove(char** board, int n, Player player) {
    int row, col;
    
    printf("Enter row and column (1-%d): ", n);
    
    // Read input
    if (scanf("%d %d", &row, &col) != 2) {
        printf("Invalid input! Please enter two numbers.\n");
        while (getchar() != '\n'); // Clear input buffer
        return false;
    }
    
    // Convert to 0-indexed
    row--;
    col--;
    
    // Validate move
    if (validateMove(board, n, row, col)) {
        board[row][col] = player.symbol;
        return true;
    } else {
        printf("Invalid move! Try again.\n");
        return false;
    }
}

/*
 * Generate and execute computer move
 * Returns: true if move was successful
 */
bool getComputerMove(char** board, int n, Player player) {
    int row, col;
    int attempts = 0;
    int maxAttempts = n * n; // Prevent infinite loop
    
    printf("Computer is thinking...\n");
    
    // Find a random empty cell
    do {
        row = rand() % n;
        col = rand() % n;
        attempts++;
        
        if (attempts > maxAttempts) {
            // Should not happen if checkDraw works correctly
            return false;
        }
    } while (board[row][col] != ' ');
    
    // Make the move
    board[row][col] = player.symbol;
    printf("Computer placed '%c' at position (%d, %d)\n", 
           player.symbol, row + 1, col + 1);
    
    return true;
}

/*
 * Validate if a move is legal
 * Returns: true if move is valid, false otherwise
 */
bool validateMove(char** board, int n, int row, int col) {
    // Check if position is within bounds
    if (row < 0 || row >= n || col < 0 || col >= n) {
        printf("Position out of bounds!\n");
        return false;
    }
    
    // Check if cell is already occupied
    if (board[row][col] != ' ') {
        printf("Cell already occupied!\n");
        return false;
    }
    
    return true;
}

/*
 * Check if a player has won
 * Returns: true if player has won, false otherwise
 */
bool checkWin(char** board, int n, char symbol) {
    // Check rows
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
    
    // Check columns
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
    
    // Check main diagonal (top-left to bottom-right)
    bool diagWin = true;
    for (int i = 0; i < n; i++) {
        if (board[i][i] != symbol) {
            diagWin = false;
            break;
        }
    }
    if (diagWin) return true;
    
    // Check anti-diagonal (top-right to bottom-left)
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

/*
 * Check if the game is a draw (board full with no winner)
 * Returns: true if draw, false otherwise
 */
bool checkDraw(char** board, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == ' ') {
                return false; // Found empty cell
            }
        }
    }
    return true; // Board is full
}

/*
 * Log the current game state to a file
 */
void logGameState(char** board, int n, Player player, int moveNum, const char* filename) {
    FILE* file = fopen(filename, "a");
    
    if (file == NULL) {
        printf("Warning: Could not open log file.\n");
        return;
    }
    
    // Write move information
    fprintf(file, " Move %d: %s (%c) \n", moveNum, player.name, player.symbol);
    
    // Write board state
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "| %c ", board[i][j]);
        }
        fprintf(file, "|\n");
    }
    fprintf(file, "\n");
    
    fclose(file);
}

/*
 * Clear the log file at the start of a new game
 */
void clearLogFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, " NEW GAME STARTED \n\n");
        fclose(file);
    }
}

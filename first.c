#include <stdio.h>
#include <stdlib.h>

void initializeBoard(char **board, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = ' ';
}

void displayBoard(char **board, int N) {
    printf("\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf(" %c ", board[i][j]);
            if (j < N - 1) printf("|");
        }
        printf("\n");
        if (i < N - 1) {
            for (int j = 0; j < N; j++) printf("---");
            printf("\n");
        }
    }
    printf("\n");
}

void logBoard(char **board, int N, FILE *logFile) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(logFile, " %c ", board[i][j]);
            if (j < N - 1) fprintf(logFile, "|");
        }
        fprintf(logFile, "\n");
        if (i < N - 1) {
            for (int j = 0; j < N; j++) fprintf(logFile, "---");
            fprintf(logFile, "\n");
        }
    }
    fprintf(logFile, "\n");
}

int isValidMove(char **board, int row, int col, int N) {
    return row >= 0 && row < N && col >= 0 && col < N && board[row][col] == ' ';
}

int checkWin(char **board, int N, char player) {
    // Check rows
    for (int i = 0; i < N; i++) {
        int win = 1;
        for (int j = 0; j < N; j++) {
            if (board[i][j] != player) { win = 0; break; }
        }
        if (win) return 1;
    }
    // Check columns
    for (int j = 0; j < N; j++) {
        int win = 1;
        for (int i = 0; i < N; i++) {
            if (board[i][j] != player) { win = 0; break; }
        }
        if (win) return 1;
    }
    // Check main diagonal
    int win = 1;
    for (int i = 0; i < N; i++) {
        if (board[i][i] != player) { win = 0; break; }
    }
    if (win) return 1;

    // Check anti-diagonal
    win = 1;
    for (int i = 0; i < N; i++) {
        if (board[i][N - i - 1] != player) { win = 0; break; }
    }
    return win;
}

int checkDraw(char **board, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

int main() {
    int N;
    printf("Enter board size (3-10): ");
    scanf("%d", &N);

    if (N < 3 || N > 10) {
        printf("Invalid size!\n");
        return 1;
    }

    // Allocate dynamic memory
    char **board = (char **)malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++)
        board[i] = (char *)malloc(N * sizeof(char));

    initializeBoard(board, N);

    FILE *logFile = fopen("tictactoe_log.txt", "w");
    if (!logFile) {
        printf("Error opening log file!\n");
        return 1;
    }

    int row, col;
    char currentPlayer = 'X';
    while (1) {
        displayBoard(board, N);
        printf("Player %c, enter your move (row col): ", currentPlayer);
        scanf("%d %d", &row, &col);
        row--; col--; // Adjust to 0-based index

        if (!isValidMove(board, row, col, N)) {
            printf("Invalid move! Try again.\n");
            continue;
        }

        board[row][col] = currentPlayer;
        logBoard(board, N, logFile);

        if (checkWin(board, N, currentPlayer)) {
            displayBoard(board, N);
            printf("Player %c wins!\n", currentPlayer);
            break;
        }
        if (checkDraw(board, N)) {
            displayBoard(board, N);
            printf("It's a draw!\n");
            break;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    fclose(logFile);

    for (int i = 0; i < N; i++) free(board[i]);
    free(board);

    return 0;
}

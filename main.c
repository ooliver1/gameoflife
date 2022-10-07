#include <stdio.h>
#include <string.h>

// Cross-platform sleep function
#ifdef _WINDOWS
#include <windows.h>
#define Clear() system("cls")
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000000)
#define Clear() system("clear")
#endif

const int WIDTH = 30;
const int HEIGHT = 10;
const float DELAY = 0.5;
const char *BLACK_TILE = "\u2B1B"; // ⬛
const char *WHITE_TILE = "\u2B1C"; // ⬜
const char *ANSII_CLEAR = "\033[2J";
// Coordinates (top left is the origin) of starting live cells, the "seed".
#define STARTING_AMOUNT 10
const int START[STARTING_AMOUNT][2] = {{2, 1}, {3, 1}, {3, 2}, {2, 3}, {3, 3},
                                       {3, 4}, {4, 4}, {5, 4}, {4, 2}, {5, 3}};

void print_board(int (*board_ptr)[HEIGHT][WIDTH]) {
    // Terminal clear.
    Clear();

    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            if ((*board_ptr)[x][y] == 0) {
                printf("%s", BLACK_TILE);
            } else {
                printf("%s", WHITE_TILE);
            }
        }

        printf("\n");
    }
}

int main() {
    int board[HEIGHT][WIDTH];

    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            board[x][y] = 0;
        }
    }

    // Add starting seed.
    for (int i = 0; i < STARTING_AMOUNT; i++) {
        board[START[i][1]][START[i][0]] = 1;
    }

    for (;;) {
        print_board(&board);

        // Create a new board array, to place the new states in.
        int new_board[HEIGHT][WIDTH];

        // Make a new board, based on the old one.
        memcpy(&new_board, &board, sizeof(board));

        for (int x = 0; x < HEIGHT; x++) {
            for (int y = 0; y < WIDTH; y++) {
                int alive_neighbors = 0;

                // Collect top left.
                if (y != 0 && x != 0) {
                    alive_neighbors += board[x - 1][y - 1];
                }

                // Collect top.
                if (y != 0) {
                    alive_neighbors += board[x][y - 1];
                }

                // Collect top right.
                if (y != 0 && x != (HEIGHT - 1)) {
                    alive_neighbors += board[x + 1][y - 1];
                }

                // Collect right.
                if (x != (HEIGHT - 1)) {
                    alive_neighbors += board[x + 1][y];
                }

                // Collect bottom right.
                if (x != (HEIGHT - 1) && y != (WIDTH - 1)) {
                    alive_neighbors += board[x + 1][y + 1];
                }

                // Collect bottom.
                if (y != (WIDTH - 1)) {
                    alive_neighbors += board[x][y + 1];
                }

                // Collect bottom left.
                if (y != (WIDTH - 1) && x != 0) {
                    alive_neighbors += board[x - 1][y + 1];
                }

                // Collect left.
                if (x != 0) {
                    alive_neighbors += board[x - 1][y];
                }

                if (alive_neighbors < 2) {
                    // Less than 2 dies.
                    new_board[x][y] = 0;
                } else if (alive_neighbors > 3) {
                    // More than 3 dies.
                    new_board[x][y] = 0;
                } else if (alive_neighbors == 3) {
                    // 3 reproduction.
                    new_board[x][y] = 1;
                }
            }
        }

        // Copy back the new boamemcpy(&new_board, &board, sizeof(board));rd
        // to the original to re-use.
        memcpy(&board, &new_board, sizeof(new_board));

        // Allow the viewer to see the board.
        Sleep(DELAY);
    }
}

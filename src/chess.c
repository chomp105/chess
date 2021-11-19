#include <stdio.h>
#include <stdlib.h>
#include "chess.h"
#include "logic.h"

void print_board(int board[8][8][2], char pieces[2][16][4]) {
    // loops through board and prints pieces to screen along with spaces
    for (int i = 0; i < 8; i++) {
        if (i != 0) {
            printf("├───┼───┼───┼───┼───┼───┼───┼───┤\n");
        } else {
            printf("┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
        }
        for (int j = 0; j < 8; j++) {
            if (board[i][j][0] != -1) {
                printf("│ %s ", pieces[board[i][j][0]][board[i][j][1]]);
            } else {
                printf("│   ");
            }
        }
        printf("│\n");
    }
    printf("└───┴───┴───┴───┴───┴───┴───┴───┘\n");
}

int move(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2][2], int player, int *game, int enpas[5]) {
    if (enpas[0] == 1 && board[sy][sx][1] == 5 && ex == enpas[1] && ey == enpas[2]) {
        transfer_piece(board, enpas[3], enpas[4], enpas[1], enpas[2]);
        enpas[0] = 0;
    }
    if (eval(board, sx, sy, ex, ey)) {
        // "checks" for check... get it? check and check... lol
        if (!check(board, sx, sy, ex, ey, king[player])) {
            transfer_piece(board, sx, sy, ex, ey);
            if ((ey == 0 || ey == 7) && board[ey][ex][1] == 5) {
                int choice;
                system("clear");
                printf("[0] rook\n[1] knight\n[2] bishop\n[3] queen\nWhich piece would you like to promote to: ");
                scanf("%d", &choice);
                if (choice > 3 || choice < 0) {
                    choice = 3;
                }
                board[ey][ex][1] = choice;
            } else if (board[ey][ex][1] == 5 && abs(ey - sy) == 2) {
                // set en passant data
                enpas[0] = 1;
                enpas[1] = sx;
                enpas[2] = sy + (1 * (ey - sy) / abs(ey - sy));
                enpas[3] = ex;
                enpas[4] = ey;
            }
            if (sx == king[player][1] && sy == king[player][0]) {
                king[player][0] = ey;
                king[player][1] = ex;
            }
        } else {
            return 0;
        }
        // checks for checkmate and stops game if it returns true
        if (checkmate(board, king, player)) {
            system("clear");
            *game = 0;
        }
        return 1;
    } else if (enpas[0] == 0) {
        transfer_piece(board, enpas[2], enpas[1], enpas[4], enpas[3]);
        enpas[0] = 1;
    }
    return 0;
}
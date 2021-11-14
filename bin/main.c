#include <stdio.h>
#include <stdlib.h>
#include "chess.h"

int main(void) { //TODO: implement advanced moves (castling, en passant, queening)
    // game board with pieces and blank spaces
    int board[8][8][2] = {
            {{ 1, 0},{ 1, 1},{ 1, 2},{ 1, 3},{ 1, 4},{ 1, 2},{ 1, 1},{ 1, 0}},
            {{ 1, 5},{ 1, 5},{ 1, 5},{ 1, 5},{ 1, 5},{ 1, 5},{ 1, 5},{ 1, 5}},
            {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
            {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
            {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
            {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
            {{ 0, 5},{ 0, 5},{ 0, 5},{ 0, 5},{ 0, 5},{ 0, 5},{ 0, 5},{ 0, 5}},
            {{ 0, 0},{ 0, 1},{ 0, 2},{ 0, 3},{ 0, 4},{ 0, 2},{ 0, 1},{ 0, 0}}
    };
    int king[2][2] = {
            {7, 4},
            {0, 4}
    };
    char pieces[2][16][4] = {
            {"\u265c", "\u265e", "\u265d", "\u265b", "\u265a", "\u265f"},
            {"\u2656", "\u2658", "\u2657", "\u2655", "\u2654", "\u2659"}
    };
    int player = 0;
    int game = 1;
    // en passant data
    int enpas[5];
    // game loop

    while(game) {
    	system("clear");
        print_board(board, pieces);
        printf("player %d: ", player + 1);
        // input for the movement
        int sx, sy, ex, ey;
        scanf("%d%d%d%d", &sx, &sy, &ex, &ey);
        sx--;sy--;ex--;ey--;
        if (board[sy][sx][0] != player) {
            continue;
        } else if (move(board, sx, sy, ex, ey, king, player, &game, enpas)) {
            player = !player;
        }
    }
    print_board(board, pieces);
    printf("Checkmate\n");
    return 0;
}
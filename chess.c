#include <stdio.h>
#include <stdlib.h>

#include "logic.h"

void print_board(int board[8][8][2], char pieces[2][16][4]);
int move(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2][2], int player, int *game);
int valid(int input);

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
        } else if (move(board, sx, sy, ex, ey, king, player, &game)) {
            player = !player;
        }
    }
    print_board(board, pieces);
    printf("Checkmate\n");
    return 0;
}

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

int move(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2][2], int player, int *game) {
    if (eval(board, sx, sy, ex, ey) && valid(ex) && valid(ey)) {
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
    }
    return 0;
}

int valid(int input) {
    return (input < 8 && input >= 0);
}

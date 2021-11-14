#ifndef CHESS_H
#define CHESS_H

void print_board(int board[8][8][2], char pieces[2][16][4]);
int move(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2][2], int player, int *game, int enpas[5]);

#endif //CHESS_H

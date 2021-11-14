#ifndef LOGIC_H
#define LOGIC_H

int checkmate(int board[8][8][2], int king[2][2], int player);
int check(int board[8][8][2], int sx, int sy, int ex, int ey, const int king[2]);
void transfer_piece(int board[8][8][2], int sx, int sy, int ex, int ey);
int eval(int board[8][8][2], int sx, int sy, int ex, int ey);
int check_rook(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist);
int check_bishop(int board[8][8][2], int sx, int sy, int xdist, int ydist);
int check_queen(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist);
int check_pawn(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist);

#endif //LOGIC_H

#include <stdio.h>
#include <stdlib.h>

void print_board(int board[8][8][2], char pieces[2][16][4]);
int move(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2][2], int player, int *game);
int checkmate(int board[8][8][2], int king[2][2], int player);
int check(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2]);
void transfer_piece(int board[8][8][2], int sx, int sy, int ex, int ey);
int eval(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2]);
int check_rook(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist);
int check_knight(int xdist, int ydist);
int check_bishop(int board[8][8][2], int sx, int sy, int xdist, int ydist);
int check_queen(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist);
int check_king(int ex, int ey, int xdist, int ydist, int king[2]);
int check_pawn(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist);

int main(void) { //TODO: implement advanced moves (castling, en passant, queening)
    // game board with pieces and blank spaces
    int board[8][8][2] = {
            {{1,0},{1,1},{1,2},{1,3},{1,4},{1,2},{1,1},{1,0}},
            {{1,5},{1,5},{1,5},{1,5},{1,5},{1,5},{1,5},{1,5}},
            {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
            {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
            {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
            {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}},
            {{0,5},{0,5},{0,5},{0,5},{0,5},{0,5},{0,5},{0,5}},
            {{0,0},{0,1},{0,2},{0,3},{0,4},{0,2},{0,1},{0,0}}
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
    return 0;
}

void print_board(int board[8][8][2], char pieces[2][16][4]) {
    // loops through board and prints pieces to screen along with spaces
    for (int i = 0; i < 8; i++) {
        printf("+---+---+---+---+---+---+---+---+\n");
        for (int j = 0; j < 8; j++) {
            if (board[i][j][0] != -1) {
                printf("| %s ", pieces[board[i][j][0]][board[i][j][1]]);
            } else {
                printf("|   ");
            }
        }
        printf("|\n");
    }
    printf("+---+---+---+---+---+---+---+---+\n");
}

int move(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2][2], int player, int *game) {
    if (eval(board, sx, sy, ex, ey, king[player])) {
        // "checks" for check... get it? check and check... lol
        if (!check(board, sx, sy, ex, ey, king[player])) {
            transfer_piece(board, sx, sy, ex, ey);
	    if (sx == king[player][1] && sy == king[player][0]) {
                king[player][0] = ey;
                king[player][1] = ex;
            }
        } else {
            return 0;
        }
        // checks for checkmate and stops game if it returns true
        if (checkmate(board, king, player)) {
            printf("Checkmate\n");
            *game = 0;
        }
        return 1;
    }
    return 0;
}

int checkmate(int board[8][8][2], int king[2][2], int player) {
    // loops through every spot on the board and for each spot, if it is an enemy of the current player,
    // it checks if it can move somewhere that will prevent check for the enemy king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[j][i][0] != -1 && board[j][i][0] != board[king[player][0]][king[player][1]][0]) {
                for (int k = 0; k < 8; k++) {
                    for (int l = 0; l < 8; l++) {
                        // if there is a move which is safe, it returns false
                        if (!(check(board, i, j, k, l, king[!player]))) {
			     return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int check(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2]) {
    int nboard[8][8][2];
    // copies board to new board to perform check function
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            nboard[i][j][0] = board[i][j][0];
            nboard[i][j][1] = board[i][j][1];
        }
    }
    if (eval(board, sx, sy, ex, ey, king) && !((sx == ex) && (sy == ey)) && board[ey][ex][0] != board[sy][sx][0]) {
        // moves piece on the new board and sets kingx and kingy to their correct positions
        transfer_piece(nboard, sx, sy, ex, ey);
	if (sx == king[1] && sy == king[0]) {
            king[0] = ey;
       	    king[1] = ex;
        }
        int kingx = king[1];
        int kingy = king[0];
        // loops through every spot on the board and checks to see if it can attack the king
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (nboard[j][i][0] != -1 && nboard[j][i][0] != nboard[kingy][kingx][0]) {
                    if (eval(nboard, i, j, kingx, kingy, king)) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
    return 1;
}

void transfer_piece(int board[8][8][2], int sx, int sy, int ex, int ey) {
    // checks that a move has occurred and that the end point is not a team member
    if (!((sx == ex) && (sy == ey)) && board[ey][ex][0] != board[sy][sx][0]) {
        // moves piece info to specified area
        board[ey][ex][0] = board[sy][sx][0];
        board[ey][ex][1] = board[sy][sx][1];
        // clears out previous position
        board[sy][sx][0] = -1;
        board[sy][sx][1] = -1;
    }
}

int eval(int board[8][8][2], int sx, int sy, int ex, int ey, int king[2]) {
    // finds piece type and team from starting position
    int p_type = board[sy][sx][1];
    // x and y distance are the end point minus the start point
    int xdist = ex - sx;
    int ydist = ey - sy;
    // checks player type to see which conditions to check
    switch(p_type) {
        case 0:
            return check_rook(board, sx, sy, ex, ey, xdist, ydist);
        case 1:
            return check_knight(xdist, ydist);
        case 2:
            return check_bishop(board, sx, sy, xdist, ydist);
        case 3:
            return check_queen(board, sx, sy, ex, ey, xdist, ydist);
        case 4:
            return check_king(ex, ey, xdist, ydist, king);
        case 5:
            return check_pawn(board, sx, sy, ex, ey, xdist, ydist);
        default:
            return 0;
    }
}

int check_rook(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist) {
    // makes sure piece moves in correct direction
    if ((sx != ex) != (sy != ey)) {
        // loops through all the spaces the piece is going through and makes sure nothing is in its path
        // see bishop comments for explanation of logic
        if (abs(xdist) > 0) {
            for (int i = 1; i < abs(xdist); i++) {
                if (board[sy][sx + (i * xdist / abs(xdist))][0] != -1) {
                    return 0;
                }
            }
        } else {
            for (int i = 1; i < abs(ydist); i++) {
                if (board[sy + (i * ydist / abs(ydist))][sx][0] != -1) {
                    return 0;
                }
            }
        }
        return 1;
    }
    return 0;
}

int check_knight(int xdist, int ydist) {
    // checks if the moves are in the correct pattern for a knight (L shape)
    //if (((abs(xdist) == 2) && (abs(ydist) == 1)) || ((abs(xdist) == 1) && (abs(ydist) == 2))) {
    if ((abs(xdist) + abs(ydist) == 3) && (abs(xdist) == 1 || abs(ydist) == 1)) {
        return 1;
    }
    return 0;
}

int check_bishop(int board[8][8][2], int sx, int sy, int xdist, int ydist) {
    // makes sure that bishop is moving in a diagonal
    if (abs(xdist) == abs(ydist)) {
        // checks if there is something in the way of the bishop
        // it loops through, and it multiplies "i" and dist and divides it by abs(dist). this means that there only has to be one for loop
        // if the bishop moves in a negative x direction then "i" will become a negative due to the equation
        for (int i = 1; i < abs(xdist); i++) {
            if (board[sy + (i * ydist / abs(ydist))][sx + (i * xdist / abs(xdist))][0] != -1) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int check_queen(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist) {
    // checks if the queen is moving like a rook or bishop and uses the according function
    if ((sx != ex) != (sy != ey)) {
        return check_rook(board, sx, sy, ex, ey, xdist, ydist);
    } else if (abs(xdist) == abs(ydist)) {
        return check_bishop(board, sx, sy, xdist, ydist);
    }
    return 0;
}

int check_king(int ex, int ey, int xdist, int ydist, int king[2]) {
    // checks that king is only moving one space away
    if (abs(xdist) < 2 && abs(ydist) < 2) {
        return 1;
    }
    return 0;
}

int check_pawn(int board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist) {
    // checks if pawn is moving forward once, twice for first move, or attacking
    // !(ydist + abs(ydist)) == !board[sy][sx][0] tests to see if the pawn is moving in the right direction.
    // ydist + abs(ydist) makes sure that the number is either 0 or positive
    if (((((xdist == 0 && abs(ydist) == 1) || (xdist == 0 && abs(ydist) == 2 && (sy == 1 || sy == 6))) && board[ey][ex][0] == -1) ||
         ((xdist == 1 && ydist == 1) && board[ey][ex][0] != -1)) && !(ydist + abs(ydist)) == !board[sy][sx][0]) {
        return 1;
    }
    return 0;
}


#include <stdio.h>
#include <stdlib.h>

void print_board(char board[8][8][2]);
void move(char board[8][8][2], int sx, int sy, int ex, int ey, int king[2][2], int player);
int checkmate(char board[8][8][2], int king[2][2], int player);
int check(char board[8][8][2], int sx, int sy, int ex, int ey, int king[2]);
void transfer_piece(char board[8][8][2], int sx, int sy, int ex, int ey, int king[2]);
int eval(char board[8][8][2], int sx, int sy, int ex, int ey, int king[2]);
int check_rook(char board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist);
int check_knight(int xdist, int ydist);
int check_bishop(char board[8][8][2], int sx, int sy, int xdist, int ydist);
int check_queen(char board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist);
int check_king(int ex, int ey, int xdist, int ydist, int king[2]);
int check_pawn(char board[8][8][2], int sy, int ex, int ey, int xdist, int ydist);

/*
 *
 * TODO: Document code, Test and refactor code, make UI, send to beta testers
 *
 */

int main(void) {
    // game board with pieces and blank spaces
    char board[8][8][2] = {
            {"br","bn","bb","bq","bk","bb","bn","bb"},
            {"  ","bp","bp","bp","bp","bp","bp","bp"},
            {"  ","  ","  ","  ","  ","  ","  ","  "},
            {"  ","  ","  ","  ","  ","  ","  ","  "},
            {"  ","  ","  ","  ","  ","  ","  ","  "},
            {"  ","  ","  ","  ","  ","  ","  ","  "},
            {"wp","wp","wp","wp","wp","wp","wp","wp"},
            {"wr","wn","wb","wq","wk","wb","wn","wr"}
    };
    int king[2][2] = {
            {0, 4},
            {7, 4}
    };
    int player = 0;
    // game loop
    int sx, sy, ex, ey;
    while(1) {
        print_board(board);
        scanf("%d%d%d%d", &sx, &sy, &ex, &ey);
        move(board, sx, sy, ex, ey, king, player);
        system("clear");0
    }
    return 0;
}

void print_board(char board[8][8][2]) {
    // loops through board and prints pieces to screen along with spaces
    for (int i = 0; i < 8; i++) {
        printf("+---+---+---+---+---+---+---+---+\n");
        for (int j = 0; j < 8; j++) {
            if (board[i][j][0] == 'w') {
                switch (board[i][j][1]) {
                    case 'r':
                        printf("| \u265c ");
                        break;
                    case 'n':
                        printf("| \u265e ");
                        break;
                    case 'b':
                        printf("| \u265d ");
                        break;
                    case 'q':
                        printf("| \u265b ");
                        break;
                    case 'k':
                        printf("| \u265b ");
                        break;
                    case 'p':
                        printf("| \u265f ");
                        break;
                }
            } else if (board[i][j][0] == 'b') {
                switch (board[i][j][1]) {
                    case 'r':
                        printf("| \u2656 ");
                        break;
                    case 'n':
                        printf("| \u2658 ");
                        break;
                    case 'b':
                        printf("| \u2657 ");
                        break;
                    case 'q':
                        printf("| \u2655 ");
                        break;
                    case 'k':
                        printf("| \u2654 ");
                        break;
                    case 'p':
                        printf("| \u2659 ");
                        break;
                }
            } else {
                printf("|   ");
            }
        }
        printf("|\n");
    }
    printf("+---+---+---+---+---+---+---+---+\n");
}

void move(char board[8][8][2], int sx, int sy, int ex, int ey, int king[2][2], int player) {
    if (eval(board, sx, sy, ex, ey, king[player])) {
        // "checks" for check... get it? check and check... lol
        //if (!check(board, sx, sy, ex, ey, king)) {
        transfer_piece(board, sx, sy, ex, ey, king[player]);
        //}
        if (checkmate(board, king, player)) {
            printf("Checkmate");
        }
    }
}

int checkmate(char board[8][8][2], int king[2][2], int player) {
    // loops through every spot on the board and for each spot, if it is an enemy of the current player,
    // it checks if it can move somewhere that will prevent check for the enemy king
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[j][i][0] != ' ' && board[j][i][0] != board[king[player][1]][king[player][0]][0]) {
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

int check(char board[8][8][2], int sx, int sy, int ex, int ey, int king[2]) {
    char nboard[8][8][2];
    // copies board to new board to perform check function
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            nboard[i][j][0] = board[i][j][0];
            nboard[i][j][1] = board[i][j][1];
        }
    }
    if (eval(board, sx, sy, ex, ey, king) && !((sx == ex) && (sy == ey)) && board[ey][ex][0] != board[sy][sx][0]) {
        // moves piece on the new board and sets kingx and kingy to their correct positions
        transfer_piece(nboard, sx, sy, ex, ey, king);
        int kingx = king[1];
        int kingy = king[0];
        // loops through every spot on the board and checks to see if it can attack the king
        //printf("## %d %c ##", eval(nboard, 0, 6, 0, 2, king), nboard[6][0][1]);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (nboard[j][i][0] != ' ' && nboard[j][i][0] != nboard[kingy][kingx][0]) {
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

void transfer_piece(char board[8][8][2], int sx, int sy, int ex, int ey, int king[2]) {
    // checks that a move has occurred and that the end point is not a team member
    if (!((sx == ex) && (sy == ey)) && board[ey][ex][0] != board[sy][sx][0]) {
        if (eval(board, sx, sy, ex, ey, king)) {
            // moves piece info to specified area
            board[ey][ex][0] = board[sy][sx][0];
            board[ey][ex][1] = board[sy][sx][1];
            // clears out previous position
            board[sy][sx][0] = ' ';
            board[sy][sx][1] = ' ';
        }
    }
}

int eval(char board[8][8][2], int sx, int sy, int ex, int ey, int king[2]) {
    // finds piece type and team from starting position
    char p_type = board[sy][sx][1];
    // x and y distance are the end point minus the start point
    int xdist = ex - sx;
    int ydist = ey - sy;
    // checks player type to see which conditions to check
    switch(p_type) {
        case 'r':
            return check_rook(board, sx, sy, ex, ey, xdist, ydist);
        case 'n':
            return check_knight(xdist, ydist);
        case 'b':
            return check_bishop(board, sx, sy, xdist, ydist);
        case 'q':
            return check_queen(board, sx, sy, ex, ey, xdist, ydist);
        case 'k':
            return check_king(ex, ey, xdist, ydist, king);
        case 'p':
            return check_pawn(board, sy, ex, ey, xdist, ydist);
        default:
            return 0;
    }
}

int check_rook(char board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist) {
    // makes sure piece moves in correct direction
    if ((sx != ex) != (sy != ey)) {
        // loops through all the spaces the piece is going through and makes sure nothing is in its path
        // see bishop comments for explanation of logic
        if (abs(xdist) > 0) {
            for (int i = 1; i < abs(xdist); i++) {
                if (board[sy][sx + (i * xdist / abs(xdist))][0] != ' ') {
                    return 0;
                }
            }
        } else {
            for (int i = 1; i < abs(ydist); i++) {
                if (board[sy + (i * ydist / abs(ydist))][sx][0] != ' ') {
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

int check_bishop(char board[8][8][2], int sx, int sy, int xdist, int ydist) {
    // makes sure that bishop is moving in a diagonal
    if (abs(xdist) == abs(ydist)) {
        // checks if there is something in the way of the bishop
        // it loops through, and it multiplies "i" and dist and divides it by abs(dist). this means that there only has to be one for loop
        // if the bishop moves in a negative x direction then "i" will become a negative due to the equation
        for (int i = 1; i < abs(xdist); i++) {
            if (board[sy + (i * ydist / abs(ydist))][sx + (i * xdist / abs(xdist))][0] != ' ') {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int check_queen(char board[8][8][2], int sx, int sy, int ex, int ey, int xdist, int ydist) {
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
        king[0] = ey;
        king[1] = ex;
        return 1;
    }
    return 0;
}

int check_pawn(char board[8][8][2], int sy, int ex, int ey, int xdist, int ydist) {
    // checks if pawn is moving forward once, twice for first move, or attacking
    // TODO: fix pawn function (it is just really bad) (idk what i was thinking)
    if (((xdist == 0 && (abs(ydist == 1) || (abs(ydist == 2)) && (sy == 1 || sy == 6)))
         || ((xdist == 1 && ydist == 1) && board[ey][ex][0] != ' '))) {
        return 1;
    }
    return 0;
}

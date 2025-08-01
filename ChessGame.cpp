#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <cstring>
using namespace std;

char board[8][8];
char play[4];
int rowFrom, colFrom, rowTo, colTo;
char capturedByWhite[32];
char capturedByBlack[32];
int whiteCapturedCount = 0;
int blackCapturedCount = 0;


void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void setupBoard() {
    memset(board, '.', sizeof(board));
    for (int c = 0; c < 8; c++) {
        board[1][c] = 'P';
        board[6][c] = 'p';
    }
    board[0][0] = 'E'; board[0][7] = 'E';
    board[7][0] = 'e'; board[7][7] = 'e';
    board[0][1] = 'H'; board[0][6] = 'H';
    board[7][1] = 'h'; board[7][6] = 'h';
    board[0][2] = 'C'; board[0][5] = 'C';
    board[7][2] = 'c'; board[7][5] = 'c';
    board[0][3] = 'K'; board[7][3] = 'k';
    board[0][4] = 'Q'; board[7][4] = 'q';
}

int checkTurn() {
    char piece = board[rowFrom][colFrom];

    // Prevent capturing own piece
    if (board[rowTo][colTo] != '.') {
        if ((isupper(piece) && isupper(board[rowTo][colTo])) ||
            (islower(piece) && islower(board[rowTo][colTo]))) {
            cout << "Invalid! Cannot capture own piece." << endl;
            Sleep(1000);
            return 0;
        }
    }

    // White pawn (P)
    if (piece == 'P') {
        if (colFrom == colTo) {
            if (rowTo == rowFrom + 1 && board[rowTo][colTo] == '.') return 1;
            if (rowFrom == 1 && rowTo == rowFrom + 2 && board[rowFrom + 1][colFrom] == '.' && board[rowTo][colTo] == '.') return 1;
        }
        return 0;
    }

    // Black pawn (p)
    if (piece == 'p') {
        if (colFrom == colTo) {
            if (rowTo == rowFrom - 1 && board[rowTo][colTo] == '.') return 1;
            if (rowFrom == 6 && rowTo == rowFrom - 2 && board[rowFrom - 1][colFrom] == '.' && board[rowTo][colTo] == '.') return 1;
        }
        return 0;
    }

    // Elephant - E or e Move in + shape
    if (piece == 'E' || piece == 'e') {
        if (rowFrom != rowTo && colFrom != colTo) return 0;
        if (rowFrom == rowTo) {
            int step = (colTo > colFrom) ? 1 : -1;
            for (int c = colFrom + step; c != colTo; c += step)
                if (board[rowFrom][c] != '.') return 0;
            return 1;
        }
        if (colFrom == colTo) {
            int step = (rowTo > rowFrom) ? 1 : -1;
            for (int r = rowFrom + step; r != rowTo; r += step)
                if (board[r][colFrom] != '.') return 0;
            return 1;
        }
        return 0;
    }

    // Horse - H or h (L shape)
    if (piece == 'H' || piece == 'h') {
        int dr = rowTo - rowFrom;
        int dc = colTo - colFrom;
        if ((dr == 2 || dr == -2) && (dc == 1 || dc == -1)) return 1;
        if ((dr == 1 || dr == -1) && (dc == 2 || dc == -2)) return 1;
        return 0;
    }

    // Camel - C or c move in X sahpe
    if (piece == 'C' || piece == 'c') {
        int dRow = rowTo - rowFrom;
        int dCol = colTo - colFrom;
        if (dRow == 0 || dCol == 0 || dRow + dCol == 0) return 0;
        if (dRow == dCol || dRow == -dCol) {
            int rStep = (dRow > 0) ? 1 : -1;
            int cStep = (dCol > 0) ? 1 : -1;
            int r = rowFrom + rStep;
            int c = colFrom + cStep;
            while (r != rowTo && c != colTo) {
                if (board[r][c] != '.') return 0;
                r += rStep;
                c += cStep;
            }
            return 1;
        }
        return 0;
    }

    // King - K or k move 1 step up,down,right,left,diagnol
    if (piece == 'K' || piece == 'k') {
        int dr = rowTo - rowFrom;
        int dc = colTo - colFrom;
        if (dr >= -1 && dr <= 1 && dc >= -1 && dc <= 1) return 1;
        return 0;
    }

    // Queen - Q or q
    if (piece == 'Q' || piece == 'q') {
        int dRow = rowTo - rowFrom;
        int dCol = colTo - colFrom;

        if (rowFrom == rowTo || colFrom == colTo) {
            int stepR = (dRow > 0) ? 1 : (dRow < 0 ? -1 : 0);
            int stepC = (dCol > 0) ? 1 : (dCol < 0 ? -1 : 0);
            int r = rowFrom + stepR, c = colFrom + stepC;
            while (r != rowTo || c != colTo) {
                if (board[r][c] != '.') return 0;
                r += stepR;
                c += stepC;
            }
            return 1;
        }

        if (dRow == dCol || dRow == -dCol) {
            int rStep = (dRow > 0) ? 1 : -1;
            int cStep = (dCol > 0) ? 1 : -1;
            int r = rowFrom + rStep, c = colFrom + cStep;
            while (r != rowTo && c != colTo) {
                if (board[r][c] != '.') return 0;
                r += rStep;
                c += cStep;
            }
            return 1;
        }

        return 0;
    }

    return 0;
}


void updateBoard()
{
    char fromPiece = board[rowFrom][colFrom];
    char toPiece = board[rowTo][colTo];

    if (toPiece != '.') {
        string attacker = isupper(fromPiece) ? "White" : "Black";
        string victim = isupper(toPiece) ? "White" : "Black";
        cout << attacker << " captured " << victim << "'s " << toPiece << endl;
        if (isupper(fromPiece)) {
            capturedByWhite[whiteCapturedCount++] = toPiece;
        } else {
            capturedByBlack[blackCapturedCount++] = toPiece;
        }
        Sleep(1000);
    }

    board[rowTo][colTo] = fromPiece;
    board[rowFrom][colFrom] = '.';
}

void printBoard() {
    system("cls");
    cout << "\n     a   b   c   d   e   f   g   h" << endl;
    cout << "   +---+---+---+---+---+---+---+---+" << endl;

    for (int r = 0; r < 8; r++) {
        cout << " " << (r + 1) << " ";
        for (int c = 0; c < 8; c++) {
            cout << "| ";
            char piece = board[r][c];
            if (isupper(piece)) setColor(15); // White
            else if (islower(piece)) setColor(10); // Black
            else setColor(8); // Empty
            cout << piece;
            setColor(15);
            cout << " ";
        }
        cout << "| " << (r + 1) << endl;
        cout << "   +---+---+---+---+---+---+---+---+" << endl;
    }
    cout << "     a   b   c   d   e   f   g   h" << endl;

    cout << "\nWhite Captures: ";
    for (int i = 0; i < whiteCapturedCount; i++) cout << capturedByWhite[i] << ' ';
    cout << "\nBlack Captures: ";
    for (int i = 0; i < blackCapturedCount; i++) cout << capturedByBlack[i] << ' ';


    cout << "\n********************************\n";
    Sleep(1000);
}

void convertPlay() {
    rowFrom = play[0] - '1';
    colFrom = play[1] - 'a';
    rowTo = play[2] - '1';
    colTo = play[3] - 'a';
}

int main() {
    int count = 0;
    setupBoard();
    printBoard();

    while (true) {
        if (count % 2 == 0) {
            setColor(11); cout << "White's Move (e.g. 2a3a or exit): "; setColor(15);
        input_white:
            cin >> play;
            if (strcmp(play, "exit") == 0) break;
            convertPlay();
            if (islower(board[rowFrom][colFrom]) || board[rowFrom][colFrom] == '.') {
                setColor(12); cout << "Invalid! It's White's turn. Enter again: "; setColor(15);
                goto input_white;
            }
            if (checkTurn()) {
                updateBoard();
                Beep(800, 200);
            } else {
                setColor(12); cout << "Invalid move! Enter again: "; setColor(15);
                goto input_white;
            }
        } else {
            setColor(12); cout << "Black's Move (e.g. 7a6a or exit): "; setColor(15);
        input_black:
            cin >> play;
            if (strcmp(play, "exit") == 0) break;
            convertPlay();
            if (isupper(board[rowFrom][colFrom]) || board[rowFrom][colFrom] == '.') {
                setColor(12); cout << "Invalid! It's Black's turn. Enter again: "; setColor(15);
                goto input_black;
            }
            if (checkTurn()) {
                updateBoard();
                Beep(800, 200);
            } else {
                setColor(12); cout << "Invalid move! Enter again: "; setColor(15);
                goto input_black;
            }
        }
        printBoard();
        count++;
    }
    return 0;
}

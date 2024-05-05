/**
 * @file validate.c
 * @author zhihaohong52
 * @brief Validates the board
 * @version 0.1
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "defs.h"
#include "stdio.h"
#include "string.h"

/**
 * @brief Checks if a square is on the board
 *
 * @param sq The square to check
 * @return int 1 if the square is on the board, 0 otherwise
 */
int SqOnBoard(const int sq) {
    return FilesBrd[sq] == OFFBOARD ? 0 : 1;
}

/**
 * @brief Checks if a side is valid
 *
 * @param side The side to check
 * @return int 1 if the side is valid, 0 otherwise
 */
int SideValid(const int side) {
    return (side == WHITE || side == BLACK) ? 1 : 0;
}

/**
 * @brief Checks if a file is valid
 *
 * @param file The file to check
 * @return int 1 if the file is valid, 0 otherwise
 */
int FileRankValid(const int fr) {
    return (fr >= 0 && fr <= 7) ? 1 : 0;
}

/**
 * @brief Checks if a piece is valid and not empty
 *
 * @param pce The piece to check
 * @return int 1 if the piece is valid and not empty, 0 otherwise
 */
int PieceValidEmpty(const int pce) {
    return (pce >= EMPTY && pce <= bK) ? 1 : 0;
}

/**
 * @brief Checks if a piece is valid
 *
 * @param pce The piece to check
 * @return int 1 if the piece is valid, 0 otherwise
 */
int PieceValid(const int pce) {
    return (pce >= wP && pce <= bK) ? 1 : 0;
}

/**
 * @brief Test if mirror of board is correct
 *
 * @param pos Pointer to the board
 */
void MirrorEvalTest(S_BOARD *pos) {
    FILE *file;
    file = fopen("mirror.epd","r");
    char lineIn [1024];
    int ev1 = 0; int ev2 = 0;
    int positions = 0;
    if (file == NULL) {
        printf("File Not Found\n");
        return;
    } else {
        while(fgets (lineIn , 1024 , file) != NULL) {
            ParseFen(lineIn, pos);
            positions++;
            ev1 = EvalPosition(pos);
            MirrorBoard(pos);
            ev2 = EvalPosition(pos);

            if(ev1 != ev2) {
                printf("\n\n\n");
                ParseFen(lineIn, pos);
                PrintBoard(pos);
                MirrorBoard(pos);
                PrintBoard(pos);
                printf("\n\nMirror Fail:\n%s\n",lineIn);
                getchar();
                return;
            }

            if( (positions % 1000) == 0)   {
                printf("position %d\n",positions);
            }

            memset(&lineIn[0], 0, sizeof(lineIn));
        }
    }
}
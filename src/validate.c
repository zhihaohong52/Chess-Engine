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
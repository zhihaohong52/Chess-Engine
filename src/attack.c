/**
 * @file attack.c
 * @author your name (you@domain.com)
 * @brief Contains functions to determine if a square is attacked by a piece of a certain color
 * @version 0.1
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "defs.h"

/**
 * @brief The directions a knight can move
 *
 */
const int KnDir[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };

/**
 * @brief The directions a rook can move
 *
 */
const int RkDir[4] = { -1, -10, 1, 10 };

/**
 * @brief The directions a bishop can move
 *
 */
const int BiDir[4] = { -9, -11, 11, 9 };

/**
 * @brief The directions a king can move
 *
 */
const int KiDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

/**
 * @brief Determines if a square is attacked by a piece of a certain color
 *
 * @param sq The square to check
 * @param side The side to check
 * @param pos A pointer to the board
 * @return int 1 if the square is attacked, 0 otherwise
 */
int SqAttacked(const int sq, const int side, const S_BOARD *pos) {

    int pce,index,t_sq,dir;

    ASSERT(SqOnBoard(sq));
    ASSERT(SideValid(side));
    ASSERT(CheckBoard(pos));

    // pawns
    if(side == WHITE) {
        if(pos->pieces[sq - 11] == wP || pos->pieces[sq - 9] == wP) {
            return TRUE;
        }
    } else {
        if(pos->pieces[sq + 11] == bP || pos->pieces[sq + 9] == bP) {
            return TRUE;
        }
    }

    // knights
    for (index = 0; index < 8; ++index) {
        pce = pos->pieces[sq + KnDir[index]];
        if (IsKn(pce) && PieceCol[pce] == side) {
            return TRUE;
        }
    }

    // rooks, queens
    for (index = 0; index < 4; ++index) {
        dir = RkDir[index];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while (pce != OFFBOARD) {
            if (pce != EMPTY) {
                if (IsRQ(pce) && PieceCol[pce] == side) {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }

    // bishops, queens
    for (index = 0; index < 4; ++index) {
        dir = BiDir[index];
        t_sq = sq + dir;
        pce = pos->pieces[t_sq];
        while (pce != OFFBOARD) {
            if (pce != EMPTY) {
                if (IsBQ(pce) && PieceCol[pce] == side) {
                    return TRUE;
                }
                break;
            }
            t_sq += dir;
            pce = pos->pieces[t_sq];
        }
    }

    // kings
    for (index = 0; index < 8; ++index) {
        pce = pos->pieces[sq + KiDir[index]];
        if (IsKi(pce) && PieceCol[pce] == side) {
            return TRUE;
        }
    }

    return FALSE;
}
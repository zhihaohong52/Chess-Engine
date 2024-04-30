/**
 * @file hashkey.c
 * @author zhihaohong52
 * @brief Contains functions to generate a unique key for a position
 * @version 0.1
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "defs.h"

/**
 * @brief Function to generate the position key using Zobrist hashing
 *
 * @param pos Pointer to the board
 * @return U64 Key for the position
 */
U64 GeneratePosKey(const S_BOARD *pos){

    int sq = 0;
    U64 finalKey = 0;
    int piece = EMPTY;

    // pieces
    for(sq = 0; sq < BRD_SQ_NUM; ++sq){
        piece = pos->pieces[sq];
        if(piece!=NO_SQ && piece!=EMPTY && piece!=OFFBOARD){
            ASSERT(piece>=wP && piece<=bK);
            finalKey ^= PieceKeys[piece][sq];
        }
    }

    if(pos->side == WHITE){
        finalKey ^= SideKey;
    }

    if(pos->enPas != NO_SQ){
        ASSERT(pos->enPas>=0 && pos->enPas<BRD_SQ_NUM);
        finalKey ^= PieceKeys[EMPTY][pos->enPas];
    }

    ASSERT(pos->castlePerm>=0 && pos->castlePerm<=15);

    finalKey ^= CastleKeys[pos->castlePerm];

    return finalKey;
}
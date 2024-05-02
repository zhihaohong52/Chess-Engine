/**
 * @file init.c
 * @author zhihaohong52
 * @brief Initializes the board
 * @version 0.1
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "defs.h"
#include "stdlib.h"

#define RAND_64 (   (U64)rand() | \
                    (U64)rand() << 15 | \
                    (U64)rand() << 30 | \
                    (U64)rand() << 45 | \
                    ((U64)rand() & 0xf) << 60    )

/**
 * @brief 120 square to 64 square conversion
 *
 */
int Sq120ToSq64[BRD_SQ_NUM];

/**
 * @brief 64 square to 120 square conversion
 *
 */
int Sq64ToSq120[64];

/**
 * @brief Set bit masks
 *
 */
U64 SetMask[64];

/**
 * @brief Clear bit masks
 *
 */
U64 ClearMask[64];

/**
 * @brief Piece keys
 *
 */
U64 PieceKeys[13][120];

/**
 * @brief Side key
 *
 */
U64 SideKey;

/**
 * @brief Castle keys
 *
 */
U64 CastleKeys[16];

/**
 * @brief Board files
 *
 */
int FilesBrd[BRD_SQ_NUM];

/**
 * @brief Board ranks
 *
 */
int RanksBrd[BRD_SQ_NUM];

/**
 * @brief Initializes the files and ranks
 *
 */
void InitFilesRanksBrd(){

    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;

    for(index = 0; index < BRD_SQ_NUM; ++index){
        FilesBrd[index] = OFFBOARD;
        RanksBrd[index] = OFFBOARD;
    }

    for(rank = RANK_1; rank <= RANK_8; ++rank){
        for(file = FILE_A; file <= FILE_H; ++file){
            sq = FR2SQ(file,rank);
            FilesBrd[sq] = file;
            RanksBrd[sq] = rank;
        }
    }
}

/**
 * @brief Initializes the hash keys
 *
 */
void InitHashKeys(){
    int index = 0;
    int index2 = 0;
    for(index = 0; index < 13; ++index){
        for(index2 = 0; index2 < 120; ++index2){
            PieceKeys[index][index2] = RAND_64;
        }
    }
    SideKey = RAND_64;
    for(index = 0; index < 16; ++index){
        CastleKeys[index] = RAND_64;
    }
}

/**
 * @brief Initializes the bit masks
 *
 */
void InitBitMasks(){
    int index = 0;

    for(index = 0; index < 64; index++){
        SetMask[index] = 0ULL;
        ClearMask[index] = 0ULL;
    }

    for(index = 0; index < 64; index++){
        SetMask[index] |= (1ULL << index);
        ClearMask[index] = ~SetMask[index];
    }

}

/**
 * @brief Initializes the 120 to 64 square conversion
 *
 */
void InitSq120To64(){

    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;
    for(index = 0; index < BRD_SQ_NUM; ++index){
        Sq120ToSq64[index] = 65;
    }

    for(index = 0; index < 64; ++index){
        Sq64ToSq120[index] = 120;
    }

    for(rank = RANK_1; rank <= RANK_8; ++rank){
        for(file = FILE_A; file <= FILE_H; ++file){
            sq = FR2SQ(file,rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

/**
 * @brief Initializes the board
 *
 */
void AllInit(){
    InitSq120To64();
    InitBitMasks();
    InitHashKeys();
    InitFilesRanksBrd();
    InitMvvLva();
}
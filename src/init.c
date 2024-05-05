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
 * @brief File bit masks
 *
 */
U64 FileBBMask[8];

/**
 * @brief Rank bit masks
 *
 */
U64 RankBBMask[8];

/**
 * @brief Black passed pawn masks
 *
 */
U64 BlackPassedMask[64];

/**
 * @brief White passed pawn masks
 *
 */
U64 WhitePassedMask[64];

/**
 * @brief Isolated pawn masks
 *
 */
U64 IsolatedMask[64];

/**
 * @brief Initializes the bitmasks for pawns
 *
 */
void InitEvalMasks() {

    int sq, tsq, r, f;

    for(sq = 0; sq < 8; ++sq) {
        FileBBMask[sq] = 0ULL;
        RankBBMask[sq] = 0ULL;
    }

    for(r = RANK_8; r >= RANK_1; r--) {
        for(f = FILE_A; f <= FILE_H; f++) {
            sq = r * 8 + f;
            FileBBMask[f] |= (1ULL << sq);
            RankBBMask[r] |= (1ULL << sq);
        }
    }

    for(sq = 0; sq < 64; ++sq) {
        WhitePassedMask[sq] = 0ULL;
        BlackPassedMask[sq] = 0ULL;
        IsolatedMask[sq] = 0ULL;
    }

    for(sq = 0; sq < 64; ++sq) {
        tsq = sq + 8;

        while(tsq < 64) {
            WhitePassedMask[sq] |= (1ULL << tsq);
            tsq += 8;
        }

        tsq = sq - 8;
        while(tsq >= 0) {
            BlackPassedMask[sq] |= (1ULL << tsq);
            tsq -= 8;
        }

        if(FilesBrd[SQ120(sq)] > FILE_A) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[SQ120(sq)] - 1];

            tsq = sq + 7;
            while(tsq < 64) {
                WhitePassedMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 9;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }

        if(FilesBrd[SQ120(sq)] < FILE_H) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[SQ120(sq)] + 1];

            tsq = sq + 9;
            while(tsq < 64) {
                WhitePassedMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 7;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
    }
}

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
    InitEvalMasks();
    InitMvvLva();
}
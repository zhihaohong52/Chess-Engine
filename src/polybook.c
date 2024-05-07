/**
 * @file polybook.c
 * @author zhihaohong52
 * @brief Contains functions to generate PolyKeys
 * @version 0.2
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "defs.h"
#include "polykeys.h"

/**
 * @brief Converts the piece to the PolyKindOfPiece
 *
 */
const int PolyKindOfPiece[13] = {
	-1, 1, 3, 5, 7, 9, 11, 0, 2, 4, 6, 8, 10
};

/**
 * @brief Check if there is a board for en passant capture
 *
 * @param board Pointer to the board
 * @return int TRUE if there is a pawn for capture, FALSE otherwise
 */
int HasPawnForCapture(const S_BOARD *board) {
	int sqWithPawn = 0;
	int targetPce = (board->side == WHITE) ? wP : bP;
	if(board->enPas != NO_SQ) {
		if(board->side == WHITE) {
			sqWithPawn = board->enPas - 10;
		} else {
			sqWithPawn = board->enPas + 10;
		}

		if(board->pieces[sqWithPawn + 1] == targetPce) {
			return TRUE;
		} else if(board->pieces[sqWithPawn - 1] == targetPce) {
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief Get PolyKey from the board
 * @cite H.G. Muller http://hgm.nubati.net/book_format.html
 *
 * @param board Pointer to the board
 * @return U64 PolyKey
 */
U64 PolyKeyFromBoard(S_BOARD *board) {

	int sq = 0, rank = 0, file = 0;
	U64 finalKey = 0;
	int piece = EMPTY;
	int polyPiece = 0;
	int offset = 0;

	for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
		piece = board->pieces[sq];
		if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
			ASSERT(piece >= wP && piece <= bK);
			polyPiece = PolyKindOfPiece[piece];
			rank = RanksBrd[sq];
			file = FilesBrd[sq];
			finalKey ^= Random64Poly[(64 * polyPiece) + (8 * rank) + file];
		}
	}

	// castling
	offset = 768;

	if(board->castlePerm & WKCA) finalKey ^= Random64Poly[offset + 0];
	if(board->castlePerm & WQCA) finalKey ^= Random64Poly[offset + 1];
	if(board->castlePerm & BKCA) finalKey ^= Random64Poly[offset + 2];
	if(board->castlePerm & BQCA) finalKey ^= Random64Poly[offset + 3];

	// enpassant
	offset = 772;
	if(HasPawnForCapture(board) == TRUE) {
		file = FilesBrd[board->enPas];
		finalKey ^= Random64Poly[offset + file];
	}

	if(board->side == WHITE) {
		finalKey ^= Random64Poly[780];
	}
	return finalKey;
}

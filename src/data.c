/**
 * @file data.c
 * @author zhihaohong52
 * @brief Contains data for the game
 * @version 0.1
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "defs.h"

/**
 * @brief Character arrays for the pieces
 *
 */
char PceChar[] = ".PNBRQKpnbrqk";

/**
 * @brief Character arrays for the sides
 *
 */
char SideChar[] = "wb-";

/**
 * @brief Character arrays for the ranks
 *
 */
char RankChar[] = "12345678";

/**
 * @brief Character arrays for the files
 *
 */
char FileChar[] = "abcdefgh";

/**
 * @brief Arrays for the piece types
 *
 */
int PieceBig[13] = { FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE };

/**
 * @brief Arrays for the major pieces (queen and rook)
 *
 */
int PieceMaj[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE };

/**
 * @brief Arrays for the minor pieces (bishop and knight)
 *
 */
int PieceMin[13] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE };

/**
 * @brief Arrays for the piece values
 *
 */
int PieceVal[13]= { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000  };

/**
 * @brief Arrays for the piece colors
 *
 */
int PieceCol[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
	BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

/**
 * @brief Arrays for the pawn pieces
 *
 */
int PiecePawn[13] = { FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE };

/**
 * @brief Arrays for the knight pieces
 *
 */
int PieceKnight[13] = { FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE };

/**
 * @brief Arrays for the king pieces
 *
 */
int PieceKing[13] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE };

/**
 * @brief Arrays for the rook and queen pieces
 *
 */
int PieceRookQueen[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE };

/**
 * @brief Arrays for the bishop and queen pieces
 *
 */
int PieceBishopQueen[13] = { FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE };

/**
 * @brief Arrays for the sliding pieces
 *
 */
int PieceSlides[13] = { FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE };
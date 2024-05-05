/**
 * @file perft.c
 * @author zhihaohong52
 * @brief Contains functions to test the move generation
 * @version 0.2
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stdio.h"
#include "defs.h"

/**
 * @brief Variable to store the number of leaf nodes
 *
 */
long leafNodes;

/**
 * @brief Function with the recursive call to test the move generation
 *
 * @param depth The depth to test the move generation
 * @param pos Pointer to the board
 */
void Perft(int depth, S_BOARD *pos) {

    ASSERT(CheckBoard(pos));

	if(depth == 0) {
        leafNodes++;
        return;
    }

    S_MOVELIST list[1];
    GenerateAllMoves(pos,list);

    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {

        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }
        Perft(depth - 1, pos);
        TakeMove(pos);
    }

    return;
}

/**
 * @brief Function for testing the move generation using perft
 *
 * @param depth The depth to test the move generation
 * @param pos Pointer to the board
 */
void PerftTest(int depth, S_BOARD *pos) {

    ASSERT(CheckBoard(pos));

	PrintBoard(pos);
	printf("\nStarting Test To Depth:%d\n",depth);
	leafNodes = 0;
    int start = GetTimeMs();
    S_MOVELIST list[1];
    GenerateAllMoves(pos,list);

    int move;
    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        move = list->moves[MoveNum].move;
        if ( !MakeMove(pos,move))  {
            continue;
        }
        long cumnodes = leafNodes;
        Perft(depth - 1, pos);
        TakeMove(pos);
        long oldnodes = leafNodes - cumnodes;
        printf("move %d : %s : %ld\n",MoveNum+1,PrMove(move),oldnodes);
    }

	printf("\nTest Complete : %ld nodes visited in %dms\n",leafNodes, GetTimeMs() - start);

    return;
}

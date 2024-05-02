/**
 * @file vice.c
 * @author zhihaohong52
 * @brief Main file to run the chess engine
 * @version 0.1
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024
 *
 * @mainpage Project Documentation
 *
 * Welcome to documentation for Vice, a chess engine written in C.
 *
 * @section sec_intro Introduction
 *
 * Vice is a chess engine written in C. The engine is inspired by the series created by Bluefever Software.
 *
 * @section sec_usage Usage
 *
 * To run the engine, simply compile the source code by running the following command:
 * @code
 * make
 * @endcode
 * Then, run the executable:\n
 * For Linux:
 * @code
 * ./vice
 * @endcode
 * For Windows:
 * @code
 * ./vice.exe
 * @endcode
 *
 * @section sec_dev Development
 *
 * The project is currently in development.
 *
 */

#include "stdio.h"
#include "stdlib.h"
#include "defs.h"

#define PERFTFEN "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"

/**
 * @brief Main function
 *
 * @return int
 */
int main(){

    AllInit();

    S_BOARD board[1];
    S_MOVELIST list[1];

    ParseFen(START_FEN, board);

    char input[6];
    int Move = NOMOVE;
    int PvNum = 0;
    int Max = 0;

    while (TRUE) {
        PrintBoard(board);
        printf("Please enter a move > ");
        fgets(input, 6, stdin);

        if (input[0] == 'q') {
            break;
        } else if (input[0] == 't') {
            TakeMove(board);
            continue;
        } else if (input[0] == 'p') {
            // PerftTest(4, board);
            Max = GetPvLine(4, board);
            printf("PvLine of %d moves: ", Max);
            for (PvNum = 0; PvNum < Max; ++PvNum) {
                Move = board->PvArray[PvNum];
                printf(" %s", PrMove(Move));
            }
            printf("\n");
        } else {
            Move = ParseMove(input, board);
            if (Move != NOMOVE) {
                StorePvMove(board, Move);
                MakeMove(board, Move);
            } else {
                printf("Move not parsed: %s\n", input);
            }
        }

        fflush(stdin);
    }

    return 0;
}
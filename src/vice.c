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

#define WAC1 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

/**
 * @brief Main function
 *
 * @return int
 */
int main(){

    AllInit();

    S_BOARD board[1];
    InitPvTable(board->PvTable);

    S_MOVELIST list[1];
    S_SEARCHINFO info[1];

    ParseFen(WAC1, board);

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
        } else if (input[0] == 's') {
            info->depth = 6;
            SearchPosition(board, info);
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

    free(board->PvTable->pTable);

    return 0;
}
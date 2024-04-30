/**
 * @file vice.c
 * @author zhihaohong52
 * @brief Main file
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

#define PERFTFEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

/**
 * @brief Main function
 *
 * @return int
 */
int main(){

    AllInit();

    S_BOARD board[1];
    S_MOVELIST list[1];

    ParseFen(PERFTFEN, board);

    PerftTest(4, board);

    return 0;
}
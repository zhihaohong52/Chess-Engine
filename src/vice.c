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
#define PERFT "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

/**
 * @brief Main function
 *
 * @return int
 */
int main(){

    AllInit();
    Uci_Loop();
    return 0;
}
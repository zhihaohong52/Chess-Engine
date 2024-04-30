/**
 * @file io.c
 * @author zhihaohong52
 * @brief Contains functions to print the board and moves
 * @version 0.1
 * @date 2024-04-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stdio.h"
#include "defs.h"

/**
 * @brief Prints the square
 *
 * @param sq Integer representing the square
 * @return char* String representing the square
 */
char *PrSq(const int sq) {

        static char sqStr[3];

        int file = FilesBrd[sq];
        int rank = RanksBrd[sq];

        sprintf(sqStr, "%c%c", ('a' + file), ('1' + rank));

        return sqStr;

}

/**
 * @brief Prints the move
 *
 * @param move Integer representing the move
 * @return char* String representing the move
 */
char *PrMove(const int move) {

        static char MvStr[6];

        int ff = FilesBrd[FROMSQ(move)];
        int rf = RanksBrd[FROMSQ(move)];
        int ft = FilesBrd[TOSQ(move)];
        int rt = RanksBrd[TOSQ(move)];

        int promoted = PROMOTED(move);

        if (promoted) {
                char pchar = 'q';
                if (IsKn(promoted)) {
                        pchar = 'n';
                } else if (IsRQ(promoted) && !IsBQ(promoted))  {
                        pchar = 'r';
                } else if (!IsRQ(promoted) && IsBQ(promoted))   {
                        pchar = 'b';
                }
                sprintf(MvStr, "%c%c%c%c%c", ('a' + ff), ('1' + rf), ('a' + ft), ('1' + rt), pchar);
        } else {
                sprintf(MvStr, "%c%c%c%c", ('a' + ff), ('1' + rf), ('a' + ft), ('1' + rt));
        }

        return MvStr;
}

/**
 * @brief Prints the move list
 *
 * @param list Pointer to the move list
 */
void PrintMoveList(const S_MOVELIST *list) {
        int index = 0;
        int score = 0;
        int move = 0;
        printf("MoveList:\n", list->count);

        for (index = 0; index < list->count; ++index) {
                move = list->moves[index].move;
                score = list->moves[index].score;

                printf("Move:%d > %s (score:%d)\n", index + 1, PrMove(move), score);
        }
        printf("MoveList Total %d Moves:\n\n", list->count);
}
/**
 * @file pvtable.c
 * @author zhihaohong52
 * @brief This file contains functions to store the hash table/ principal variation table (PV Table)
 * @version 0.1
 * @date 2024-05-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stdio.h"
#include "defs.h"

/**
 * @brief Get the Pv Line object
 *
 * @param depth The depth of the search
 * @param pos Pointer to the board
 * @return int Count of the moves in the PV line
 */
int GetPvLine(const int depth, S_BOARD *pos) {

    ASSERT(depth < MAXDEPTH);

    int move = ProbePVTable(pos);
    int count = 0;

    while (move != NOMOVE && count < depth) {

        ASSERT(count < MAXDEPTH);

        if (MoveExists(pos, move)) {
            MakeMove(pos, move);
            pos->PvArray[count++] = move;
        } else {
            break;
        }
        move = ProbePVTable(pos);
    }

    while(pos->ply > 0) {
        TakeMove(pos);
    }

    return count;

}

/**
 * @brief Initilize the PV table to 2MB
 *
 */
const int PvSize = 0x100000 * 2;

/**
 * @brief Clear the PV table
 *
 */
void ClearPVTable(S_PVTABLE *table) {

    S_PVENTRY *pvEntry;

    for(pvEntry = table->pTable; pvEntry < table->pTable + table->numEntries; pvEntry++) {
        pvEntry->posKey = 0ULL;
        pvEntry->move = NOMOVE;
    }

}

/**
 * @brief Initialize the PV table
 *
 * @param table Pointer to the PV table
 */
void InitPvTable(S_PVTABLE *table) {

    table->numEntries = PvSize / sizeof(S_PVENTRY);
    table->numEntries -= 2;
    free(table->pTable);
    table->pTable = (S_PVENTRY *)malloc(table->numEntries * sizeof(S_PVENTRY));
    ClearPVTable(table);
    printf("PV Table init complete with %d entries\n",table->numEntries);

}

/**
 * @brief Function to store the principal variation move
 *
 * @param pos Pointer to the board
 * @param move The move to be stored
 */
void StorePvMove(const S_BOARD *pos, const int move) {

    int index = pos->posKey % pos->PvTable->numEntries;
    ASSERT(index >= 0 && index <= pos->PvTable->numEntries - 1);

    pos->PvTable->pTable[index].move = move;
    pos->PvTable->pTable[index].posKey = pos->posKey;
}

/**
 * @brief Function to probe the PV table
 *
 * @param pos Pointer to the board
 * @return int The move from the PV table
 */
int ProbePVTable(const S_BOARD *pos) {

    int index = pos->posKey % pos->PvTable->numEntries;
    ASSERT(index >= 0 && index <= pos->PvTable->numEntries - 1);

    if(pos->PvTable->pTable[index].posKey == pos->posKey) {
        return pos->PvTable->pTable[index].move;
    }

    return NOMOVE;
}
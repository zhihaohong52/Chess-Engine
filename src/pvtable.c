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

    int move = ProbePvMove(pos);
    int count = 0;

    while (move != NOMOVE && count < depth) {

        ASSERT(count < MAXDEPTH);

        if (MoveExists(pos, move)) {
            MakeMove(pos, move);
            pos->PvArray[count++] = move;
        } else {
            break;
        }
        move = ProbePvMove(pos);
    }

    while(pos->ply > 0) {
        TakeMove(pos);
    }

    return count;

}

/**
 * @brief Hash table size
 *
 */
const int HashSize = 0x100000 * 16;

/**
 * @brief Clear the hash table
 *
 */
void ClearHashTable(S_HASHTABLE *table) {

    S_HASHENTRY *tableEntry;

    for(tableEntry = table->pTable; tableEntry < table->pTable + table->numEntries; tableEntry++) {
        tableEntry->posKey = 0ULL;
        tableEntry->move = NOMOVE;
        tableEntry->depth = 0;
        tableEntry->score = 0;
        tableEntry->flags = 0;
    }
    table->newWrite = 0;
}

/**
 * @brief Initialize the hash table
 *
 * @param table Pointer to the hash table
 */
void InitHashTable(S_HASHTABLE *table) {

    table->numEntries = HashSize / sizeof(S_HASHENTRY);
    table->numEntries -= 2;
    free(table->pTable);
    table->pTable = (S_HASHENTRY *)malloc(table->numEntries * sizeof(S_HASHENTRY));
    ClearHashTable(table);
    printf("PV Table init complete with %d entries\n",table->numEntries);

}

/**
 * @brief Function to probe the hash table
 *
 * @param pos Pointer to the board
 * @param move Pointer to the move
 * @param score Pointer to the score
 * @param alpha Pointer to the alpha
 * @param beta Pointer to the beta
 * @param depth Pointer to the depth
 * @return int The move from the hash table
 */
int ProbeHashEntry(S_BOARD *pos, int *move, int *score, int alpha, int beta, int depth) {

	int index = pos->posKey % pos->HashTable->numEntries;

	ASSERT(index >= 0 && index <= pos->HashTable->numEntries - 1);
    ASSERT(depth>=1&&depth<MAXDEPTH);
    ASSERT(alpha<beta);
    ASSERT(alpha>=-INFINITE&&alpha<=INFINITE);
    ASSERT(beta>=-INFINITE&&beta<=INFINITE);
    ASSERT(pos->ply>=0&&pos->ply<MAXDEPTH);

	if( pos->HashTable->pTable[index].posKey == pos->posKey ) {
		*move = pos->HashTable->pTable[index].move;
		if(pos->HashTable->pTable[index].depth >= depth){
			pos->HashTable->hit++;

			ASSERT(pos->HashTable->pTable[index].depth>=1&&pos->HashTable->pTable[index].depth<MAXDEPTH);
            ASSERT(pos->HashTable->pTable[index].flags>=HFALPHA&&pos->HashTable->pTable[index].flags<=HFEXACT);

			*score = pos->HashTable->pTable[index].score;
			if(*score > ISMATE) *score -= pos->ply;
            else if(*score < -ISMATE) *score += pos->ply;

			switch(pos->HashTable->pTable[index].flags) {

                ASSERT(*score>=-INFINITE&&*score<=INFINITE);

                case HFALPHA: if(*score<=alpha) {
                    *score=alpha;
                    return TRUE;
                    }
                    break;
                case HFBETA: if(*score>=beta) {
                    *score=beta;
                    return TRUE;
                    }
                    break;
                case HFEXACT:
                    return TRUE;
                    break;
                default: ASSERT(FALSE); break;
            }
		}
	}

	return FALSE;
}

/**
 * @brief Function to store the hash entry
 *
 * @param pos Pointer to the board
 * @param move The move to be stored
 * @param score The score of the move
 * @param flags The flags of the move
 * @param depth The depth of the search
 */
void StoreHashEntry(S_BOARD *pos, const int move, int score, const int flags, const int depth) {

	int index = pos->posKey % pos->HashTable->numEntries;

	ASSERT(index >= 0 && index <= pos->HashTable->numEntries - 1);
	ASSERT(depth>=1&&depth<MAXDEPTH);
    ASSERT(flags>=HFALPHA&&flags<=HFEXACT);
    ASSERT(score>=-INFINITE&&score<=INFINITE);
    ASSERT(pos->ply>=0&&pos->ply<MAXDEPTH);

	if( pos->HashTable->pTable[index].posKey == 0) {
		pos->HashTable->newWrite++;
	} else {
		pos->HashTable->overWrite++;
	}

	if(score > ISMATE) score += pos->ply;
    else if(score < -ISMATE) score -= pos->ply;

	pos->HashTable->pTable[index].move = move;
    pos->HashTable->pTable[index].posKey = pos->posKey;
	pos->HashTable->pTable[index].flags = flags;
	pos->HashTable->pTable[index].score = score;
	pos->HashTable->pTable[index].depth = depth;
}

/**
 * @brief Function to probe the principal variation table
 *
 * @param pos Pointer to the board
 * @return int The move from the PV table
 */
int ProbePvMove(const S_BOARD *pos) {

	int index = pos->posKey % pos->HashTable->numEntries;
	ASSERT(index >= 0 && index <= pos->HashTable->numEntries - 1);

	if( pos->HashTable->pTable[index].posKey == pos->posKey ) {
	    return pos->HashTable->pTable[index].move;
	}

	return NOMOVE;
}
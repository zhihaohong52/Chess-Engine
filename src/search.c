/**
 * @file search.c
 * @author zhihaohong52
 * @brief This file contains functions to search the board
 * @version 0.2
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stdio.h"
#include "defs.h"

#define MATE 29000

/**
 * @brief Check if the time is up
 *
 * @param info Pointer to the search info
 */
static void CheckUp(S_SEARCHINFO *info) {
    if (info->timeset == TRUE && GetTimeMs() > info->stoptime) {
        info->stopped = TRUE;
    }

    ReadInput(info);
}

/**
 * @brief Pick the next move based on the score
 *
 * @param moveNum Move number
 * @param list Pointer to the move list
 */
static void PickNextMove(int moveNum, S_MOVELIST *list) {

    S_MOVE temp;
    int index = 0;
    int bestScore = 0;
    int bestNum = moveNum;

    for (index = moveNum; index < list->count; ++index) {
        if (list->moves[index].score > bestScore) {
            bestScore = list->moves[index].score;
            bestNum = index;
        }
    }

    ASSERT(moveNum >= 0 && moveNum < list->count);
    ASSERT(bestNum >= 0 && bestNum < list->count);
    ASSERT(bestNum >= moveNum);

    temp = list->moves[moveNum];
    list->moves[moveNum] = list->moves[bestNum];
    list->moves[bestNum] = temp;
}

/**
 * @brief Check if the position is a repetition
 *
 * @param pos Pointer to the board
 * @return int TRUE if the position is a repetition, FALSE otherwise
 */
static int IsRepetition(const S_BOARD *pos) {
    int index = 0;

    for (index = pos->hisPly - pos->fiftyMove; index < pos->hisPly - 1; ++index) {

        ASSERT(index >= 0 && index < MAXGAMEMOVES);

        if (pos->posKey == pos->history[index].posKey) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * @brief Clear the search history and killers
 *
 * @param pos Pointer to the board
 * @param info Pointer to the search info
 */
static void ClearForSearch(S_BOARD *pos, S_SEARCHINFO *info) {

    int index = 0;
    int index2 = 0;

    for (index = 0; index < 13; ++index) {
        for (index2 = 0; index2 < BRD_SQ_NUM; ++index2) {
            pos->searchHistory[index][index2] = 0;
        }
    }

    for (index = 0; index < 2; ++index) {
        for (index2 = 0; index2 < MAXDEPTH; ++index2) {
            pos->searchKillers[index][index2] = 0;
        }
    }

    pos->HashTable->overWrite = 0;
    pos->HashTable->hit = 0;
    pos->HashTable->cut = 0;
    pos->ply = 0;

    info->stopped = 0;
    info->nodes = 0;
    info->fh = 0;
    info->fhf = 0;
}

/**
 * @brief Quiescence search to avoid horizon effect
 *
 * @param alpha Alpha value
 * @param beta Beta value
 * @param pos Pointer to the board
 * @param info Pointer to the search info
 * @return int
 */
static int Quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info) {

	ASSERT(CheckBoard(pos));
	ASSERT(beta>alpha);
	if(( info->nodes & 2047 ) == 0) {
		CheckUp(info);
	}

	info->nodes++;

	if(IsRepetition(pos) || pos->fiftyMove >= 100) {
		return 0;
	}

	if(pos->ply > MAXDEPTH - 1) {
		return EvalPosition(pos);
	}

	int Score = EvalPosition(pos);

	ASSERT(Score>-INFINITE && Score<INFINITE);

	if(Score >= beta) {
		return beta;
	}

	if(Score > alpha) {
		alpha = Score;
	}

	S_MOVELIST list[1];
    GenerateAllCaps(pos,list);

    int MoveNum = 0;
	int Legal = 0;
	Score = -INFINITE;

	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {

		PickNextMove(MoveNum, list);

        if ( !MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }

		Legal++;
		Score = -Quiescence( -beta, -alpha, pos, info);
        TakeMove(pos);

		if(info->stopped == TRUE) {
			return 0;
		}

		if(Score > alpha) {
			if(Score >= beta) {
				if(Legal==1) {
					info->fhf++;
				}
				info->fh++;
				return beta;
			}
			alpha = Score;
		}
    }

	ASSERT(alpha >= OldAlpha);

	return alpha;
}

/**
 * @brief Alpha beta search
 *
 * @param alpha Alpha value
 * @param beta Beta value
 * @param depth Depth of the search
 * @param pos Pointer to the board
 * @param info Pointer to the search info
 * @param DoNull Flag to indicate if null move is allowed
 * @return int
 */
static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int DoNull) {

    ASSERT(CheckBoard(pos));

    if (depth <= 0) {
        return Quiescence(alpha, beta, pos, info);
    }

    if ((info->nodes & 2047) == 0) {
        CheckUp(info);
    }

    info->nodes++;

    if ((IsRepetition(pos) || pos->fiftyMove >= 100) && pos->ply) {
        return 0;
    }

    if (pos->ply > MAXDEPTH - 1) {
        return EvalPosition(pos);
    }

    int InCheck = SqAttacked(pos->KingSq[pos->side], pos->side^1, pos);

    if (InCheck == TRUE) {
        depth++;
    }

    int Score = -INFINITE;
    int PvMove = NOMOVE;

    if (ProbeHashEntry(pos, &PvMove, &Score, alpha, beta, depth) == TRUE) {
        pos->HashTable->cut++;
        return Score;
    }

    if (DoNull && !InCheck && pos->ply && (pos->bigPce[pos->side] > 0) && depth >= 4) {
        MakeNullMove(pos);
        Score = -AlphaBeta(-beta, -beta + 1, depth - 4, pos, info, FALSE);
        TakeNullMove(pos);
        if (info->stopped == TRUE) {
            return 0;
        }
        if (Score >= beta && abs(Score) < ISMATE) {
            info->nullCut++;
            return beta;
        }
    }

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);

    int MoveNum = 0;
    int Legal = 0;
    int OldAlpha = alpha;
    int BestMove = NOMOVE;

    int BestScore = -INFINITE;

    Score = -INFINITE;

    if (PvMove != NOMOVE) {
        for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {
            if (list->moves[MoveNum].move == PvMove) {
                list->moves[MoveNum].score = 2000000;
                break;
            }
        }
    }

    for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {

        PickNextMove(MoveNum, list);

        if (!MakeMove(pos, list->moves[MoveNum].move)) {
            continue;
        }

        Legal++;
        Score = -AlphaBeta(-beta, -alpha, depth - 1, pos, info, TRUE);
        TakeMove(pos);

        if (info->stopped == TRUE) {
            return 0;
        }

        if(Score > BestScore) {
            BestScore = Score;
            BestMove = list->moves[MoveNum].move;
            if (Score > alpha) {
                if (Score >= beta) {
                    if (Legal == 1) {
                        info->fhf++;
                    }
                    info->fh++;

                    if(!(list->moves[MoveNum].move & MFLAGCAP)) {
                        pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
                        pos->searchKillers[0][pos->ply] = list->moves[MoveNum].move;
                    }

                    StoreHashEntry(pos, BestMove, beta, HFBETA, depth);

                    return beta;
                }
                alpha = Score;
                BestMove = list->moves[MoveNum].move;
                if(!(list->moves[MoveNum].move & MFLAGCAP)) {
                    pos->searchHistory[pos->pieces[FROMSQ(BestMove)]][TOSQ(BestMove)] += depth;
                }
            }
        }
    }

    if (Legal == 0) {
        if (InCheck) {
            return -MATE + pos->ply;
        } else {
            return 0;
        }
    }

    if (alpha != OldAlpha) {
        StoreHashEntry(pos, BestMove, BestScore, HFEXACT, depth);
    } else {
        StoreHashEntry(pos, BestMove, alpha, HFALPHA, depth);
    }

    return alpha;
}

/**
 * @brief Iterative deepening search
 *
 * @param pos
 * @param info
 */
void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info) {

    int bestMove = NOMOVE;
    int bestScore = -INFINITE;
    int currentDepth = 0;
    int pvMoves = 0;
    int pvNum = 0;

    ClearForSearch(pos, info);

    // Iterative deepening
    for (currentDepth = 1; currentDepth <= info->depth; ++currentDepth) {
                                //alpha  beta
        bestScore = AlphaBeta(-INFINITE, INFINITE, currentDepth, pos, info, TRUE);

        if (info->stopped == TRUE) {
            break;
        }

        pvMoves = GetPvLine(currentDepth, pos);
        bestMove = pos->PvArray[0];
        if (info->GAME_MODE == UCIMODE) {
            printf("info score cp %d depth %d nodes %ld time %d ",
                bestScore, currentDepth, info->nodes, GetTimeMs()-info->starttime);
        } else if (info->GAME_MODE == XBOARDMODE && info->POST_THINKING == TRUE) {
            printf("%d %d %d %ld ",
                currentDepth, bestScore, (GetTimeMs()-info->starttime)/10, info->nodes);
        } else if (info->POST_THINKING == TRUE) {
            printf("score:%d depth:%d nodes:%ld time:%d(ms) ",
                bestScore, currentDepth, info->nodes, GetTimeMs()-info->starttime);
        }
        if (info->GAME_MODE == UCIMODE || info->POST_THINKING == TRUE) {
            pvMoves = GetPvLine(currentDepth, pos);
            printf("pv");
            for (pvNum = 0; pvNum < pvMoves; ++pvNum) {
                printf(" %s", PrMove(pos->PvArray[pvNum]));
            }
            printf("\n");
        }
        // printf("Ordering:%.2f\n", (info->fhf/info->fh));
    }

    if(info->GAME_MODE == UCIMODE) {
		printf("bestmove %s\n",PrMove(bestMove));
	} else if(info->GAME_MODE == XBOARDMODE) {
		printf("move %s\n",PrMove(bestMove));
		MakeMove(pos, bestMove);
	} else {
		printf("\n\n***!! Vice makes move %s !!***\n\n",PrMove(bestMove));
		MakeMove(pos, bestMove);
		PrintBoard(pos);
	}


}

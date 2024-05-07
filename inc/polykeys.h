/**
 * @file polykeys.h
 * @author zhihaohong52
 * @brief Contains definitions for the PolyKeys
 * @version 0.2
 * @date 2024-05-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef POLYKEYS_H
#define POLYKEYS_H

#ifdef _MSC_VER
#  define U64_POLY(u) (u##ui64)
#else
#  define U64_POLY(u) (u##ULL)
#endif

extern const U64 Random64Poly[781];

#endif
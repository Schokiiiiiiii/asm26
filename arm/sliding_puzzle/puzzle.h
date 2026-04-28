/**
 * @file   puzzle.h
 * @author Rick Wertenbroek
 * @date   29.10.20
 *
 * @brief  Puzzles
 */

#include "sliding_puzzle.h"

#ifndef __PUZZLE_H__
#define __PUZZLE_H__

// You can try out other puzzles by changing the values below
const uint8_t puzzle[BOARD_SIZE][BOARD_SIZE] =
    {{4, 1, 3},
     {0, 2, 6},
     {7, 5, 8}};


// Don't change the impossible puzzle
const uint8_t impossible_puzzle[BOARD_SIZE][BOARD_SIZE] =
    {{2, 1, 3},
     {4, 0, 6},
     {7, 5, 8}};

#endif /* __PUZZLE_H__ */
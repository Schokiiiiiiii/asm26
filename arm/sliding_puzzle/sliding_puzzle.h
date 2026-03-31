/**
 * @file   sliding_puzzle.h
 * @author Rick Wertenbroek
 * @date   29.10.20
 *
 * @brief  Header for the sliding puzzle solver for ASM lab
 */

#ifndef __SLIDING_PUZZLE_H__
#define __SLIDING_PUZZLE_H__

#define NIBBLE_SIZE 4
#define BOARD_SIZE 3
#define NUMBER_TILES 8
#define BOARD_MEMORY (BOARD_SIZE*BOARD_SIZE*sizeof(uint8_t))

#define QUEUE_CAPACITY (1024)
#define CACHE_CAPACITY (QUEUE_CAPACITY*2)

enum direction_enum {left, right, up, down};
typedef enum direction_enum direction_t;

/// @brief Pair data structure
typedef struct Pair {
    uint32_t _1;
    uint32_t _2;
} Pair;

/// @brief Node for the A* search algorithm
struct Node;
typedef struct Node {
    const uint8_t *puzzle;
    uint32_t moves;
    struct Node *prev;
} Node;

/// @brief temporary structure to hold newly created neighbors
typedef struct neighbors_t {
    uint8_t* puzzles[4];
} neighbors_t;

/// @brief Goal position (solved puzzle)
const uint8_t GOAL[BOARD_MEMORY] = {1,2,3,4,5,6,7,8,0};

/// @brief Priority queue data structure
typedef struct Queue {
    uint32_t size;
    Node *elements[QUEUE_CAPACITY];
    uint32_t priorities[QUEUE_CAPACITY];
} Queue;

/// @brief Cache entry data structure
typedef struct CacheEntry {
    Node *node;
    uint32_t puzzle_id;
} CacheEntry;

/// @brief Cache data structure
typedef struct Cache {
    uint32_t size;
    CacheEntry entries[CACHE_CAPACITY];
} Cache;

#if __QEMU_BARE__
    // Student assembly code
    extern void swap_asm(uint8_t *a, uint8_t *b);
    extern uint32_t hamming_distance_asm(const uint8_t* puzzle);
    extern uint32_t manhattan_distance_asm(const uint8_t* puzzle);
#else
    // Empty functions to emulate empty assembly code on host
    void swap_asm(uint8_t *a, uint8_t *b) {}
    uint32_t hamming_distance_asm(const uint8_t* puzzle) {return 0;}
    uint32_t manhattan_distance_asm(const uint8_t* puzzle) {return 0;}
#endif

// Hash of puzzle (unique if puzzle is 3x3)
uint32_t get_puzzle_id(const uint8_t *puzzle);

// C versions of the functions
void swap_c(uint8_t *a, uint8_t *b);
uint32_t hamming_score(const uint8_t* puzzle);
uint32_t manhattan_distance(const uint8_t* puzzle);

#endif /* __SLIDING_PUZZLE_H__ */
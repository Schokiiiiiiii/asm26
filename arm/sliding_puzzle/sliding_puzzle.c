/**
 * @file   sliding_puzzle.c
 * @author Rick Wertenbroek
 * @date   29.10.20
 *
 * @brief  Simple sliding puzzle solver for ASM lab
 */

#ifndef __QEMU_BARE__
#define __QEMU_BARE__ 0
#endif

#if __QEMU_BARE__
//#   warning "Compiling for QEMU Bare Metal (U-Boot)"
#   include <common.h>
#   include <exports.h>
#   define calloc(X, Y) (calloc_custom((X), (Y)))

    void *calloc_custom(unsigned int x, unsigned int y) {
        uint8_t *ptr = malloc(x*y);
        for(unsigned int i = 0; i < x*y; ++i) {
            *(ptr+i) = 0;
        }
        return (void *)ptr;
    }

    void memcpy_custom(uint8_t *dest, const uint8_t *src, unsigned int num) {
        while(num--) {
            *dest++ = *src++;
        }
    }
#   define memcpy memcpy_custom
#else
#   warning "Compiling for host"
#   include <stdint.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif

#include "sliding_puzzle.h"
#include "puzzle.h"

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define ABSDIFF(X, Y) (MAX(X,Y) - MIN(X,Y))

//////////////////////////////
// Global function pointers //
//////////////////////////////
void (*swap_function)(uint8_t *, uint8_t *) = swap_c;
//uint32_t (*distance_function)(const uint8_t *) = hamming_score;
uint32_t (*distance_function)(const uint8_t *) = manhattan_distance;

/// @brief Prompts for a key to continue
void prompt_continue(void) {
#if __QEMU_BARE__
	printf("Hit any key to continue ... ");
	while (!tstc());
	/* consume input */
	(void) getc();
	printf("\n\n");
#else
    // Do Nothing
#endif
}

// Node functions
///////////////////

/// @brief Allocates an initial (no prev) node from a given puzzle
Node *node_from_puzzle(const uint8_t* puzzle) {
    Node *node = malloc(sizeof(struct Node));
    uint8_t *puz = malloc(BOARD_MEMORY);
    memcpy(puz, puzzle, BOARD_MEMORY);
    node->puzzle = puz;
    node->moves = 0;
    node->prev = NULL;

    return node;
}

// Make the dual of the puzzle, i.e., exchange two non empty tiles (if one of the tile is empty, well it is a legal swap_c and does not make it dual)
/// @brief Generates a node with the dual of a given puzzle
Node *dual_node_from_puzzle(const uint8_t* puzzle) {
    Node *node = malloc(sizeof(struct Node));
    uint8_t *puz = malloc(BOARD_MEMORY);
    memcpy(puz, puzzle, BOARD_MEMORY);
    for(uint32_t i = 0; i < BOARD_MEMORY; ++i) {
        if (puz[i] && puz[i+1]) {
            swap_c(&(puz[i]), &(puz[i+1]));
            break;
        }
    }
    node->puzzle = puz;
    node->moves = 0;
    node->prev = NULL;

    return node;
}

/// @brief Deallocates a node and associated puzzle
void deallocate_node(Node *node) {
    if (node->puzzle) {
        free((void *)node->puzzle);
    }
    free((void *)node);
}

// Solver Cache / Dynamic memory management
/////////////////////////////////////////////

/// @brief Slow O(n) insert // stupid lookup
uint8_t cache_lookup(Node *node, Cache* cache) {
    uint32_t i = cache->size;
    uint32_t id = get_puzzle_id(node->puzzle);
    while(i > 0) {
        if (cache->entries[i].puzzle_id == id) {
            // Free on hit so we don't keep duplicates
            deallocate_node(node);
            return 1; // Cache hit
        }
        i--;
    }

    if (cache->size == CACHE_CAPACITY) {
        printf("Cache is full\n");
        for(;;);
    }
    // Here we redo the work, but who cares
    i = cache->size;
    while((i > 0) && (id < cache->entries[i-1].puzzle_id)) {
        cache->entries[i] = cache->entries[i-1];
        i--;
    }
    cache->entries[i].node = node;
    cache->entries[i].puzzle_id = id;
    cache->size++;

    return 0;
}

/// @brief Clears the cache and deallocates every cache hit
void clear_cache(Cache* cache) {
    for (uint32_t i = 0; i < cache->size; ++i) {
        deallocate_node(cache->entries[i].node);
    }
    cache->size = 0;
}

// Solver priority queue functions
////////////////////////////////////

/// @brief prints the contents of the queue (for debug)
void print_queue(Queue *queue) {
    for (uint32_t i = 0; i < queue->size; ++i) {
        printf("element 0x%016x prio : %d\n", (unsigned int)queue->elements[i], (unsigned int)queue->priorities[i]);
    }
}

/// @brief clears the queue
void clear_queue(Queue *queue) {
    queue->size = 0;
}

/// @brief adds element to the queue - Enqueue O(N)
void enqueue(Node *ptr, uint32_t priority, Queue* queue) {
    if (queue->size == QUEUE_CAPACITY) {
        printf("Queue is full, element dropped\n");
        for(;;);
        return;
    }

    // Lowest priority last (highest priority first)
    uint32_t i = queue->size;
    while((i > 0) && (priority > queue->priorities[i-1])) {
        queue->elements[i] = queue->elements[i-1];
        queue->priorities[i] = queue->priorities[i-1];
        i--;
    }
    queue->elements[i] = ptr;
    queue->priorities[i] = priority;
    queue->size++;
}

/// @brief Get lowest priority node - Dequeue O(1)
Node *dequeue(Queue* queue) {
    if (queue->size == 0) {
        printf("Empty queue, returning NULL pointer\n");
        return NULL;
    }

    return queue->elements[--(queue->size)];
}

/// @brief Prints a tile
void print_tile(const uint32_t tile_nibble, uint32_t *printed) {
    if (tile_nibble & 0x8) {
        printf("x ");
        (*printed)++;
        if ((*printed % BOARD_SIZE) == 0) {
            printf("\n");
        }
    }
    printf("%d ", (tile_nibble & 0x7) + 1);
    (*printed)++;
    if ((*printed % BOARD_SIZE) == 0) {
        printf("\n");
    }
}

/// @brief Checks if a position is legal
uint8_t legal_position(const uint32_t position) {
    // Check that the space position is set at most once
    // The MSB bit set indicates that the space is in front
    // If no MSB bit is set, the space is in front.
    uint32_t space = position & 0x88888888;
    uint8_t space_set = 0;
    for (uint32_t i = 0; i < 32; ++i) {
        if ((space >> i) & 0x1) {
            if (space_set) {
                printf("Illegal position because space is set twice\n");
                return 0;
            }
            space_set = 1;
        }
    }

    // Check that the values from 0 to 7 are all seen once
    // (representing the values of 1 to 8 of the puzzle)
    uint32_t values = position & 0x77777777;
    uint8_t values_seen = 0;
    for (uint32_t i = 0; i < 8; ++i) {
        uint32_t value = (values >> NIBBLE_SIZE*i) & 0xF;
        values_seen |= 0x1 << value;
    }

    if (values_seen != 0xFF) {
        printf("Illegal position because a value is defined more than once\n");
        return 0;
    }

    return 1;
}

/// @brief Get unique id (at least for 3x3 Boards)
uint32_t get_puzzle_id(const uint8_t* puzzle) {
    uint32_t id = 0;
    for (size_t i = 0; i < BOARD_MEMORY; ++i) {
        id |= ((*(puzzle+i)-1) & 0x7) << (i*4);
        if (*(puzzle+i) == 0) {
            id |= (8 << (i*4));
        }
    }
    return id;
}

/// @brief Print the position of a puzzle
void print_position(const uint32_t position) {
    if (!legal_position(position)) return;
    uint32_t printed = 0;
    for (uint32_t i = 0; i < NUMBER_TILES; ++i) {
        uint32_t tile_nibble = (position >> (NUMBER_TILES-1-i)*NIBBLE_SIZE) & 0xF;
        print_tile(tile_nibble, &printed);
    }
    if (printed < BOARD_SIZE*BOARD_SIZE) {
        printf("x \n");
    }
}

/// @brief Checks if a 3x3 puzzle is legal
uint32_t legal_puzzle(const uint8_t *position) {
    uint32_t seen_values = 0;
    for (uint32_t i = 0; i < BOARD_SIZE; ++i) {
        for (uint32_t j = 0; j < BOARD_SIZE; ++j) {
            seen_values |= 1 << (*position++);
        }
    }
    return (seen_values == 0x1FF);
}

/// @brief prints a puzzle where "=" is the empty tile
void print_puzzle(const uint8_t *position) {
    for (uint32_t i = 0; i < BOARD_SIZE; ++i) {
        for (uint32_t j = 0; j < BOARD_SIZE; ++j) {
            if (*position) {
                printf("%2d", *position);
            } else {
                printf(" =");
            }
            position++;
        }
        printf("\n");
    }
}

/// @brief returns the number of different tiles between two puzzles (puzzle and goal)
uint32_t hamming_distance(const uint8_t *puzzle, const uint8_t *goal) {
    const uint8_t* position = puzzle;
    const uint8_t* goal_position = goal;
    uint32_t hamming_distance = 0;
    for (uint32_t i = 0; i < BOARD_SIZE; ++i) {
        for (uint32_t j = 0; j < BOARD_SIZE; ++j) {
            // If puzzle and goal differ increment distance
            // Taking the empty space into account does not really affect the other algorithms
            if (*position++ != *goal_position++) {
                //printf("%d != %d at %d\n", *(position-1), *(goal_position-1), position-puzzle-1);
                hamming_distance++;
            }
        }
    }
    //print_puzzle(puzzle);
    //print_puzzle(goal);
    return hamming_distance;
}

/// @brief returns the number of different tiles compared to default goal
uint32_t hamming_score(const uint8_t *puzzle) {
    return hamming_distance(puzzle, GOAL);
}

/// @brief The goal is reached if the distance to the goal is zero
uint8_t is_goal(const uint8_t *puzzle) {
    return (hamming_score(puzzle) == 0);
}

// Intended position of value
// .-> x   e.g., "1" is supposed to go at 0,0
// |             "2" is supposed to go at 1,0
// v y           "5" is supposed to go at 1,1 etc.
/// @brief Computes the intended position of a value in the puzzle
Pair intended_position(uint8_t value) {
    Pair xy = {BOARD_SIZE-1, BOARD_SIZE-1};
    if (value == 0) {
        return xy; // Empty space is supposed to go here (arbitrary choice)
    } else {
        xy._1 = (value-1) % BOARD_SIZE;
        xy._2 = (value-1) / BOARD_SIZE;
    }

    return xy;
}

/// @brief computes the manhattan distance between a puzzle and the goal
uint32_t manhattan_distance(const uint8_t* puzzle) {
    uint32_t position = 0;
    uint32_t distance = 0;
    for (uint32_t y = 0; y < BOARD_SIZE; ++y) {
        for (uint32_t x = 0; x < BOARD_SIZE; ++x) {
            Pair xy = intended_position(puzzle[position]);
            //distance += abs(x - xy._1) + abs(y - xy._2); // Will underflow and never become negative with unsigned
            // Doing the abs with max / min allows to use unsigned values without problems
            distance += ABSDIFF(x, xy._1) + ABSDIFF(y, xy._2);
            position++;
        }
    }
    return distance;
}

/// @brief finds the position of the empty tile in a puzzle
uint32_t find_empty_tile(const uint8_t *puzzle) {
    // Find empty tile
    uint32_t space_position = 0;
    for (uint32_t i = 0; i < BOARD_SIZE; ++i) {
        for (uint32_t j = 0; j < BOARD_SIZE; ++j) {
            if (*(puzzle+space_position) == 0) {
                return space_position;
            }
            space_position++;
        }
    }
    return 0; // Not found // Should not happen
}

/// @brief A standard abba swap
void swap_c(uint8_t *a, uint8_t* b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

/// @brief Swaps a tile with the empty space given a direction
uint8_t *swap_tile(uint8_t *puzzle, direction_t direction) {
    uint32_t empty_tile_position = find_empty_tile(puzzle);
    uint8_t *position = puzzle+empty_tile_position;
    switch (direction)
    {
    case left:
        if (empty_tile_position % BOARD_SIZE) {
            swap_function(position-1, position);
        }
        break;
    case right:
        if ((empty_tile_position % BOARD_SIZE) != (BOARD_SIZE-1)) {
            swap_function(position, position+1);
        }
        break;
    case up:
        if (empty_tile_position / BOARD_SIZE) {
            swap_function(position-BOARD_SIZE, position);
        }
        break;
    case down:
        if ((empty_tile_position / BOARD_SIZE) != (BOARD_SIZE-1)) {
            swap_function(position, position+BOARD_SIZE);
        }
        break;

    default:
        break;
    }

    return puzzle;
}

// 2, 3, or 4 neighbors
/// @brief Generates the neighboring puzzles (max 4)
uint32_t create_neighbors(const uint8_t *puzzle, neighbors_t *neighbors) {
    uint32_t empty_tile_position = find_empty_tile(puzzle);

    uint32_t _ = 0;

    // Do the swaps
    if (empty_tile_position % BOARD_SIZE > 0) {
        neighbors->puzzles[_] = malloc(BOARD_MEMORY);
        memcpy(neighbors->puzzles[_], puzzle, BOARD_MEMORY);
        swap_tile(neighbors->puzzles[_], left);
        _++;
    }
    if ((empty_tile_position % BOARD_SIZE) < BOARD_SIZE-1) {
        neighbors->puzzles[_] = malloc(BOARD_MEMORY);
        memcpy(neighbors->puzzles[_], puzzle, BOARD_MEMORY);
        swap_tile(neighbors->puzzles[_], right);
        _++;
    }
    if ((empty_tile_position / BOARD_SIZE) > 0) {
        neighbors->puzzles[_] = malloc(BOARD_MEMORY);
        memcpy(neighbors->puzzles[_], puzzle, BOARD_MEMORY);
        swap_tile(neighbors->puzzles[_], up);
        _++;
    }
    if ((empty_tile_position / BOARD_SIZE) < BOARD_SIZE-1) {
        neighbors->puzzles[_] = malloc(BOARD_MEMORY);
        memcpy(neighbors->puzzles[_], puzzle, BOARD_MEMORY);
        swap_tile(neighbors->puzzles[_], down);
        _++;
    }

    // Here the neighbors are ready
    return _;
}

// This goes through the previous nodes until first node (prev = NULL) has been reached and prints them in reverse order (i.e., from initial position to solution)
/// @brief Prints the path to a given node based on the previous nodes (used to show path to solution)
void print_path_to_node(Node *solution_node) {
    uint32_t num_steps = 0;
    Node *node = solution_node;
    while (node->prev) {
        num_steps++;
        node = node->prev;
    }

    Node **stack = malloc(sizeof(Node *) * num_steps);
    node = solution_node;
    for (uint32_t i = 0; i < num_steps; ++i) {
        *(stack+i) = node->prev;
        node = node->prev;
    }
    for (uint32_t i = 0; i < num_steps; ++i) {
        print_puzzle((*(stack+num_steps-i-1))->puzzle);
        printf("=>\n");
    }
    print_puzzle(solution_node->puzzle);
    free((void *)stack);
}

// A* search for a solution
/// @brief Does a single step towards a solution (A* search step)
Node *a_star_search_step(Queue *queue, Cache *cache) {
    Node *working_node = dequeue(queue);

    if (working_node) {
        if ((*distance_function)(working_node->puzzle) == 0) {
            // Goal reached !
            return working_node;
        }

        neighbors_t neighbors = {{NULL, NULL, NULL, NULL}};
        // Create neighbors allocates the puzzles (up to 4)
        uint32_t num_neighbors = create_neighbors(working_node->puzzle, &neighbors);

        for (uint32_t i = 0; i < num_neighbors; ++i) {
            const uint8_t* puz = neighbors.puzzles[i];
            uint32_t distance = (*distance_function)(puz);
            Node *node = malloc(sizeof(struct Node));
            node->puzzle = puz;
            node->moves = working_node->moves + 1;
            node->prev = working_node;

            // Don't queue positions that have already been seen
            if (!cache_lookup(node, cache)) {
                // If "Miss" enqueue (the cache deallocates on "Hit" in order to free duplicates)
                enqueue(node, distance + node->moves, queue);
            }
        }
    } else {
        return (Node *)-1;
    }

    // Goal not yet reached
    return NULL;
}

// Lock-stepped A* search in the puzzle and dual puzzle search space
/// @brief Searches for a solution for a given puzzle by doing a lock-stepped dual A* search
uint32_t dual_a_star_search(const uint8_t* puzzle) {
    uint32_t correct = 1;

    // Data structures
    Cache *cache = calloc(sizeof(Cache), 1);
    Cache *dual_cache = calloc(sizeof(Cache), 1);
    Queue *queue = calloc(sizeof(Queue), 1);
    Queue *dual_queue = calloc(sizeof(Queue), 1);

    Node *initial_position = node_from_puzzle(puzzle);
    // Dual is puzzle with two non empty tiles swapped
    Node *dual_initial_position = dual_node_from_puzzle(puzzle);

    // Enqueue inital position
    enqueue(initial_position, (*distance_function)(puzzle) + initial_position->moves, queue);
    cache_lookup(initial_position, cache);
    // Enqueue dual inital position
    enqueue(dual_initial_position, (*distance_function)(puzzle) + dual_initial_position->moves, dual_queue);
    cache_lookup(dual_initial_position, dual_cache);

    // Lock step solve puzzle and dual puzzle
    // i.e., launch the solver
    for (;;) {
        Node* solution = NULL;
        solution = a_star_search_step(queue, cache);
        if (solution == (Node *)-1) {
            printf("Search space exhausted... no solution found\n");
            correct = 0;
            break;
        } else if (solution) {
            printf("Solution found !\n");
            print_path_to_node(solution);
            if (hamming_score(solution->puzzle)) {
                printf("The solution found is incorrect !\n");
                correct = 0;
            }
            break;
        }
        solution = a_star_search_step(dual_queue, dual_cache);
        if (solution == (Node *)-1) {
            printf("Search space exhausted... no solution found\n");
            correct = 0;
            break;
        } else if (solution) {
            printf("Puzzle has no solution\n");
            break;
        }
    }

    // Free the memory, here clear_cache() deallocates the puzzles e.g., allocated by create_neighbors() it also deallocates the nodes.
    clear_queue(queue);
    clear_cache(cache);
    free(queue);
    free(cache);
    clear_queue(dual_queue);
    clear_cache(dual_cache);
    free(dual_queue);
    free(dual_cache);

    return correct;
}

/// @brief The main entrypoint of the application
int main(int argc, char *argv[]) {
    int err = 0;

    // Show puzzle
    printf("Puzzle is : \n");
    print_puzzle((const uint8_t *)puzzle);

    // If puzzle is illegal return
    if (!legal_puzzle((const uint8_t *) puzzle)) {
        printf("The puzzle is not legal !\n");
        err = -1;
        goto exit;
    }

    // Testing the swap function
    swap_function = swap_asm;
    uint8_t a[7] = {0xAA,0x55,0xAA,42,0x55,0xAA,0x55};
    uint8_t b[7] = {0,0,0,88,0,0,0};
    uint8_t *ptr_a = a+3;
    uint8_t *ptr_b = b+3;
    swap_function(ptr_a, ptr_b);
    if ((a[3] != 88) || (b[3] != 42)) {
        printf("[ASM] swap_asm() seems incorrect...\n");
        printf("The C version of the swapping function will be used\n");
        swap_function = swap_c;
        prompt_continue();
    } else {
        *ptr_a = 88;
        *ptr_b = 0;
        for (int i = 0; i < 7; ++i) {
            if ((a[i] == 0) || b[i]) {
                swap_function = swap_c;
            }
        }
    }

    // Solving the puzzle with the C version
    printf("Solving with Hamming distance\n");
    distance_function = hamming_score;
    dual_a_star_search((const uint8_t *)puzzle);
    prompt_continue();
    // printf("Solving with Manhattan distance\n");
    // distance_function = manhattan_distance;
    // dual_a_star_search((const uint8_t *)puzzle);
    // prompt_continue();

    // Solving the puzzle with student hamming distance
    printf("Solving with Hamming distance ASM\n");
    distance_function = hamming_distance_asm;
    if (!dual_a_star_search((const uint8_t *) puzzle)) {
        printf("[ASM] hamming_distance_asm() seems incorrect...\n");
    }
    prompt_continue();
    printf("Solving with Manhattan distance ASM\n");
    distance_function = manhattan_distance_asm;
    if (!dual_a_star_search((const uint8_t *) puzzle)) {
        printf("[ASM] manhattan_distance_asm() seems incorrect...\n");
    }
    prompt_continue();

    // Example of an impossible puzzle
    printf("Trying to solve an impossible puzzle\n");
    print_puzzle((const uint8_t *)impossible_puzzle);
    distance_function = hamming_score;
    dual_a_star_search((const uint8_t *)impossible_puzzle);

exit:
#if __QEMU_BARE__
	printf("Hit any key to exit ... ");
	while (!tstc());

	/* consume input */
	(void) getc();

	printf("\n\n");
#endif

	return err;
}

#ifdef __QEMU_BARE__
#undef calloc
#undef memcpy
#endif
#undef MIN
#undef MAX

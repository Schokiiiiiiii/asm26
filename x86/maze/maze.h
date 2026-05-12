/**
 * @file   maze.h
 * @brief  Header for the maze traversal exercise for ASM lab
 */

#ifndef __MAZE_H__
#define __MAZE_H__

#include <stdint.h>

typedef const char data_t;

/// @brief Room for the maze graph
struct Room;
typedef struct Room {
    data_t      *data;     
    struct Room *north;   
    struct Room *east;    
    struct Room *south;   
    struct Room *west;    
    uint32_t     visited; 
} Room;

#if __QEMU_BARE__
    // Student assembly code
    extern void traverse_maze_asm(Room *start);
#else
    // Empty function to emulate empty assembly code on host
    void traverse_maze_asm(Room *start) {}
#endif

#endif /* __MAZE_H__ */
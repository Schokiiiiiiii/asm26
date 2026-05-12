/**
 * @file   maze.c
 * @brief  Maze traversal exercise for ASM lab
 */

#ifndef __QEMU_BARE__
#define __QEMU_BARE__ 0
#endif

#if __QEMU_BARE__
#   include <common.h>
#   include <exports.h>
#else
#   warning "Compiling for host"
#   include <stdint.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif

#include "maze.h"

#define MAX_ROOMS 1024

/// @brief Dynamically allocates a room
Room *allocate_room(data_t *data) {
    Room *room = malloc(sizeof(Room));

    if (room == NULL) {
        printf("malloc() failed !\n");
        return NULL;
    }

    room->data = data;
    room->north = NULL;
    room->east = NULL;
    room->south = NULL;
    room->west = NULL;
    room->visited = 0;

    return room;
}

/// @brief Connects two rooms in one direction only
void connect_two_way(Room *from, Room *to, char direction) {
    if (from == NULL) return;

    switch (direction) {
        case 'N':
            from->north = to;
            to->south = from;
            break;
        case 'E':
            from->east = to;
            to->west = from;
            break;
        case 'S':
            from->south = to;
            to->north = from;
            break;
        case 'W':
            from->west = to;
            to->east = from;
            break;
        default:
            break;
    }
}

/// @brief Resets visited flag recursively-ish using a simple array
void reset_rooms(Room *rooms[], uint32_t count) {
    for (uint32_t i = 0; i < count; ++i) {
        if (rooms[i]) {
            rooms[i]->visited = 0;
        }
    }
}

/// @brief Frees all allocated rooms
void free_rooms(Room *rooms[], uint32_t count) {
    for (uint32_t i = 0; i < count; ++i) {
        free(rooms[i]);
        rooms[i] = NULL;
    }
}

/// @brief The main entrypoint of the application
int main(int argc, char *argv[]) {
    int err = 0;
    printf("First maze expected output:\n");
    printf("Entrance Corridor Throne_room Library Armory\n\n");

    Room *entrance = allocate_room("Entrance");
    Room *corridor = allocate_room("Corridor");
    Room *armory   = allocate_room("Armory");
    Room *throne   = allocate_room("Throne_room");
    Room *library  = allocate_room("Library");

    Room *maze1[] = {
        entrance, corridor, armory, throne, library
    };

    /*
     * Structure:
     *
     *   Corridor --- throne_room --- library
     *     |
     * Entrance --- Armory
     *
     * From Entrance, North is explored before East.
     */

    connect_two_way(entrance, corridor, 'N');
    connect_two_way(entrance, armory,   'E');
    connect_two_way(corridor, throne,   'E');
    connect_two_way(throne, library,    'E');

    printf("First maze output:\n");
    traverse_maze_asm(entrance);
    reset_rooms(maze1, sizeof(maze1) / sizeof(maze1[0]));

    printf("\n\n---------------\n\n");

    printf("Second maze expected output:\n");
    printf("Gate Watchtower Courtyard Kitchen Cellar Crypt Dungeon Bedroom\n\n");

    Room *gate       = allocate_room("Gate");
    Room *watchtower = allocate_room("Watchtower");
    Room *bedroom    = allocate_room("Bedroom");
    Room *courtyard  = allocate_room("Courtyard");
    Room *kitchen    = allocate_room("Kitchen");
    Room *cellar     = allocate_room("Cellar");
    Room *dungeon    = allocate_room("Dungeon");
    Room *crypt      = allocate_room("Crypt");

    Room *silent_dead_end = allocate_room(NULL);

    Room *maze2[] = {
        gate, watchtower, courtyard, kitchen,
        cellar, dungeon, crypt, silent_dead_end, bedroom
    };

    /*
     * Structure (entrance is gate):
     *
     *          Watchtower
     *             |
     * Bedroom -- Gate ----- Courtyard -- Kitchen
     *             |             |
     *           Dungeon       Cellar -- Crypt
     *
     * silent_dead_end has no data, so it can be visited without printing.
     */

    connect_two_way(gate, watchtower, 'N');
    connect_two_way(gate, courtyard,  'E');
    connect_two_way(gate, bedroom,  'W');

    connect_two_way(courtyard, kitchen, 'E');
    connect_two_way(courtyard, dungeon, 'S');

    connect_two_way(kitchen, cellar, 'S');
    connect_two_way(cellar, crypt,   'E');

    connect_two_way(dungeon, silent_dead_end, 'S');

    printf("Second maze output:\n");
    traverse_maze_asm(gate);
    reset_rooms(maze2, sizeof(maze2) / sizeof(maze2[0]));

    printf("\n\n---------------\n\n");

    printf("Third maze expected output:\n");
    printf("Hall Gallery Chapel Archives Treasury Stables Garden\n\n");

    Room *hall     = allocate_room("Hall");
    Room *gallery  = allocate_room("Gallery");
    Room *chapel   = allocate_room("Chapel");
    Room *archives = allocate_room("Archives");
    Room *treasury = allocate_room("Treasury");
    Room *garden   = allocate_room("Garden");
    Room *stables  = allocate_room("Stables");

    Room *maze3[] = {
        hall, gallery, chapel, archives, treasury, garden, stables
    };

    /*
     * Structure with several cycles:
     *
     * Gallery -- Chapel
     *   |          |
     * Hall ---- Archives -- Treasury
     *              |          |
     *            Garden -- Stables
     *
     * With cycles
     */

    connect_two_way(hall, gallery,  'N');
    connect_two_way(hall, archives, 'E');

    connect_two_way(gallery, chapel, 'E');
    connect_two_way(chapel, archives, 'S');

    connect_two_way(archives, treasury, 'E');
    connect_two_way(archives, garden, 'S');

    connect_two_way(garden, stables,  'E');
    connect_two_way(stables, treasury,  'N');

    printf("Third maze output:\n");
    traverse_maze_asm(hall);
    reset_rooms(maze3, sizeof(maze3) / sizeof(maze3[0]));

    printf("\n\n---------------\n\n");

    printf("Fourth maze expected output:\n");
    printf("Start Room_1 Room_2 Room_3 Exit\n\n");

    Room *start = allocate_room("Start");
    Room *room1 = allocate_room("Room_1");
    Room *room2 = allocate_room("Room_2");
    Room *room3 = allocate_room("Room_3");
    Room *exit  = allocate_room("Exit");

    Room *maze4[] = {
        start, room1, room2, room3, exit
    };

    /*
     * Degenerate maze, like a linked list:
     *
     * Start -> Room_1 -> Room_2 -> Room_3 -> Exit
     */

    connect_two_way(start, room1, 'E');
    connect_two_way(room1, room2, 'E');
    connect_two_way(room2, room3, 'E');
    connect_two_way(room3, exit,  'E');

    printf("Fourth maze output:\n");
    traverse_maze_asm(start);
    reset_rooms(maze4, sizeof(maze4) / sizeof(maze4[0]));

    printf("\n\n---------------\n\n");

#if !__QEMU_BARE__
    free_rooms(maze1, sizeof(maze1) / sizeof(maze1[0]));
    free_rooms(maze2, sizeof(maze2) / sizeof(maze2[0]));
    free_rooms(maze3, sizeof(maze3) / sizeof(maze3[0]));
    free_rooms(maze4, sizeof(maze4) / sizeof(maze4[0]));
#endif

#if __QEMU_BARE__
    printf("Hit any key to exit ... ");
    while (!tstc());

    (void) getc();

    printf("\n\n");
#endif

    return err;
}

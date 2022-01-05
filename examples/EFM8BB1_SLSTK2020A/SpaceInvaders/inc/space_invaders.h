/////////////////////////////////////////////////////////////////////////////
// Space_invaders.h
/////////////////////////////////////////////////////////////////////////////

#ifndef SPACE_INVADERS_H_
#define SPACE_INVADERS_H_

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "space_invaders_config.h"

/////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////

#define MAX_LEVEL                       9

#define GAME_OVER_WAIT_TIME             2000
#define NEW_LEVEL_WAIT_TIME             2000
#define GAME_OVER_RESET_TIMEOUT         60000

// Player sprite dimensions
#define PLAYER_HEIGHT                   player_height
#define PLAYER_WIDTH                    player_width
#define PLAYER_HIT_LEFT_MARGIN          1
#define PLAYER_HIT_RIGHT_MARGIN         2
#define PLAYER_HIT_WIDTH                (player_width - PLAYER_HIT_RIGHT_MARGIN - PLAYER_HIT_LEFT_MARGIN)

// Area limits between score lines and player lines
// Both lines can be used to draw enemies/projectiles
#define TOP_LINE_LIMIT                  (FONT_HEIGHT + 1)
#define BOTTOM_LINE_LIMIT               (DISP_HEIGHT - PLAYER_HEIGHT - 1)

// Enemy swarm dimensions
#define ENEMY_COLS                      5
#define ENEMY_ROWS                      3
#define ENEMY_HEIGHT                    alien_a_height
#define ENEMY_WIDTH                     alien_a_width
#define ENEMY_HIT_LEFT_MARGIN           2
#define ENEMY_HIT_RIGHT_MARGIN          2
#define ENEMY_HIT_WIDTH                 (alien_a_width - ENEMY_HIT_LEFT_MARGIN - ENEMY_HIT_RIGHT_MARGIN)
#define TOTAL_ENEMY_HEIGHT              (ENEMY_HEIGHT * ENEMY_ROWS)
#define TOTAL_ENEMY_WIDTH               (ENEMY_WIDTH * ENEMY_COLS)

// Player bullet dimensions
#define BULLET_WIDTH                    bullet_width
#define BULLET_HEIGHT                   bullet_height
#define BULLET_HIT_LEFT_MARGIN          3
#define BULLET_HIT_RIGHT_MARGIN         4
#define BULLET_HIT_WIDTH                (bullet_width - BULLET_HIT_RIGHT_MARGIN - BULLET_HIT_LEFT_MARGIN)

// Enemy bolt dimensions
#define BOLT_WIDTH                      bolt_a_width
#define BOLT_HEIGHT                     bolt_a_height
#define BOLT_HIT_LEFT_MARGIN            2
#define BOLT_HIT_RIGHT_MARGIN           3
#define BOLT_HIT_WIDTH                  (bolt_a_width - BOLT_HIT_RIGHT_MARGIN - BOLT_HIT_LEFT_MARGIN)

/////////////////////////////////////////////////////////////////////////////
// Macros
/////////////////////////////////////////////////////////////////////////////

// Return true if object a intersects object b (overlaps by at least 1 px)
#define OBJ_OVERLAPS_OBJ(a, a_len, b, b_len)    (a + a_len > b && a < b + b_len)

// Return true if line a intersects object b
#define LINE_OVERLAPS_OBJ(a, b, b_len)          OBJ_OVERLAPS_OBJ(a, 1, b, b_len)

/////////////////////////////////////////////////////////////////////////////
// Debug Macros
/////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
#define DEBUG_ASSERT(expr)              if (!expr) while(1);
#else
#define DEBUG_ASSERT(expr)
#endif

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

void SpaceInvaders_Main();

#endif /* SPACE_INVADERS_H_ */

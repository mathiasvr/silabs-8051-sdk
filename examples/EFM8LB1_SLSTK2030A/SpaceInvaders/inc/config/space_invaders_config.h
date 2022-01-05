/////////////////////////////////////////////////////////////////////////////
// space_invaders_config.h
/////////////////////////////////////////////////////////////////////////////

#ifndef SPACE_INVADERS_CONFIG_H_
#define SPACE_INVADERS_CONFIG_H_

/////////////////////////////////////////////////////////////////////////////
// Configuration
/////////////////////////////////////////////////////////////////////////////

// Set to 1 to enable debug mode, showing dirty lines
#define DEBUG_SHOW_DIRTY_LINES          0
#define DEBUG_SHOW_DIRTY_LINES_DELAY    100

// Set to 1 to show when a hit is detected
#define DEBUG_SHOW_HIT_DETECTION        0
#define DEBUG_SHOW_HIT_DETECTION_DELAY  500

// Enemy swarm x & y advancement rate
#define ENEMY_ADVANCE_X                 1
#define ENEMY_ADVANCE_Y                 2

// Player x advancement rate
#define PLAYER_ADVANCE_X                1

// Projectile y advancement rate
#define BULLET_ADVANCE_Y                2
#define BOLT_ADVANCE_Y                  1

// LCD refresh rate in Hz
#define GAME_FRAME_RATE                 50

#endif /* SPACE_INVADERS_CONFIG_H_ */

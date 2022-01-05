/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Space_invaders.c
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "bsp.h"
#include "utils.h"
#include "joystick.h"
#include "tick.h"
#include "disp.h"
#include "render.h"
#include "space_invaders.h"

// Standard library
#include <string.h>
#include "retargetserial.h"
#include <stdlib.h>

// Sprites
#include "alien_a.h"
#include "alien_b.h"
#include "metroid_a.h"
#include "metroid_b.h"
#include "squid.h"
#include "player.h"
#include "bullet.h"
#include "bolt_a.h"
#include "bolt_b.h"

// Scene Layout
// ------------
//  _______________________
// |                       |
// | LEVEL 1   SCORE:    0 |
// | A    A'   A ^  A'   A |
// | M    M'   M |  M'   M |
// | S    S    S |  S    S |
// |      |      |         |
// |      |      B         |
// |      Z      |         |
// |      |      |         |
// |      v      P         |
// |_______________________|
//
// Where:
// A  = Enemy sprite (alien_a)
// A' = Enemy sprite (alien_b)
// M  = Enemy sprite (metroid_a)
// M' = Enemy sprite (metroid_b)
// S  = Enemy sprite (squid)
// P  = Player sprite (player)
// Z  = Enemy projectile sprite (bolt)
// B  = Player projectile sprite (bullet)

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

void RenderCleanDirty();
void RenderScore(uint8_t reset, uint16_t offset);
void RenderLevelScreen();
void RenderGameOverScreen();
void RenderScene();

void InputPlayer();

void StateFireBullet();
void StateFireBolt();
void StateAdvanceEnemy();
void StateAdvanceScene();

void HitDetectEnemy();
void HitDetectPlayer();

void GameNextLevel();
void GameLevelReset();
void GameReset();
void GameInit();
void GameLoop();
void GameSynchFrame();

/////////////////////////////////////////////////////////////////////////////
// Globals
/////////////////////////////////////////////////////////////////////////////

// Generic line buffer
SI_SEGMENT_VARIABLE(Line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

/////////////////////////////////////////////////////////////////////////////
// Globals - Game
/////////////////////////////////////////////////////////////////////////////

uint16_t GameLevel;
uint16_t GameScore;
bool GameOver;
bool AnimateEnemy;
bool AnimateBolt;

// Game over text
SI_SEGMENT_VARIABLE(GameOverStr[], char, RENDER_STR_SEG) = "Game Over";

// Enemy advance rate for each level (1/100 fixed point)
SI_SEGMENT_VARIABLE(EnemyAdvanceRate[MAX_LEVEL], uint16_t, const SI_SEG_CODE) = {
    10,  // Level 1
    20,  // Level 2
    25,  // Level 3
    50,  // Level 4
    75,  // Level 5
    100, // Level 6
    125, // Level 7
    150, // Level 8
    200, // Level 9
};

SI_SEGMENT_VARIABLE(EnemyScoreValue[ENEMY_ROWS], uint8_t, const SI_SEG_CODE) = {
    30, // Row 1
    20, // Row 2
    10, // Row 3
};

/////////////////////////////////////////////////////////////////////////////
// Globals - Enemy
/////////////////////////////////////////////////////////////////////////////

// Enemy sprite lookup table
SI_SEGMENT_VARIABLE_SEGMENT_POINTER(EnemySprites[2][ENEMY_ROWS][ENEMY_COLS], uint8_t, RENDER_SPRITE_SEG, SI_SEG_IDATA) = {
   // Default sprites
   {{ alien_a_bits,    alien_b_bits,      alien_a_bits,      alien_b_bits,      alien_a_bits },
    { metroid_a_bits,  metroid_b_bits,    metroid_a_bits,    metroid_b_bits,    metroid_a_bits },
    { alien_a_bits,    alien_b_bits,      alien_a_bits,      alien_b_bits,      alien_a_bits }},

   // Alternate sprites (for animation)
   {{ alien_b_bits,    alien_a_bits,      alien_b_bits,      alien_a_bits,      alien_b_bits },
    { metroid_b_bits,  metroid_a_bits,    metroid_b_bits,    metroid_a_bits,    metroid_b_bits },
    { alien_b_bits,    alien_a_bits,      alien_b_bits,      alien_a_bits,      alien_b_bits }},
};

// Keep track of which enemies are alive
SI_SEGMENT_VARIABLE(EnemyAlive[ENEMY_ROWS][ENEMY_COLS], uint8_t, SI_SEG_IDATA);

uint8_t Enemy_x;
uint8_t Enemy_y;
uint16_t Enemy_x_fp;
bool EnemyAdvanced;
bool EnemyDirectionRight;
uint8_t EnemiesRemaining;

bool EnemyFire;
uint8_t Bolt_x;
uint8_t Bolt_y;

/////////////////////////////////////////////////////////////////////////////
// Globals - Player
/////////////////////////////////////////////////////////////////////////////

uint8_t Player_x;
uint8_t Player_y;

bool PlayerFire;
uint8_t Bullet_x;
uint8_t Bullet_y;
bool CleanDirtyBulletLines;

/////////////////////////////////////////////////////////////////////////////
// Functions - Render
/////////////////////////////////////////////////////////////////////////////

// Clear any lines that won't be rendered this frame (such as projectile trails)
void RenderCleanDirty()
{
    uint8_t line;
    uint8_t dirtyLine;

    /////////////////
    // Enemy Trail //
    /////////////////

    // Clear lines from enemy trail (one shot)
    if (EnemyAdvanced)
    {
        EnemyAdvanced = false;

        for (line = 0; line < ENEMY_ADVANCE_Y; line++)
        {
#if DEBUG_SHOW_DIRTY_LINES
            DISP_ClearLine(Enemy_y - line - 1, DISP_FOREGROUND_COLOR);
            Wait(DEBUG_SHOW_DIRTY_LINES_DELAY);
#endif
            DISP_ClearLine(Enemy_y - line - 1, DISP_BACKGROUND_COLOR);
        }
    }

    //////////////////////
    // Enemy Bolt Trail //
    //////////////////////

    // Clear lines from enemy projectile trail
    if (EnemyFire)
    {
        for (line = 0; line < BOLT_ADVANCE_Y; line++)
        {
            dirtyLine = Bolt_y - line - 1;

            // Don't clean if line will be overwritten by enemy or player lines
            if (dirtyLine >= Enemy_y + TOTAL_ENEMY_HEIGHT &&
                dirtyLine <= BOTTOM_LINE_LIMIT)
            {
                // Don't clean if line will be overwritten by bullet lines
                if (!PlayerFire ||
                    !LINE_OVERLAPS_OBJ(dirtyLine, Bullet_y, BULLET_HEIGHT))
                {
#if DEBUG_SHOW_DIRTY_LINES
                    DISP_ClearLine(dirtyLine, DISP_FOREGROUND_COLOR);
                    Wait(DEBUG_SHOW_DIRTY_LINES_DELAY);
#endif
                    DISP_ClearLine(dirtyLine, DISP_BACKGROUND_COLOR);
                }
            }
        }
    }

    /////////////////////////
    // Player Bullet Trail //
    /////////////////////////

    // Clear lines from player projectile trail
    if (PlayerFire)
    {
        for (line = 0; line < BULLET_ADVANCE_Y; line++)
        {
            dirtyLine = Bullet_y + BULLET_HEIGHT + line;

            // Don't clean if line will be overwritten by enemy or player lines
            if (dirtyLine >= Enemy_y + TOTAL_ENEMY_HEIGHT &&
                dirtyLine <= BOTTOM_LINE_LIMIT)
            {
                // Don't clean if line will be overwritten by bolt lines
                if (!EnemyFire ||
                    !LINE_OVERLAPS_OBJ(dirtyLine, Bolt_y, BOLT_HEIGHT))
                {
#if DEBUG_SHOW_DIRTY_LINES
                    DISP_ClearLine(dirtyLine, DISP_FOREGROUND_COLOR);
                    Wait(DEBUG_SHOW_DIRTY_LINES_DELAY);
#endif
                    DISP_ClearLine(dirtyLine, DISP_BACKGROUND_COLOR);
                }
            }
        }
    }

    /////////////////////////////////////////
    // Player Bullet Trail (Entire Bullet) //
    /////////////////////////////////////////

    // Clean lines from player projectile trail after killing
    // an enemy (one shot)
    if (CleanDirtyBulletLines)
    {
        CleanDirtyBulletLines = false;

        for (line = 0; line < BULLET_HEIGHT + BULLET_ADVANCE_Y; line++)
        {
            dirtyLine = Bullet_y + line;
#if DEBUG_SHOW_DIRTY_LINES
            DISP_ClearLine(dirtyLine, DISP_FOREGROUND_COLOR);
            Wait(DEBUG_SHOW_DIRTY_LINES_DELAY);
#endif
            DISP_ClearLine(dirtyLine, DISP_BACKGROUND_COLOR);
        }
    }
}

// Update/reset the score
void RenderScore(uint8_t reset, uint16_t offset)
{
    uint8_t i;
    char scoreStr[25];

    if (reset)
        GameScore = 0;
    else
        GameScore += offset;

    RETARGET_SPRINTF(scoreStr, "LEVEL %d  SCORE:%d", (int16_t)GameLevel+1, (int16_t)GameScore);

    for (i = 0; i < FONT_HEIGHT; i++)
    {
        RENDER_ClrLine(Line);
        RENDER_StrLine(Line, 4, i, scoreStr);
        DISP_WriteLine(i + 1, Line);
    }
}

void RenderLevelScreen()
{
    uint8_t i;
    uint8_t dispLine;
    uint8_t strWidth;
    char str[10];

    RETARGET_SPRINTF(str, "LEVEL %d", (int16_t)GameLevel+1);
    strWidth = RENDER_GetStrSize(str);

    for (i = 0; i < FONT_HEIGHT; i++)
    {
        dispLine = ((DISP_HEIGHT - FONT_HEIGHT) / 2) + i;
        RENDER_ClrLine(Line);
        RENDER_StrLine(Line, (DISP_WIDTH - strWidth) / 2, i, str);
        DISP_WriteLine(dispLine, Line);
    }
}

void RenderGameOverScreen()
{
    uint8_t i;
    uint8_t dispLine;
    uint8_t strWidth = RENDER_GetStrSize(GameOverStr);

    for (i = 0; i < FONT_HEIGHT; i++)
    {
        dispLine = ((DISP_HEIGHT - FONT_HEIGHT) / 2) + i;
        RENDER_ClrLine(Line);
        RENDER_StrLine(Line, (DISP_WIDTH - strWidth) / 2, i, GameOverStr);
        DISP_WriteLine(dispLine, Line);
    }
}

// Draw the frame to the screen
void RenderScene()
{
    uint8_t row, col;
    uint8_t line;
    uint8_t boltLine;
    uint8_t bulletLine;
    uint8_t playerLine;
    uint8_t dispLine = Enemy_y;
    uint8_t lineStart;
    uint8_t lineEnd;

    ///////////////////////
    // Clean Dirty Lines //
    ///////////////////////

    // Clear any lines that won't be rendered this frame (such as projectile trails)
    RenderCleanDirty();

    //////////////////////
    // Draw Enemy Swarm //
    //////////////////////

    for (row = 0; row < ENEMY_ROWS; row++)
    {
        for (line = 0; line < ENEMY_HEIGHT; line++)
        {
            RENDER_ClrLine(Line);

            // Draw enemy
            for (col = 0; col < ENEMY_COLS; col++)
            {
                if (EnemyAlive[row][col])
                {
                    if (AnimateEnemy)
                        RENDER_SpriteLine(Line, Enemy_x + col * ENEMY_WIDTH, line, EnemySprites[0][row][col], ENEMY_WIDTH);
                    else
                        RENDER_SpriteLine(Line, Enemy_x + col * ENEMY_WIDTH, line, EnemySprites[1][row][col], ENEMY_WIDTH);
                }
            }

            // Draw enemy bolt lines that overlap enemies
            if (EnemyFire)
            {
                // Check that the enemy line overlaps the bolt
                if (LINE_OVERLAPS_OBJ(dispLine, Bolt_y, BOLT_HEIGHT))
                {
                    boltLine = dispLine - Bolt_y;
                    if (AnimateBolt)
                        RENDER_SpriteLineForeground(Line, Bolt_x, boltLine, bolt_a_bits, BOLT_WIDTH);
                    else
                        RENDER_SpriteLineForeground(Line, Bolt_x, boltLine, bolt_b_bits, BOLT_WIDTH);
                }
            }

            // Draw player bullet lines that overlap enemies
            if (PlayerFire)
            {
                // Check that the enemy line overlaps the bullet
                if (LINE_OVERLAPS_OBJ(dispLine, Bullet_y, BULLET_HEIGHT))
                {
                    bulletLine = dispLine - Bullet_y;
                    RENDER_SpriteLineForeground(Line, Bullet_x, bulletLine, bullet_bits, BULLET_WIDTH);
                }
            }

            // Draw current display line to screen
            DISP_WriteLine(dispLine, Line);
            dispLine++;
        }
    }

    /////////////////////
    // Draw Enemy Bolt //
    /////////////////////

    if (EnemyFire)
    {
        // Don't draw sprite lines that overlap with enemy lines
        // (draw that in the enemy render code above)
        if (Bolt_y > Enemy_y + TOTAL_ENEMY_HEIGHT)
            lineStart = 0;
        else
            lineStart = Enemy_y + TOTAL_ENEMY_HEIGHT - Bolt_y;

        // Don't draw sprite lines that overlap with player lines
        // (draw that in the player render code below)
        if (Bolt_y + BOLT_HEIGHT <= Player_y)
            lineEnd = BOLT_HEIGHT;
        else
            lineEnd = Player_y - Bolt_y;

        for (line = lineStart; line < lineEnd; line++)
        {
            boltLine = Bolt_y + line;

            // Skip over lines that overlap with the bullet
            if (PlayerFire)
            {
                if (boltLine >= Bullet_y &&
                    boltLine < Bullet_y + BULLET_HEIGHT)
                    continue;
            }

            RENDER_ClrLine(Line);
            if (AnimateBolt)
                RENDER_SpriteLine(Line, Bolt_x, line, bolt_a_bits, BOLT_WIDTH);
            else
                RENDER_SpriteLine(Line, Bolt_x, line, bolt_b_bits, BOLT_WIDTH);
            DISP_WriteLine(boltLine, Line);
        }
    }

    ////////////////////////
    // Draw Player Bullet //
    ////////////////////////

    if (PlayerFire)
    {
        // Don't draw sprite lines that overlap with enemy lines
        // (draw that in the enemy render code above)
        if (Bullet_y > Enemy_y + TOTAL_ENEMY_HEIGHT)
            lineStart = 0;
        else
            lineStart = Enemy_y + TOTAL_ENEMY_HEIGHT - Bullet_y;

        for (line = lineStart; line < BULLET_HEIGHT; line++)
        {
            bulletLine = Bullet_y + line;
            RENDER_ClrLine(Line);

            // Draw the bolt and overlay it below the bullet
            // to prevent flicker
            if (EnemyFire)
            {
                // Check if current line overlaps the bolt sprite
                if (LINE_OVERLAPS_OBJ(bulletLine, Bolt_y, BOLT_HEIGHT))
                {
                    if (AnimateBolt)
                        RENDER_SpriteLine(Line, Bolt_x, bulletLine - Bolt_y, bolt_a_bits, BOLT_WIDTH);
                    else
                        RENDER_SpriteLine(Line, Bolt_x, bulletLine - Bolt_y, bolt_b_bits, BOLT_WIDTH);
                }
            }

            RENDER_SpriteLineForeground(Line, Bullet_x, line, bullet_bits, BULLET_WIDTH);
            DISP_WriteLine(bulletLine, Line);
        }
    }

    /////////////////
    // Draw Player //
    /////////////////

    for (line = 0; line < PLAYER_HEIGHT; line++)
    {
        playerLine = Player_y + line;
        RENDER_ClrLine(Line);

        // Draw enemy bolt lines that overlap player
        if (EnemyFire)
        {
            // Check if current line overlaps the bolt sprite
            if (LINE_OVERLAPS_OBJ(playerLine, Bolt_y, BOLT_HEIGHT))
            {
                if (AnimateBolt)
                    RENDER_SpriteLine(Line, Bolt_x, playerLine - Bolt_y, bolt_a_bits, BOLT_WIDTH);
                else
                    RENDER_SpriteLine(Line, Bolt_x, playerLine - Bolt_y, bolt_b_bits, BOLT_WIDTH);
            }
        }

        RENDER_SpriteLineForeground(Line, Player_x, line, player_bits, PLAYER_WIDTH);
        DISP_WriteLine(playerLine, Line);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Functions - Input
/////////////////////////////////////////////////////////////////////////////

// Handle player input and move the player
void InputPlayer()
{
    uint32_t mv;
    uint8_t dir;

    ADC0CN0_ADBUSY = 1;
    while (!ADC0CN0_ADINT);
    ADC0CN0_ADINT = 0;

    mv = ((uint32_t)ADC0) * 3300 / 1024;

    dir = JOYSTICK_convert_mv_to_direction(mv);

    // Move player left
    if (dir == JOYSTICK_W)
    {
        if (Player_x != 0)
            Player_x -= PLAYER_ADVANCE_X;
    }

    // Move player right
    if (dir == JOYSTICK_E)
    {
        if (Player_x < DISP_WIDTH - PLAYER_WIDTH)
            Player_x += PLAYER_ADVANCE_X;
    }

    // Shoot player projectile
    if (BSP_PB0 == BSP_PB_PRESSED)
    {
        // Can't fire if a projectile is already in motion
        if (!PlayerFire)
        {
            PlayerFire = true;
            Bullet_x = Player_x + (PLAYER_WIDTH - BULLET_WIDTH) / 2;
            Bullet_y = Player_y - BULLET_HEIGHT;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Functions - State
/////////////////////////////////////////////////////////////////////////////

// Fire the player bullet
void StateFireBullet()
{
    // Player projectile in motion
    if (PlayerFire)
    {
        Bullet_y -= BULLET_ADVANCE_Y;

        if (Bullet_y < TOP_LINE_LIMIT)
        {
            PlayerFire = false;
        }
    }
}

// Choose a random enemy to fire a bolt
void StateFireBolt()
{
    uint8_t randShooter;
    uint8_t row;
    uint8_t col;

    // Enemy will fire after the last shot finished
    if (!EnemyFire)
    {
        EnemyFire = true;

        // Choose a random shooter from remaining enemies
        randShooter = (rand() % EnemiesRemaining) + 1;

        // Find the shooter
        for (row = 0; row < ENEMY_ROWS; row++)
        {
            for (col = 0; col < ENEMY_COLS; col++)
            {
                if (EnemyAlive[row][col])
                {
                    randShooter--;

                    // Found the shooter
                    if (randShooter == 0)
                    {
                        Bolt_x = Enemy_x + (col * ENEMY_WIDTH) + ((ENEMY_WIDTH - BOLT_WIDTH) / 2);
                        Bolt_y = Enemy_y + (row+1) * ENEMY_HEIGHT;
                        return;
                    }
                }
            }
        }

        // Could not find a shooter, something went wrong
        DEBUG_ASSERT(false);
    }
    // Enemy projectile in motion
    else
    {
        Bolt_y += BOLT_ADVANCE_Y;

        if (Bolt_y > BOTTOM_LINE_LIMIT + 1)
        {
            EnemyFire = false;
        }
    }
}

void StateAdvanceEnemy()
{
    // Enemies moving right
    if (EnemyDirectionRight)
    {
        // Calculate new position using fixed point
        Enemy_x_fp += EnemyAdvanceRate[GameLevel];

        // Convert to absolute px position
        Enemy_x = (Enemy_x_fp + 50) / 100;

        // Enemy reached right-hand side
        if (Enemy_x >= DISP_WIDTH - TOTAL_ENEMY_WIDTH - 1)
        {
            Enemy_y += ENEMY_ADVANCE_Y;
            EnemyAdvanced = true;
            EnemyDirectionRight = 0;
        }
    }
    // Enemies moving left
    else
    {
        // Calculate new position using fixed point
        Enemy_x_fp -= EnemyAdvanceRate[GameLevel];

        // Convert to absolute px position
        Enemy_x = (Enemy_x_fp + 50) / 100;

        // Enemy reached left-hand side
        if (Enemy_x <= 1 || Enemy_x > DISP_WIDTH - TOTAL_ENEMY_WIDTH)
        {
            Enemy_y += ENEMY_ADVANCE_Y;
            EnemyAdvanced = true;
            EnemyDirectionRight = 1;
        }
    }
}

// Move the enemy and perform hit detection
void StateAdvanceScene()
{
    static uint8_t frameCount = 0;

    // Move the enemy swarm
    StateAdvanceEnemy();

    // Han shot first!
    StateFireBullet();
    HitDetectEnemy();

    // Enemy fires
    StateFireBolt();
    HitDetectPlayer();

    // Alternate sprite animations
    if (frameCount % 10 == 0)
    {
        AnimateEnemy = !AnimateEnemy;
    }
    // Alternate sprite animations
    if (frameCount % 2 == 0)
    {
        AnimateBolt = !AnimateBolt;
    }

    frameCount++;
}

/////////////////////////////////////////////////////////////////////////////
// Functions - Hit Detection
/////////////////////////////////////////////////////////////////////////////

// Check if player bullet hit an enemy
// The bullet can only kill one enemy
// The enemy in the lowest row and farthest to the left
// that overlaps the bullet will die
void HitDetectEnemy()
{
    uint8_t i;
    uint8_t row;
    uint8_t col;

    if (!PlayerFire)
        return;

    for (i = 0; i < ENEMY_ROWS; i++)
    {
        row = ENEMY_ROWS - i - 1;

        // Check if bullet hits an enemy in the current row (starting from the bottom)
        // Check if bullet intersects enemy on current row in the y dimension
        if (OBJ_OVERLAPS_OBJ(
                Bullet_y, BULLET_HEIGHT,
                Enemy_y + (row * ENEMY_HEIGHT), ENEMY_HEIGHT))
        {
            for (col = 0; col < ENEMY_COLS; col++)
            {
                if (EnemyAlive[row][col])
                {
                    // Check if bullet hits an enemy in the current column (starting from the left)
                    // Check if bullet intersects enemy on current column in the x dimension
                    if (OBJ_OVERLAPS_OBJ(
                            Bullet_x + BULLET_HIT_LEFT_MARGIN, BULLET_HIT_WIDTH,
                            Enemy_x + (col * ENEMY_WIDTH) + ENEMY_HIT_LEFT_MARGIN, ENEMY_HIT_WIDTH))
                    {
                        EnemyAlive[row][col] = 0;
                        EnemiesRemaining--;
                        PlayerFire = false;

                        // Clean bullet trail in CleanDirtyLines()
                        if (row == ENEMY_ROWS - 1)
                        {
                            // Set one shot
                            CleanDirtyBulletLines = true;
                        }

                        // Add kill score
                        RenderScore(false, EnemyScoreValue[row]);

#if DEBUG_SHOW_HIT_DETECTION
                        DISP_ClearLine(Bullet_y, DISP_FOREGROUND_COLOR);
                        DISP_ClearLine(Bullet_y + BULLET_HEIGHT, DISP_FOREGROUND_COLOR);
                        Wait(DEBUG_SHOW_HIT_DETECTION_DELAY);
#endif
                        return;
                    }
                }
            }
        }
    }
}

// Check if enemy bolt hit the player
void HitDetectPlayer()
{
    if (!EnemyFire)
        return;

    // Bolt overlaps player in y-axis
    if (OBJ_OVERLAPS_OBJ(
            Bolt_y, BOLT_HEIGHT,
            Player_y, PLAYER_HEIGHT))
    {
        // Bolt overlaps player in x-axis
        if (OBJ_OVERLAPS_OBJ(
                Bolt_x + BOLT_HIT_LEFT_MARGIN, BOLT_HIT_WIDTH,
                Player_x + PLAYER_HIT_LEFT_MARGIN, PLAYER_HIT_WIDTH))
        {
#if DEBUG_SHOW_HIT_DETECTION
            DISP_ClearLine(Bolt_y, COLOR_BLACK);
            DISP_ClearLine(Bolt_y + BOLT_HEIGHT, COLOR_BLACK);
            Wait(DEBUG_SHOW_HIT_DETECTION_DELAY);
#endif

            GameOver = true;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Functions - Game
/////////////////////////////////////////////////////////////////////////////

void GameNextLevel()
{
    // Increase level (capped at lvl 9)
    if (GameLevel < MAX_LEVEL - 1)
        GameLevel++;

    RenderScore(false, 0);

    // Show new level screen
    RenderLevelScreen();

    // Wait a second before starting the next round
    Wait(NEW_LEVEL_WAIT_TIME);

    // Reset state variables for new level
    GameLevelReset();
}

void GameLevelReset()
{
    uint8_t row;
    uint8_t col;

    AnimateEnemy = false;
    AnimateBolt = false;

    for (row = 0; row < ENEMY_ROWS; row++)
    {
        for (col = 0; col < ENEMY_COLS; col++)
        {
            EnemyAlive[row][col] = 1;
        }
    }

    Enemy_x = 0;
    Enemy_x_fp = 0;
    Enemy_y = TOP_LINE_LIMIT;
    EnemyAdvanced = false;
    EnemyDirectionRight = true;
    EnemiesRemaining = ENEMY_ROWS * ENEMY_COLS;

    EnemyFire = false;

    Player_x = (DISP_WIDTH - PLAYER_WIDTH) / 2;
    Player_y = (DISP_HEIGHT - PLAYER_HEIGHT);

    PlayerFire = false;

    CleanDirtyBulletLines = false;

    DISP_ClearAll();

    // Render new level text
    RenderScore(false, 0);
}

void GameReset()
{
    // Seed random number generator using system
    // up time
    srand(GetTickCount());

    GameLevel = 0;
    GameScore = 0;
    GameOver = false;

    GameLevelReset();

    DISP_ClearAll();

    // Reset the score
    RenderScore(true, 0);
}

void GameInit()
{
    GameReset();
}

void GameLoop()
{
    static uint16_t gameOverTick;

    // Game is running
    if (!GameOver)
    {
        InputPlayer();
        StateAdvanceScene();
        RenderScene();

        // Check for player level victory
        if (EnemiesRemaining == 0)
        {
            GameNextLevel();
        }
        else
        {
            // Check for end game condition
            // Enemies reached the last line
            if (Enemy_y + TOTAL_ENEMY_HEIGHT > BOTTOM_LINE_LIMIT)
            {
                GameOver = true;
            }
        }

        if (GameOver)
        {
            RenderGameOverScreen();

            // Wait a second in case the user presses a few too many buttons
            Wait(GAME_OVER_WAIT_TIME);

            // Mark game over time for automatic reset
            gameOverTick = GetTickCount();
        }
    }
    // Game is over
    else
    {
        // Wait until the user presses a button to start a new game
        if (BSP_PB0 == BSP_PB_PRESSED || BSP_PB1 == BSP_PB_PRESSED)
        {
            GameReset();
        }
        // Automatically reset after 1 minute
        else if (GetTickCount() - gameOverTick > GAME_OVER_RESET_TIMEOUT)
        {
            GameReset();
        }
    }
}

// Delay until start of next frame
void GameSynchFrame()
{
    static uint16_t lastTick = 0;
    uint16_t tick;

    // Render at 50 Hz
    while (((tick = GetTickCount()) - lastTick) < HZ_TO_MS(GAME_FRAME_RATE));
    lastTick = tick;
}

/////////////////////////////////////////////////////////////////////////////
// Functions - Main
/////////////////////////////////////////////////////////////////////////////

void SpaceInvaders_Main()
{
    GameInit();

    while (1)
    {
        GameLoop();
        GameSynchFrame();
    }
}

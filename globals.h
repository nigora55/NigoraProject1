#ifndef GLOBALS_H
#define GLOBALS_H

// Common utility constants
constexpr float EPSILON = 0.0001f;

// Commonly used characters (used in level parsing, shared across logic)
inline constexpr char WALL      = '#';
inline constexpr char WALL_DARK = '=';
inline constexpr char AIR       = '-';
inline constexpr char SPIKE     = '^';
inline constexpr char PLAYER    = '@';
inline constexpr char ENEMY     = '&';
inline constexpr char COIN      = '*';
inline constexpr char EXIT      = 'E';

#pragma once
extern int game_frame;

#endif // GLOBALS_H

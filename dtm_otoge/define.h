#pragma once
#include "DxLib.h"

#define WINDOW_W 854
#define WINDOW_H 480

#define NOTES_NUM 1919

#define LEFT 0
#define RIGHT 1

#define CURSOR_Y 400
#define CURSOR_X_LEFT  WINDOW_W / 4
#define CURSOR_X_RIGHT WINDOW_W * 3 / 4

#define PROGRESS_FRAME DrawString( WINDOW_W - 250 ,  WINDOW_H - 50, "Now Loading..." , GetColor(255, 255, 255) ); DrawBox(WINDOW_W - 110, WINDOW_H - 50, WINDOW_W - 10, WINDOW_H - 30, GetColor(255, 255, 255), FALSE);
#define PROGRESS(a,b) DrawBox(WINDOW_W - 110, WINDOW_H - 50, WINDOW_W - 10 - (a / b) * 100, WINDOW_H - 30, GetColor(255, 255, 255), TRUE);

#define VOL_BGM 255
#define VOL_SE 127

extern bool windowMode, exitApplication;
extern int DEF_LEVEL, DEF_SONG, DEF_SPEED;
#ifndef BLOCK_H
#define BLOCK_H

#include <time.h>
#include <SDL.h>
#include <global.h>
#include <texture.h>
#include <sound.h>

class Block
{
private:
    Texture cell[COLOR_NUM + 1];
    int curType;
    int curColor;
    int nextType;
    int nextColor;
    int trans;
    SDL_Point position;
    SDL_Rect staticBox;
    SDL_Rect dynamicBox;
    int speedX;
    int speedY;
    int score;
    int maxScore;
    bool alive;
    bool start;
    bool pause;
    bool gameover;
    bool money;
    void updateStaticBox();
    void moveDynamicBox();
    void eraseLine();
    bool checkCollision();
    void restart();
public:
    Block(Window window);
    void init();
    void draw(Window window);
    void drawNextBlock(Window window);
    void drawPile(Window window);
    void move();
    void autoDrop();
    void fall();
    void rotate();
    void copyToPile();
    bool isAlive();
    bool isStart();
    bool isPaused();
    bool isGameover();
    bool isMoney();
    int getScore();
    int getMaxScore();
    void handleEvent(SDL_Event event);
};

extern int pile[BACKGROUND_CELL_HEIGHT][BACKGROUND_CELL_WIDTH];
extern const SDL_Rect wall;
bool hitWall(SDL_Rect block);
bool hitPile(SDL_Point cellPosition, const int block[][4], int pile[][10]);
void eraseLine();
int getPileHeight();

#endif // !BLOCK_H

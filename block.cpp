#include <block.h>

void Block::updateStaticBox()
{
    int minX = BLOCK_PIXEL_WIDTH;
    int minY = BLOCK_PIXEL_HEIGHT;
    int maxX = 0;
    int maxY = 0;
    for (int line = 0; line < BLOCK_CELL_HEIGHT; line++)
    {
        for (int column = 0; column < BLOCK_CELL_WIDTH; column++)
        {
            if (block[curType][trans][line][column])
            {
                minX = std::min(minX, column * CELL_WIDTH);
                minY = std::min(minY, line * CELL_HEIGHT);
                maxX = std::max(maxX, (column + 1) * CELL_WIDTH);
                maxY = std::max(maxY, (line + 1) * CELL_HEIGHT);
            }
        }
    }
    staticBox = { minX,minY,maxX - minX,maxY - minY };
}

void Block::moveDynamicBox()
{
    dynamicBox = { position.x + staticBox.x,position.y + staticBox.y,staticBox.w,staticBox.h };
}

Block::Block(Window window)
{
    srand((unsigned)time(NULL));
    cell[RED].createFromImage(window, "rsc\\red.png");
    cell[GREEN].createFromImage(window, "rsc\\green.png");
    cell[BLUE].createFromImage(window, "rsc\\blue.png");
    cell[YELLOW].createFromImage(window, "rsc\\yellow.png");
    cell[BROWN].createFromImage(window, "rsc\\brown.png");
    cell[PURPLE].createFromImage(window, "rsc\\purple.png");
    cell[GREY].createFromImage(window, "rsc\\grey.png");
    curType = 0;
    curColor = 0;
    nextType = rand() % TYPE_NUM;
    nextColor = nextType % COLOR_NUM + 1;
    trans = 0;
    position = { BACKGROUND_START_X,BACKGROUND_START_Y };
    staticBox = { 0,0,BLOCK_CELL_WIDTH,BLOCK_CELL_HEIGHT };
    speedX = 0;
    speedY = 0;
    score = 0;
    maxScore = 0;
    alive = false;
    start = false;
    pause = false;
    gameover = false;
    money = false;
}

void Block::init()
{
    if (gameover || pause)
        return;
    curType = nextType;
    curColor = nextColor;
    nextType = rand() % TYPE_NUM;
    nextColor = nextType % COLOR_NUM + 1;
    trans = 0;
    position = { BLOCK_START_X,BLOCK_START_Y };
    updateStaticBox();
    moveDynamicBox();
    alive = true;
}

void Block::draw(Window window)
{
    if (!alive || gameover || !start)
        return;
    int x = position.x;
    int y = position.y;
    for (int line = 0; line < BLOCK_CELL_HEIGHT; line++)
    {
        x = position.x;
        for (int column = 0; column < BLOCK_CELL_WIDTH; column++)
        {
            if (block[curType][trans][line][column])
                cell[curColor].render(window, x, y);
            x += CELL_WIDTH;
        }
        y += CELL_HEIGHT;
    }
}

void Block::drawNextBlock(Window window)
{
    int x = NEXT_POSITION_X;
    int y = NEXT_POSITION_Y;
    for (int line = 0; line < BLOCK_CELL_HEIGHT; line++)
    {
        x = NEXT_POSITION_X;
        for (int column = 0; column < BLOCK_CELL_WIDTH; column++)
        {
            if (block[nextType][0][line][column])
                cell[nextColor].render(window, x, y);
            x += CELL_WIDTH;
        }
        y += CELL_HEIGHT;
    }
}

void Block::drawPile(Window window)
{
    for (int line = 0; line < BACKGROUND_CELL_HEIGHT; line++)
    {
        for (int column = 0; column < BACKGROUND_CELL_WIDTH; column++)
        {
            if (pile[line][column])
                cell[pile[line][column]].render(window, column * CELL_WIDTH + BACKGROUND_START_X, line * CELL_HEIGHT + BACKGROUND_START_Y);
        }
    }
}

void Block::move()
{
    if (!alive || gameover || !start || pause)
        return;
    //Move in X direction.
    position.x += speedX;
    moveDynamicBox();
    if (checkCollision())
    {
        position.x -= speedX;
        moveDynamicBox();
    }
    //Move in Y direction.
    position.y += speedY;
    moveDynamicBox();
    if (checkCollision())
    {
        position.y -= speedY;
        moveDynamicBox();
        alive = false;
    }
}

void Block::autoDrop()
{
    if (!alive || gameover || !start || pause)
        return;
    //Move in Y direction.
    position.y += CELL_HEIGHT;
    moveDynamicBox();
    if (checkCollision())
    {
        position.y -= CELL_HEIGHT;
        moveDynamicBox();
        alive = false;
    }
}

void Block::fall()
{
    if (!alive || gameover || !start || pause)
        return;
    while (!checkCollision())
    {
        position.y += CELL_HEIGHT;
        moveDynamicBox();
    }
    position.y -= CELL_HEIGHT;
    moveDynamicBox();
    alive = false;
}

void Block::rotate()
{
    if (!alive || gameover || !start || pause)
        return;
    trans = (++trans) % TRANS_NUM;
    updateStaticBox();
    moveDynamicBox();
    if (checkCollision())
    {
        trans = ((--trans) % TRANS_NUM + TRANS_NUM) % TRANS_NUM;
        updateStaticBox();
        moveDynamicBox();
    }
}

void Block::copyToPile()
{
    if (alive || gameover || !start || pause)
        return;
    int cellX = (position.x - BACKGROUND_START_X) / CELL_WIDTH;
    int cellY = (position.y - BACKGROUND_START_Y) / CELL_HEIGHT;
    for (int line = 0; line < BLOCK_CELL_HEIGHT; line++)
    {
        for (int column = 0; column < BLOCK_CELL_WIDTH; column++)
        {
            if (block[curType][trans][line][column])
                pile[cellY + line][cellX + column] = curColor;
        }
    }
    eraseLine();
    if (getPileHeight() > BACKGROUND_CELL_HEIGHT - 2)
        gameover = true;
}

bool Block::checkCollision()
{
    return (hitWall(dynamicBox) || hitPile(position, block[curType][trans], pile));
}

bool Block::isAlive()
{
    return alive;
}

bool Block::isStart()
{
    return start;
}

bool Block::isPaused()
{
    return pause;
}

bool Block::isGameover()
{
    return gameover;
}

bool Block::isMoney()
{
    return money;
}

int Block::getScore()
{
    return score;
}

int Block::getMaxScore()
{
    return maxScore;
}

void Block::handleEvent(SDL_Event event)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
        switch (event.key.keysym.sym)
        {
            case SDLK_w:
                rotate();
                playChunk("rsc\\medium.wav");
                break;
            case SDLK_s:
                speedY += CELL_HEIGHT;
                break;
            case SDLK_a:
                speedX -= CELL_WIDTH;
                break;
            case SDLK_d:
                speedX += CELL_WIDTH;
                break;
            case SDLK_SPACE:
                fall();
                playChunk("rsc\\high.wav");
                break;
            case SDLK_RETURN:
                start = true;
                break;
            case SDLK_r:
                restart();
                playChunk("rsc\\start.wav");
                break;
            case SDLK_p:
                if (start || !gameover)
                    pause = !pause;
                break;
            case SDLK_m:
                pause = !pause;
                money = !money;
                break;
        }
    else if(event.type==SDL_KEYUP&&event.key.repeat==0)
        switch (event.key.keysym.sym)
        {
            case SDLK_s:
                speedY -= CELL_HEIGHT;
                break;
            case SDLK_a:
                speedX += CELL_WIDTH;
                break;
            case SDLK_d:
                speedX -= CELL_WIDTH;
                break;
        }
}

void Block::restart()
{
    if (!gameover)
        return;
    memset(pile, 0, sizeof(pile));
    gameover = false;
    score = 0;
    init();
}

int pile[BACKGROUND_CELL_HEIGHT][BACKGROUND_CELL_WIDTH] = { 0 };

const SDL_Rect wall = { BACKGROUND_START_X,BACKGROUND_START_Y,BACKGROUND_PIXEL_WIDTH,BACKGROUND_PIXEL_HEIGHT };

bool hitWall(SDL_Rect block)
{
    if (block.x >= wall.x && block.x + block.w <= wall.x + wall.w && block.y + block.h <= wall.y + wall.h)
        return 0;
    else
        return 1;
}

bool hitPile(SDL_Point position, const int block[][BLOCK_CELL_WIDTH], int pile[][BACKGROUND_CELL_WIDTH])
{
    int cellX = (position.x - BACKGROUND_START_X) / CELL_WIDTH;
    int cellY = (position.y - BACKGROUND_START_Y) / CELL_HEIGHT;
    int temp = 0;
    for (int line = 0; line < BLOCK_CELL_HEIGHT; line++)
    {
        for (int column = 0; column < BLOCK_CELL_WIDTH; column++)
        {
            temp = pile[cellY + line][cellX + column];
            if (temp && block[line][column])
                return 1;
        }
    }
    return 0;
}

void Block::eraseLine()
{
    //Erase part.
    for (int line = 0; line < BACKGROUND_CELL_HEIGHT; line++)
    {
        int count = 0;
        for (int column = 0; column < BACKGROUND_CELL_WIDTH; column++)
        {
            //Once vacancy appears, jump to the next line.
            if (pile[line][column] == 0)
                break;
            //Or go on checking.
            else
                count++;
        }
        //Once a full line appears, return that line to zero.
        if (count == BACKGROUND_CELL_WIDTH)
        {
            for (int i = 0; i < BACKGROUND_CELL_WIDTH; i++)
            {
                pile[line][i] = 0;
            }
            score++;
            if (maxScore < score)
                maxScore = score;
            playChunk("rsc\\erase.wav");
        }
    }
    //Rewrite part.
    int temp[BACKGROUND_CELL_HEIGHT][BACKGROUND_CELL_WIDTH];
    memset(temp, 0, sizeof(temp));
    int templine = BACKGROUND_CELL_HEIGHT - 1;
    for (int line = BACKGROUND_CELL_HEIGHT - 1; line >= 0; line--)
    {
        bool empty = 1;
        for (int column = 0; column < BACKGROUND_CELL_WIDTH; column++)
        {
            if (pile[line][column])
            {
                temp[templine][column] = pile[line][column];
                empty = 0;
            }
        }
        if (!empty)
            templine--;
    }
    memcpy(pile, temp, sizeof(pile));
}

int getPileHeight()
{
    for (int line = 0; line < BACKGROUND_CELL_HEIGHT; line++)
    {
        for (int column = 0; column < BACKGROUND_CELL_WIDTH; column++)
        {
            if (pile[line][column])
                return BACKGROUND_CELL_HEIGHT - line;
        }
    }
    return 0;
}
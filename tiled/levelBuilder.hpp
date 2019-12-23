//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#ifndef LEVELBUILDER_HPP
#define LEVELBUILDER_HPP

#include <tamtypes.h>
#include "utils.h"

// forward declarations
class Coin;
class Mushroom;
class Flower;
class character;
typedef struct gsTexture GSTEXTURE;

class Level
{
public:
    u8 coinCount;
    u8 mushroomCount;
    u8 flowerCount;
    u8 goombaCount;
    u8 koopaCount;

    character* goomba;
    character* koopa;
    Coin* coin;
    Flower* flower;
    Mushroom* mushroom;

    u8 width;
    u8 height;
    u8 absoluteHeight;
    u8 tile_width;
    u8 tile_height;
    GSTEXTURE* spritesheet;
    u8* data;
    u8 solid[64];

    // respawn locations when entering a pipe
    point playerRespawnEntryLocation;
    point viewportRespawnEntryLocation;
    u8 cameraLockEntry;

    // respawn locations for exiting a pipe
    point playerRespawnExitLocation;
    point viewportRespawnExitLocation;
    u8 cameraLockExit;
    
    Level();
    ~Level();
    u8 get_box(int x, int y);
    u16 get_index(int x, int y);
    Coin* getCoin(int x, int y);
    Mushroom* getMushroom(int x, int y);
    Flower* getFlower(int x, int y);
};

class LevelBuilderBase
{
public:
    Level* level;
    GSGLOBAL* gsGlobal;
    GSTEXTURE* tilesheet;
    GSTEXTURE* pickupTexture;
    GSTEXTURE* koopaSprites;
    GSTEXTURE* goombaSprites;

    LevelBuilderBase(GSGLOBAL* gsGlobal, GSTEXTURE* tilesheet, GSTEXTURE* pickupTexture, GSTEXTURE* koopaSprites, GSTEXTURE* goombaSprites);
    virtual ~LevelBuilderBase();
    void loadCoins(GSTEXTURE* tex);
    void loadMushrooms(GSTEXTURE* tex);
    void loadFlowers(GSTEXTURE* tex);
    void loadGoombas(GSTEXTURE* tex);
    void loadKoopas(GSTEXTURE* tex);
    virtual Level* build(GSGLOBAL* gsGlobal);
};

class LevelBuilder_1_1 : public LevelBuilderBase
{
public:
    LevelBuilder_1_1(GSGLOBAL* gsGlobal, GSTEXTURE* tilesheet, GSTEXTURE* pickupTexture, GSTEXTURE* koopaSprites, GSTEXTURE* goombaSprites);
    ~LevelBuilder_1_1();
    void loadCoins(GSTEXTURE* tex);
    void loadMushrooms(GSTEXTURE* tex);
    void loadFlowers(GSTEXTURE* tex);
    void loadGoombas(GSTEXTURE* tex);
    void loadKoopas(GSTEXTURE* tex);
    Level* build(GSGLOBAL* gsGlobal);
};

class LevelBuilder_1_2 : public LevelBuilderBase
{
public:
    LevelBuilder_1_2(GSGLOBAL* gsGlobal, GSTEXTURE* tilesheet, GSTEXTURE* pickupTexture, GSTEXTURE* koopaSprites, GSTEXTURE* goombaSprites);
    ~LevelBuilder_1_2();
    void loadCoins(GSTEXTURE* tex);
    void loadMushrooms(GSTEXTURE* tex);
    void loadFlowers(GSTEXTURE* tex);
    void loadGoombas(GSTEXTURE* tex);
    void loadKoopas(GSTEXTURE* tex);
    Level* build(GSGLOBAL* gsGlobal);
};

class LevelBuilder
{
public:
    LevelBuilderBase* levelBuilder;
    GSGLOBAL* gsGlobal;
    GSTEXTURE* tilesheet;
    GSTEXTURE* pickupTexture;
    GSTEXTURE* koopaSprites;
    GSTEXTURE* goombaSprites;
    
    LevelBuilder(GSGLOBAL* gsGlobal, GSTEXTURE* tilesheet, GSTEXTURE* pickupTexture, GSTEXTURE* koopaSprites, GSTEXTURE* goombaSprites);
    ~LevelBuilder();
    Level* build(u8 world, u8 level);
};
#endif

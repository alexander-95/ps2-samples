//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#ifndef LEVELBUILDER_HPP
#define LEVELBUILDER_HPP

#include <tamtypes.h>

// forward declarations
class Coin;
class Mushroom;
class Flower;
class character;
typedef struct gsTexture GSTEXTURE;

class LevelBuilderBase
{
public:
    u8 coinCount;
    u8 mushroomCount;
    u8 flowerCount;
    u8 goombaCount;
    u8 koopaCount;

    u8 width;
    u8 height;
    u8 absoluteHeight;
    u8 tile_width;
    u8 tile_height;
    GSTEXTURE* spritesheet;
    u8* data;
    u8* solid;
};

class LevelBuilder_1_1 : public LevelBuilderBase
{
public:
    LevelBuilder_1_1();
    ~LevelBuilder_1_1();
    Coin* loadCoins(GSTEXTURE* tex);
    Mushroom* loadMushrooms(GSTEXTURE* tex);
    Flower* loadFlowers(GSTEXTURE* tex);
    character* loadGoombas(GSTEXTURE* tex);
    character* loadKoopas(GSTEXTURE* tex);
    u8 get_box(int x, int y);
    u16 get_index(int x, int y);
    Coin* getCoin(Coin* coin, int x, int y);
    Mushroom* getMushroom(Mushroom* mushroom, int x, int y);
    Flower* getFlower(Flower* flower, int x, int y);
};

class LevelBuilder_1_2
{
public:
    LevelBuilder_1_2();
    ~LevelBuilder_1_2();
};

class LevelBuilder
{
public:
    LevelBuilder();
    ~LevelBuilder();
};
#endif

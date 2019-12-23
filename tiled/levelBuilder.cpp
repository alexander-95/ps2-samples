//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>
#include "graphics.hpp"
#include "levelBuilder.hpp"
#include "pickup.hpp"
#include "character.hpp"
#include "map_data.h"

LevelBuilderBase::LevelBuilderBase(GSGLOBAL* gsGlobal, GSTEXTURE* tilesheet, GSTEXTURE* pickupTexture, GSTEXTURE* koopaSprites, GSTEXTURE* goombaSprites)
{
    this->gsGlobal = gsGlobal;
    this->tilesheet = tilesheet;
    this->pickupTexture = pickupTexture;
    this->koopaSprites = koopaSprites;
    this->goombaSprites = goombaSprites;
}

LevelBuilderBase::~LevelBuilderBase()
{

}

Level* LevelBuilderBase::build(GSGLOBAL* gsGlobal)
{
    return NULL;
}

Level::Level()
{
    coinCount = 0;
    mushroomCount = 0;
    flowerCount = 0;
    goombaCount = 0;
    koopaCount = 0;

    width = 0;
    height = 0;
    
}
Level::~Level()
{}

u8 Level::get_box(int x, int y)
{
    int index = get_index(x, y);
    return data[index];
}
u16 Level::get_index(int x, int y)
{
    int index_x = x / tile_width;
    int index_y = y / tile_height;
    return (index_y*width)+index_x;
}
Coin* Level::getCoin(int x, int y)
{
    for(int i = 0; i < coinCount; i++)
    {
        if(coin[i].x == x && coin[i].y == y)
        {
            printf("returning coin %d\n", i);
            return &coin[i];
        }
    }
    return NULL;
}
Mushroom* Level::getMushroom(int x, int y)
{
    for(int i = 0; i < mushroomCount; i++)
    {
        if(mushroom[i].x == x && mushroom[i].y == y)
        {
            printf("returning mushroom %d\n", i);
            return &mushroom[i];
        }
    }
    return NULL;
}
Flower* Level::getFlower(int x, int y)
{
    for(int i = 0; i < flowerCount; i++)
    {
        if(flower[i].x == x && flower[i].y == y)
        {
            printf("returning flower %d\n", i);
            return &flower[i];
        }
    }
    return NULL;
}

LevelBuilder_1_1::LevelBuilder_1_1(GSGLOBAL* gsGlobal, GSTEXTURE* tilesheet, GSTEXTURE* pickupTexture, GSTEXTURE* koopaSprites, GSTEXTURE* goombaSprites) :
    LevelBuilderBase(gsGlobal, tilesheet, pickupTexture, koopaSprites, goombaSprites)
{
    
}
LevelBuilder_1_1::~LevelBuilder_1_1()
{

}

Level* LevelBuilder_1_1::build(GSGLOBAL* gsGlobal)
{
    level = new Level();
    level->coinCount = 32;
    level->mushroomCount = 4;
    level->flowerCount = 2;
    level->koopaCount = 1;
    level->goombaCount = 16;

    level->width = 224;
    level->height = 15;
    level->absoluteHeight = 30;
    level->tile_width = 16;
    level->tile_height = 16;

    u8 solid[64] = {0,1,0,0,0,0,0,0,
                    0,0,0,1,1,1,0,0,
                    1,1,0,0,0,0,0,0,
                    1,1,0,0,0,0,0,0,
                    1,1,1,1,0,0,0,0,
                    1,1,1,0,0,0,0,0,
                    1,1,1,0,0,0,0,0,
                    0,0,0,0,0,0,0,0};
    for(int i = 0; i < 64;i++)level->solid[i] = solid[i];
    
    loadCoins(pickupTexture);
    loadMushrooms(pickupTexture);
    loadFlowers(pickupTexture);
    loadGoombas(goombaSprites);
    loadKoopas(koopaSprites);
    level->spritesheet = tilesheet;
    level->data = map_data_1_1;

    level->playerRespawnEntryLocation.x = 2384;
    level->playerRespawnEntryLocation.y = 240;
    level->viewportRespawnEntryLocation.x = 2368;
    level->viewportRespawnEntryLocation.y = 240;
    level->cameraLockEntry = ENABLED;
    level->cameraLockExit = DISABLED;

    return level;
}

void LevelBuilder_1_1::loadCoins(GSTEXTURE* tex)
{
    level->coin = new Coin[level->coinCount];

    for(int i = 0; i < 32; i++)
    {
        level->coin[i].width = 8;
        level->coin[i].height = 16;
        level->coin[i].spritesheet = tex;
    }
    level->coin[0].x = 260; level->coin[0].y = 144;
    level->coin[1].x = 372; level->coin[1].y = 144;
    level->coin[2].x = 356; level->coin[2].y = 80;
    level->coin[3].x = 1508; level->coin[3].y = 80;
    level->coin[4].x = 1700; level->coin[4].y = 144;
    level->coin[5].x = 1748; level->coin[5].y = 144;
    level->coin[6].x = 1796; level->coin[6].y = 144;
    level->coin[7].x = 2068; level->coin[7].y = 80;
    level->coin[8].x = 2084; level->coin[8].y = 80;
    level->coin[9].x = 2724; level->coin[9].y = 144;

    // underground coins
    level->coin[10].x = 2436; level->coin[10].y = 384; level->coin[10].activated = 1;
    level->coin[11].x = 2452; level->coin[11].y = 384; level->coin[11].activated = 1;
    level->coin[12].x = 2468; level->coin[12].y = 384; level->coin[12].activated = 1;
    level->coin[13].x = 2484; level->coin[13].y = 384; level->coin[13].activated = 1;
    level->coin[14].x = 2500; level->coin[14].y = 384; level->coin[14].activated = 1;
    level->coin[15].x = 2516; level->coin[15].y = 384; level->coin[15].activated = 1;
    level->coin[16].x = 2532; level->coin[16].y = 384; level->coin[16].activated = 1;
    level->coin[17].x = 2436; level->coin[17].y = 352; level->coin[17].activated = 1;
    level->coin[18].x = 2452; level->coin[18].y = 352; level->coin[18].activated = 1;
    level->coin[19].x = 2468; level->coin[19].y = 352; level->coin[19].activated = 1;
    level->coin[20].x = 2484; level->coin[20].y = 352; level->coin[20].activated = 1;
    level->coin[21].x = 2500; level->coin[21].y = 352; level->coin[21].activated = 1;
    level->coin[22].x = 2516; level->coin[22].y = 352; level->coin[22].activated = 1;
    level->coin[23].x = 2532; level->coin[23].y = 352; level->coin[23].activated = 1;

    level->coin[24].x = 2452; level->coin[24].y = 320; level->coin[24].activated = 1;
    level->coin[25].x = 2468; level->coin[25].y = 320; level->coin[25].activated = 1;
    level->coin[26].x = 2484; level->coin[26].y = 320; level->coin[26].activated = 1;
    level->coin[27].x = 2500; level->coin[27].y = 320; level->coin[27].activated = 1;
    level->coin[28].x = 2516; level->coin[28].y = 320; level->coin[28].activated = 1;
}

void LevelBuilder_1_1::loadMushrooms(GSTEXTURE* tex)
{
    level->mushroom = new Mushroom[level->mushroomCount];
    for(int i = 0; i < level->mushroomCount; i++)
    {
        level->mushroom[i].width = 16;
        level->mushroom[i].height = 16;
        level->mushroom[i].type = 1;
        level->mushroom[i].activated = 0;
        level->mushroom[i].spritesheet = tex;
    }
    level->mushroom[0].x = 336; level->mushroom[0].y = 144;
    level->mushroom[1].x = 1024; level->mushroom[1].y = 128; level->mushroom[1].sprite = 1;
}

void LevelBuilder_1_1::loadFlowers(GSTEXTURE* tex)
{
    level->flower = new Flower[level->flowerCount];
    for(int i = 0; i < level->flowerCount; i++)
    {
        level->flower[i].width = 16;
        level->flower[i].height = 16;
        level->flower[i].type = 2;
        level->flower[i].spritesheet = tex;
    }
    level->flower[0].x = 1248; level->flower[0].y = 144;
    level->flower[1].x = 1744; level->flower[1].y = 80;
}

void LevelBuilder_1_1::loadGoombas(GSTEXTURE* tex)
{
    level->goomba = new character[level->goombaCount];

    for(int i = 0; i < level->goombaCount; i++)
    {
        level->goomba[i].spritesheet = tex;
        level->goomba[i].direction = 1;
    }

    level->goomba[0].worldCoordinates.x = 352; level->goomba[0].worldCoordinates.y = 192;level->goomba[0].direction = 0;
    level->goomba[1].worldCoordinates.x = 640; level->goomba[1].worldCoordinates.y = 192;
    level->goomba[2].worldCoordinates.x = 816; level->goomba[2].worldCoordinates.y = 192;
    level->goomba[3].worldCoordinates.x = 848; level->goomba[3].worldCoordinates.y = 192;
    level->goomba[4].worldCoordinates.x = 1280; level->goomba[4].worldCoordinates.y = 64;
    level->goomba[5].worldCoordinates.x = 1312; level->goomba[5].worldCoordinates.y = 64;
    level->goomba[6].worldCoordinates.x = 1552; level->goomba[6].worldCoordinates.y = 192;
    level->goomba[7].worldCoordinates.x = 1576; level->goomba[7].worldCoordinates.y = 192;
    level->goomba[8].worldCoordinates.x = 1824; level->goomba[8].worldCoordinates.y = 192;
    level->goomba[9].worldCoordinates.x = 1848; level->goomba[9].worldCoordinates.y = 192;
    level->goomba[10].worldCoordinates.x = 1984; level->goomba[10].worldCoordinates.y = 192;
    level->goomba[11].worldCoordinates.x = 2008; level->goomba[11].worldCoordinates.y = 192;
    level->goomba[12].worldCoordinates.x = 2048; level->goomba[12].worldCoordinates.y = 192;
    level->goomba[13].worldCoordinates.x = 2072; level->goomba[13].worldCoordinates.y = 192;
    level->goomba[14].worldCoordinates.x = 2784; level->goomba[14].worldCoordinates.y = 192;
    level->goomba[15].worldCoordinates.x = 2808; level->goomba[15].worldCoordinates.y = 192;
}

void LevelBuilder_1_1::loadKoopas(GSTEXTURE* tex)
{
    level->koopa = new character[level->koopaCount];
    level->koopa[0].spritesheet = tex;

    level->koopa[0].height = 24;
    level->koopa[0].worldCoordinates.x = 1712;
    level->koopa[0].worldCoordinates.y = 184;
}


LevelBuilder_1_2::LevelBuilder_1_2(GSGLOBAL* gsGlobal, GSTEXTURE* tilesheet, GSTEXTURE* pickupTexture, GSTEXTURE* koopaSprites, GSTEXTURE* goombaSprites) :
    LevelBuilderBase(gsGlobal, tilesheet, pickupTexture, koopaSprites, goombaSprites)
{

}
LevelBuilder_1_2::~LevelBuilder_1_2()
{

}

Level* LevelBuilder_1_2::build(GSGLOBAL* gsGlobal)
{
    level = new Level();
    level->coinCount = 0;
    level->mushroomCount = 0;
    level->flowerCount = 0;
    level->koopaCount = 0;
    level->goombaCount = 0;

    level->width = 192;
    level->height = 15;
    level->absoluteHeight = 45;
    level->tile_width = 16;
    level->tile_height = 16;

    u8 solid[64] = {0,1,0,0,0,0,0,0,
                    0,0,0,1,1,1,0,0,
                    1,1,0,0,0,0,0,0,
                    1,1,0,0,0,0,0,0,
                    1,1,1,1,0,0,0,0,
                    1,1,1,0,0,0,0,0,
                    1,1,1,0,0,0,0,0,
                    0,0,0,0,0,0,0,0};
    for(int i = 0; i < 64;i++)level->solid[i] = solid[i];
    
    //loadCoins(pickupTexture);
    //loadMushrooms(pickupTexture);
    //loadFlowers(pickupTexture);
    //loadGoombas(goombaSprites);
    //loadKoopas(koopaSprites);
    level->spritesheet = tilesheet;
    level->data = map_data_1_2;

    level->playerRespawnEntryLocation.x = 16;
    level->playerRespawnEntryLocation.y = 272;
    level->viewportRespawnEntryLocation.x = 0;
    level->viewportRespawnEntryLocation.y = 240;
    level->cameraLockEntry = DISABLED;
    level->cameraLockExit = ENABLED;    
    return level;
}

LevelBuilder::LevelBuilder(GSGLOBAL* gsGlobal, GSTEXTURE* tilesheet, GSTEXTURE* pickupTexture, GSTEXTURE* koopaSprites, GSTEXTURE* goombaSprites)
{
    this->gsGlobal = gsGlobal;
    this->tilesheet = tilesheet;
    this->pickupTexture = pickupTexture;
    this->koopaSprites = koopaSprites;
    this->goombaSprites = goombaSprites;    
}
LevelBuilder::~LevelBuilder()
{
    
}
Level* LevelBuilder::build(u8 world, u8 level)
{
    LevelBuilderBase* levelBuilder;
    if(world == 1)
    {
        if(level == 1) levelBuilder =  new LevelBuilder_1_1(gsGlobal, tilesheet, pickupTexture, koopaSprites, goombaSprites);
        else if(level == 2) levelBuilder =  new LevelBuilder_1_2(gsGlobal, tilesheet, pickupTexture, koopaSprites, goombaSprites);
        else
        {
            printf("Failed to create level %d-%d", world, level);
            return NULL;
        }
    }
    else
    {
        printf("Failed to create level %d-%d", world, level);
        return NULL;
    }
    return levelBuilder->build(gsGlobal);
}

//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include "levelBuilder.hpp"
#include "pickup.hpp"
#include "character.hpp"

u8 LevelBuilder_1_1::get_box(int x, int y)
{
    int index = get_index(x, y);
    return data[index];
}
u16 LevelBuilder_1_1::get_index(int x, int y)
{
    int index_x = x / tile_width;
    int index_y = y / tile_height;
    return (index_y*width)+index_x;
}

LevelBuilder_1_1::LevelBuilder_1_1()
{
    coinCount = 32;
    mushroomCount = 4;

    width = 224;
    height = 15;
    absoluteHeight = 30;
    tile_width = 16;
    tile_height = 16;
}
LevelBuilder_1_1::~LevelBuilder_1_1()
{

}

Coin* LevelBuilder_1_1::loadCoins(GSTEXTURE* tex)
{
    Coin* coin = new Coin[coinCount];
    
    for(int i = 0; i < 32; i++)
    {
        coin[i].width = 8;
        coin[i].height = 16;
        coin[i].spritesheet = tex;
    }
    coin[0].x = 260; coin[0].y = 144;
    coin[1].x = 372; coin[1].y = 144;
    coin[2].x = 356; coin[2].y = 80;
    coin[3].x = 1508; coin[3].y = 80;
    coin[4].x = 1700; coin[4].y = 144;
    coin[5].x = 1748; coin[5].y = 144;
    coin[6].x = 1796; coin[6].y = 144;
    coin[7].x = 2068; coin[7].y = 80;
    coin[8].x = 2084; coin[8].y = 80;
    coin[9].x = 2724; coin[9].y = 144;

    // underground coins
    coin[10].x = 2436; coin[10].y = 384; coin[10].activated = 1;
    coin[11].x = 2452; coin[11].y = 384; coin[11].activated = 1;
    coin[12].x = 2468; coin[12].y = 384; coin[12].activated = 1;
    coin[13].x = 2484; coin[13].y = 384; coin[13].activated = 1;
    coin[14].x = 2500; coin[14].y = 384; coin[14].activated = 1;
    coin[15].x = 2516; coin[15].y = 384; coin[15].activated = 1;
    coin[16].x = 2532; coin[16].y = 384; coin[16].activated = 1;
    coin[17].x = 2436; coin[17].y = 352; coin[17].activated = 1;
    coin[18].x = 2452; coin[18].y = 352; coin[18].activated = 1;
    coin[19].x = 2468; coin[19].y = 352; coin[19].activated = 1;
    coin[20].x = 2484; coin[20].y = 352; coin[20].activated = 1;
    coin[21].x = 2500; coin[21].y = 352; coin[21].activated = 1;
    coin[22].x = 2516; coin[22].y = 352; coin[22].activated = 1;
    coin[23].x = 2532; coin[23].y = 352; coin[23].activated = 1;
    
    coin[24].x = 2452; coin[24].y = 320; coin[24].activated = 1;
    coin[25].x = 2468; coin[25].y = 320; coin[25].activated = 1;
    coin[26].x = 2484; coin[26].y = 320; coin[26].activated = 1;
    coin[27].x = 2500; coin[27].y = 320; coin[27].activated = 1;
    coin[28].x = 2516; coin[28].y = 320; coin[28].activated = 1;

    return coin;
}

Mushroom* LevelBuilder_1_1::loadMushrooms(GSTEXTURE* tex)
{
    mushroomCount = 4;
    Mushroom* mushroom = new Mushroom[mushroomCount];
    for(int i = 0; i < 4; i++)
    {
        mushroom[i].width = 16;
        mushroom[i].height = 16;
        mushroom[i].type = 1;
        mushroom[i].activated = 0;
        mushroom[i].spritesheet = tex;
    }
    mushroom[0].x = 336; mushroom[0].y = 144;
    mushroom[1].x = 1024; mushroom[1].y = 128; mushroom[1].sprite = 1;
    return mushroom;
}

Flower* LevelBuilder_1_1::loadFlowers(GSTEXTURE* tex)
{
    flowerCount = 2;
    Flower* flower = new Flower[flowerCount];
    for(int i = 0; i < 2; i++)
    {
        flower[i].width = 16;
        flower[i].height = 16;
        flower[i].type = 2;
        flower[i].spritesheet = tex;
    }
    flower[0].x = 1248; flower[0].y = 144;
    flower[1].x = 1744; flower[1].y = 80;
    return flower;
}

character* LevelBuilder_1_1::loadGoombas(GSTEXTURE* tex)
{    
    goombaCount = 16;
    character* goomba = new character[goombaCount];

    for(int i = 0; i < goombaCount; i++)
    {
        goomba[i].spritesheet = tex;
        goomba[i].direction = 1;
    }
    
    goomba[0].worldCoordinates.x = 352; goomba[0].worldCoordinates.y = 192;goomba[0].direction = 0;
    goomba[1].worldCoordinates.x = 640; goomba[1].worldCoordinates.y = 192;
    goomba[2].worldCoordinates.x = 816; goomba[2].worldCoordinates.y = 192;
    goomba[3].worldCoordinates.x = 848; goomba[3].worldCoordinates.y = 192;
    goomba[4].worldCoordinates.x = 1280; goomba[4].worldCoordinates.y = 64;
    goomba[5].worldCoordinates.x = 1312; goomba[5].worldCoordinates.y = 64;
    goomba[6].worldCoordinates.x = 1552; goomba[6].worldCoordinates.y = 192;
    goomba[7].worldCoordinates.x = 1576; goomba[7].worldCoordinates.y = 192;
    goomba[8].worldCoordinates.x = 1824; goomba[8].worldCoordinates.y = 192;
    goomba[9].worldCoordinates.x = 1848; goomba[9].worldCoordinates.y = 192;
    goomba[10].worldCoordinates.x = 1984; goomba[10].worldCoordinates.y = 192;
    goomba[11].worldCoordinates.x = 2008; goomba[11].worldCoordinates.y = 192;
    goomba[12].worldCoordinates.x = 2048; goomba[12].worldCoordinates.y = 192;
    goomba[13].worldCoordinates.x = 2072; goomba[13].worldCoordinates.y = 192;
    goomba[14].worldCoordinates.x = 2784; goomba[14].worldCoordinates.y = 192;
    goomba[15].worldCoordinates.x = 2808; goomba[15].worldCoordinates.y = 192;
    return goomba;
}

character* LevelBuilder_1_1::loadKoopas(GSTEXTURE* tex)
{
    koopaCount = 1;
    character* koopa = new character[koopaCount];
    koopa[0].spritesheet = tex;

    koopa[0].height = 24;
    koopa[0].worldCoordinates.x = 1712;
    koopa[0].worldCoordinates.y = 184;
    return koopa;
}


LevelBuilder_1_2::LevelBuilder_1_2()
{

}
LevelBuilder_1_2::~LevelBuilder_1_2()
{

}

LevelBuilder::LevelBuilder()
{

}
LevelBuilder::~LevelBuilder()
{

}

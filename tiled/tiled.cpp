//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include <gsKit.h>

#include "textures/mario.h"
#include "textures/hud.h"
#include "textures/title.h"
#include "textures/cursor.h"

#include "controller.hpp"
#include "character.hpp"
#include "block.hpp"
#include "pickup.hpp"
#include "HUD.hpp"

#include "levelBuilder.hpp"
#include "graphics.hpp"
#include "draw.hpp"
#include "utils.h"

#include "textures/spritesheet.h"
#include "textures/goomba.h"
#include "textures/koopa.h"
#include "textures/pickups.h"

GSGLOBAL* character::gsGlobal;
GSGLOBAL* pickup::gsGlobal;
GSGLOBAL* block::gsGlobal;
GSGLOBAL* HUD::gsGlobal;

point* character::viewport;
point* pickup::viewport;
point* block::viewport;

int main()
{
    GSGLOBAL* gsGlobal = gsKit_init_global();
    gsGlobal->Mode = GS_MODE_PAL;
    gsGlobal->Width=640;
    gsGlobal->Height=512;
    //gsGlobal->PSMZ = GS_PSMZ_32;
    gsGlobal->PSM = GS_PSM_CT16;
    gsGlobal->ZBuffering = GS_SETTING_ON;
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
    gsGlobal->DoubleBuffering = GS_SETTING_ON;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    //Initialize the DMAC
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsKit_init_screen(gsGlobal);
    gsKit_set_clamp(gsGlobal, GS_CMODE_CLAMP);
    gsKit_mode_switch(gsGlobal, GS_ONESHOT);
    
    //controller setup
    static int padBuf[256] __attribute__((aligned(64)));
    int port=0, slot=0;
    controller pad;
    pad.loadModules();
    padInit(0);
    pad.openPad(port,slot, padBuf);

    // place the top corner of the screen at this location of the map and render the first tile.
    // int x = 0, y=0; // top left corner of the screen
    point viewport = {0, 0};
    
    int scale_factor = 2;
    u64 bg_color = GS_SETREG_RGBAQ(0x5C,0x94,0xFC,0x00,0x00);

    GSTEXTURE hudTexture = loadTexture(gsGlobal, hud_array, 128, 128, GS_PSM_CT32);
    GSTEXTURE marioSprites = loadTexture(gsGlobal, mario_array, 512, 32, GS_PSM_CT32 );
    GSTEXTURE tilesheet = loadTexture(gsGlobal, spritesheet_array, 128, 128, GS_PSM_CT32 );
    GSTEXTURE pickupTexture = loadTexture(gsGlobal, pickups_array, 64, 64, GS_PSM_CT32);
    GSTEXTURE koopaSprites = loadTexture(gsGlobal, koopa_array, 96, 24, GS_PSM_CT32 );
    GSTEXTURE goombaSprites = loadTexture(gsGlobal, goomba_array, 32, 16, GS_PSM_CT32);

    
    HUD hud;
    HUD::gsGlobal = gsGlobal;
    hud.spritesheet = &hudTexture;
    
    character::gsGlobal = gsGlobal;
    character::viewport = &viewport;
    
    //character mario;
    PlayableCharacter mario;
    mario.spritesheet = &marioSprites;

    pickup::gsGlobal = gsGlobal;
    pickup::viewport = &viewport;

    LevelBuilder levelBuilder(gsGlobal, &tilesheet, &pickupTexture, &koopaSprites, &goombaSprites);
    
    // this will represent the box that was hit
    block block1;
    block::gsGlobal = gsGlobal;
    block::viewport = &viewport;
    
    mario.vy = 0;
    int gravity = 1;
    u8 tick = 0;
    u8 superMario = 0;
    int time = 400;
    int score = 0;
    int lives = 3;
    u8 frameByFrame = 0;
    u8 restart = 0;

    Level* level1 = levelBuilder.build(1, 1);
    drawStartScreen(gsGlobal, &pad, &hud, level1);
    delete level1;
    Level* level = levelBuilder.build(1, 1);
    
    drawLevelStart(gsGlobal, &hud, &mario, score, lives);
    mario.worldCoordinates.x = 0;
    mario.worldCoordinates.y = 192;
    
    while(1)
    {
        // set the background color
        if(viewport.y == 0)bg_color = GS_SETREG_RGBAQ(0x5C,0x94,0xFC,0x00,0x00); // above ground
        else bg_color = GS_SETREG_RGBAQ(0x00,0x00,0x00,0x00,0x00); // underground (in cave)
        gsKit_clear(gsGlobal, bg_color);

        pad.read();
        if(!mario.canMoveDown(level, 1) && !mario.animationMode)mario.sprite = 0;
        else if(!mario.animationMode) mario.sprite = 5;
        // mario fell into a pit
        if(!mario.animationMode && mario.worldCoordinates.y > viewport.y + 208)
        {
            printf("fell\n");
            mario.animationMode = 4;
        }
        if(mario.pipeOnRight(level))mario.animationMode = 5;

        if(!mario.animationMode)
        {
            mario.reactToControllerInput(&pad, tick, level, scale_factor, &superMario, &frameByFrame);
        }
        else
        {
            mario.doAnimation(tick, &superMario, &restart);
        }
        
        if(restart)
        {
            mario.animationMode = 0;
            mario.animationFrame = 0;
            mario.sprite = 0;
            drawLevelStart(gsGlobal, &hud, &mario, score, lives);
            mario.worldCoordinates.x = 0;
            viewport.x = 0;
            mario.worldCoordinates.y = 192;
            mario.collisionDetection = 1;
            restart = 0;
        }
                
        // dealing with gravity
        if(mario.vy > 0)
        {
            // did mario stomp any goombas?
            for(int i = 0; i < level->goombaCount; i++)
            {
                if(level->goomba[i].isOnScreen() && mario.isTouching(&level->goomba[i]))
                {
                    level->goomba[i].sprite = 1;
                    mario.vy = -6;
                    level->goomba[i].activated = 0;
                    level->goomba[i].collisionDetection = 0;
                }
            }
            
            if(!mario.animationMode && mario.canMoveDown(level, mario.vy))
            {
                mario.worldCoordinates.y += mario.vy;
                if((tick & 3) == 0)mario.vy += gravity;
            }
            else if(!mario.animationMode)
            {
                while(mario.vy > 0 && !mario.canMoveDown(level, mario.vy))mario.vy--;
                mario.worldCoordinates.y += mario.vy;
                mario.vy = 0;
                
                mario.sprite = 0;
            }
        }
        else if(mario.vy < 0 || mario.canMoveDown(level, 1)) // jumping
        {
            if(mario.canMoveUp(level, mario.vy*(-1)))
            {
                mario.worldCoordinates.y += mario.vy;
                if((tick & 3) == 0)mario.vy += gravity;
            }
            else
            {
                while(mario.vy < 0 && !mario.canMoveUp(level, mario.vy*(-1)))mario.vy++;
                if(mario.vy == 0)
                {
                    int index = level->get_index(mario.worldCoordinates.x + 8, mario.worldCoordinates.y - 1);
                    point pickup;
                    pickup.x = (index % level->width) * 16;
                    pickup.y = (index / level->width) * 16;

                    Coin* collected_coin = level->getCoin(pickup.x + 4, pickup.y);
                    Flower* collected_flower = level->getFlower(pickup.x, pickup.y);
                    Mushroom* collected_mushroom = level->getMushroom(pickup.x, pickup.y);
                    
                    printf("hit box %d (%d) coin location: <%d, %d>\n", index, level->data[index], pickup.x, pickup.y);
                    if(level->data[index] == 11 || level->data[index] == 33)
                    {
                        block1.sprite = 32;
                        block1.x = ((mario.worldCoordinates.x + 8) >> 4)<<4;
                        block1.y = ((mario.worldCoordinates.y - 1) >> 4)<<4;
                        block1.spritesheet = level->spritesheet;
                        level->data[index] = 0;
                        block1.active = 1;
                        printf("viewport: <%d, %d>", viewport.x, viewport.y);
                        printf("<%d, %d>\n", block1.x, block1.y);

                        if(collected_coin)
                        {
                            collected_coin->activated = 1;
                            score+=200;
                        }
                        else if(collected_flower)
                        {
                            collected_flower->activated = 1;
                        }
                        else if(collected_mushroom)
                        {
                            collected_mushroom->activated = 1;
                        }
                    }
                }
                mario.worldCoordinates.y += mario.vy;
            }
        }
        else if(mario.vy == 0) // mario is only able to get killed by enemies when not jumping or falling
        {
            for(int i = 0; i < level->goombaCount; i++)
            {
                if(mario.animationMode == 0 && level->goomba[i].isOnScreen() && mario.isTouching(&level->goomba[i]))
                {
                    printf("touching a goomba\n");
                    mario.animationMode = 4;
                }
            }
        }
        if(superMario)mario.sprite+=15;
        drawScreen(gsGlobal, level->spritesheet, scale_factor, level, viewport);
        mario.draw();
        for(int i = 0; i < level->coinCount; i++)
        {
            level->coin[i].draw();
            if(i < 10)level->coin[i].update();
            if(mario.pickedup(&level->coin[i]))
            {
                printf("picked up a coin\n");
                level->coin[i].activated = 0;
                score+=200;
            }
        }
        for(int i = 0; i < level->mushroomCount; i++)
        {
            if(mario.pickedup(&level->mushroom[0]))
            {
                printf("picked up mushroom\n");
                level->mushroom[i].activated = 0;
                mario.animationMode = 2;
            }
            level->mushroom[i].draw();
            level->mushroom[i].update();
        }
        for(int i = 0; i < level->flowerCount; i++)
        {
            level->flower[i].draw();
            if((tick & 7)==0)
            {
                level->flower[i].update();
            }
            if(tick&1)
            {
                level->flower[i].sprite++;
                level->flower[i].sprite&=3;
            }
        }
        for(int i = 0;i < level->goombaCount; i++)
        {
            if(level->goomba[i].isOnScreen())level->goomba[i].draw();
        }
        for(int i = 0;i < level->koopaCount; i++)
        {
            if(level->koopa[0].isOnScreen())level->koopa[0].draw();
        }

        if(block1.active)
        {
            block1.draw();
            if(tick&1)
            {
                if(block1.update())
                {
                    int index_x = block1.x / level->tile_width;
                    int index_y = block1.y / level->tile_height;
                    printf("box location: <%d, %d>\n", block1.x, block1.y);
                    block1.active = 0;
                    block1.phase = 0;
                    level->data[(index_y*level->width)+index_x] = 32;
                }
            }
        }
        hud.draw(0, 0);
        hud.drawTime(time);
        hud.drawScore(score);
        hud.drawWorld(1, 1);
        
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, 0);
        if((tick&3) == 0) // every 4 frames
        {
            if((tick&7) == 0) // every 8 frames
            {
                for(int i = 0; i < level->goombaCount; i++)
                {
                    if(level->goomba[i].isOnScreen())level->goomba[i].hflip^=1;
                }
                for(int i = 0; i < level->coinCount; i++)
                {
                    level->coin[i].sprite++;
                    level->coin[i].sprite&=3;
                }
            }
            for(int i = 0; i < level->goombaCount; i++)
            {
                if(level->goomba[i].isOnScreen())
                {
                    level->goomba[i].traverse(level);
                    level->goomba[i].gravity(level,tick, gravity);
                }
            }
            for(int i = 0; i < level->koopaCount; i++)
            {
                level->koopa[0].traverse(level);
                level->koopa[0].gravity(level,tick, gravity);
                level->koopa[0].hflip = level->koopa[0].direction ^ 1;
                if((tick&7) == 0)level->koopa[0].sprite = 2;
                else level->koopa[0].sprite = 3;
            }

        }
        if((tick&31) == 0)time--; // every 32 frames
        tick++;
        }
    return 0;
}

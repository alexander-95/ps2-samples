//  ____     ___ ____
//  ____|   |    ____|
// |     ___|   |____

#include "draw.hpp"
#include <stdio.h>
#include "textures/title.h"
#include "textures/cursor.h"
#include "graphics.hpp"

void drawTile(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, Level* level, point start, int value, int z)
{
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);// set color
    int index_x = value % 8; // 8 is the width of the tilesheet, in tiles
    int index_y = value / 8; // 8 is the width of the tilesheet, in tiles

    int x1 = start.x*scale_factor;
    int y1 = start.y*scale_factor;

    int x2 = (start.x + level->tile_width)*scale_factor;
    int y2 = (start.y + level->tile_height)*scale_factor;

    int u1 = level->tile_width*index_x;
    int v1 = level->tile_height*index_y;

    int u2 = level->tile_width*(index_x+1);
    int v2 = level->tile_height*(index_y+1);
    
    gsKit_prim_quad_texture(gsGlobal, spritesheet,
                            x1, y1, u1, v1, 
                            x1, y2, u1, v2,
                            x2, y1, u2, v1,
                            x2, y2, u2, v2,
                            z,TexCol);
}

void drawScreen(GSGLOBAL* gsGlobal, GSTEXTURE* spritesheet, int scale_factor, Level* level, point viewport)
{
    // figure out the tile to draw
    point tile;
    tile.x = viewport.x / level->tile_width;
    tile.y = viewport.y / level->tile_height;

    // where do we start in the tile
    point point1;
    point1.x = viewport.x % level->tile_width;
    point1.y = viewport.y % level->tile_height;

    // position of the top left corner of the current tile
    point start;
    start.x = 0 - point1.x;
    start.y = 0 - point1.y;

    u8 rowsDrawn = 0;

    while(start.y < gsGlobal->Height && tile.y < level->absoluteHeight && rowsDrawn < level->height)
    {
        while(start.x < gsGlobal->Width && tile.x < level->width)
        {
            int index = tile.y * level->width + tile.x;
            int value = level->data[index];
            int z = level->solid[value]*2;
            drawTile(gsGlobal, spritesheet, scale_factor, level, start, value, z);
            start.x += level->tile_width;
            tile.x++;
        }
        start.x = 0 - point1.x;
        tile.x = viewport.x / level->tile_width;
        start.y += level->tile_height;
        rowsDrawn++;
        tile.y++;
    }
}

void drawLevelStart(GSGLOBAL* gsGlobal, HUD* hud, PlayableCharacter* mario, int score, int lives)
{
    mario->worldCoordinates.x = 128;
    mario->worldCoordinates.y = 128;
    for(u16 tick = 0; tick < 128; tick++)
    {
        hud->draw(0, 0);
        hud->drawScore(score);
        hud->drawWorld(1, 1);
        mario->draw();
        hud->drawLives(lives);
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, 0);
    }
    printf("game started\n");
}

void drawStartScreen(GSGLOBAL* gsGlobal, controller* pad, HUD* hud, Level* level1)
{
    u64 bg_color = GS_SETREG_RGBAQ(0x5C,0x94,0xFC,0x00,0x00);
    u64 TexCol = GS_SETREG_RGBAQ(0x80,0x80,0x80,0x80,0x00);
    GSTEXTURE title;
    GSTEXTURE cursor;
    gsKit_texture_abgr(gsGlobal, &title, title_array, 176, 88, GS_PSM_CT32 );
    gsKit_texture_abgr(gsGlobal, &cursor, cursor_array, 8, 8, GS_PSM_CT32 );
    point p1; p1.x = 160, p1.y = 60;
    point p2; p2.x = 210, p2.y = 250;

    point start; start.x = 0, start.y = 0;

    int x1 = 160;
    int y1 = 60;
    int u1 = 0;
    int v1 = 0;

    int x2 = x1+(title.Width * 2);
    int y2 = y1+(title.Height * 2);
    int u2 = title.Width;
    int v2 = title.Height;

    int x3 = 210;
    int y3 = 250;
    int u3 = 0;
    int v3 = 0;

    int x4 = x3+(cursor.Width * 2);
    int y4 = y3+(cursor.Height * 2);
    int u4 = cursor.Width;
    int v4 = cursor.Height;

    u8 menu_option = 0;
    while(1)
    {
        pad->read();
        if(pad->down())
        {
            menu_option = 1;
            y3 = 282;
            y4 = y3 + (cursor.Height * 2);
        }
        else if(pad->up())
        {
            menu_option = 0;
            y3 = 250;
            y4 = y3 + (cursor.Height * 2);
        }
        if(pad->x(1) && menu_option == 0)break;

        // draw "SUPER MARIO BROS." logo
        gsKit_prim_quad_texture(gsGlobal, &title,
                                x1, y1, u1, v1,
                                x1, y2, u1, v2,
                                x2, y1, u2, v1,
                                x2, y2, u2, v2,
                                5, TexCol);

        // draw goomba cursor
        gsKit_prim_quad_texture(gsGlobal, &cursor,
                                x3, y3, u3, v3,
                                x3, y4, u3, v4,
                                x4, y3, u4, v3,
                                x4, y4, u4, v4,
                                5, TexCol);
        
        drawScreen(gsGlobal, level1->spritesheet, 2, level1, start);
        hud->drawDigit(250,250, 1);
        hud->drawString(282,250, "PLAYER GAME");
        hud->drawDigit(250,280, 2);
        hud->drawString(282,280, "PLAYER GAME");
        
        gsKit_sync_flip(gsGlobal);
        gsKit_queue_exec(gsGlobal);
        gsKit_queue_reset(gsGlobal->Per_Queue);
        gsKit_clear(gsGlobal, bg_color);
    }
}

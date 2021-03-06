/****************************
Copyright © 2006-2015 Luke Salisbury
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
****************************/
#ifndef _DISPLAYOPENGL_H_
#define _DISPLAYOPENGL_H_

#include "display/display_types.h"
#ifndef 
#define 
#endif

bool Lux_OGL_Init( uint16_t  width, uint16_t height, uint8_t bpp, uint16_t * actual_width, uint16_t * actual_height );
void Lux_OGL_Destory();
void Lux_OGL_Display2Screen( int32_t * x, int32_t * y);
void Lux_OGL_BackgroundObject( MapObject background );
void Lux_OGL_Update( uint8_t screen, LuxRect rect);
void Lux_OGL_Show( uint8_t screen );
void Lux_OGL_TextSprites( bool able );

LuxSprite * Lux_OGL_PNGtoSprite( uint8_t * data, uint32_t size );
void Lux_OGL_SetRotation( int16_t roll, int16_t pitch, int16_t yaw );
void Lux_OGL_DisplayPointer( uint8_t player, int16_t x, int16_t y, ObjectEffect effect );

bool Lux_OGL_RefreshSpriteSheet( std::string name, std::map<uint32_t, LuxSprite *> * children );
bool Lux_OGL_LoadSpriteSheet( std::string name, std::map<uint32_t, LuxSprite *> * children);
bool Lux_OGL_LoadSpriteSheetImage( elix::Image * image, std::map<uint32_t, LuxSprite *> * children);
bool Lux_OGL_FreeSpriteSheet( std::map<uint32_t, LuxSprite *> * children);
bool Lux_OGL_FreeSprite( LuxSprite * sprite );
#endif

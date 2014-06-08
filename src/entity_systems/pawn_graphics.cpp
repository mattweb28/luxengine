/****************************
Copyright © 2006-2011 Luke Salisbury
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
****************************/
#include "audio.h"
#include "config.h"
#include "core.h"
#include "display.h"
#include "engine.h"
#include "entity_manager.h"
#include "pawn_helper.h"
#include "world.h"
#include "misc_functions.h"
#include "display_functions.h"
#include "elix_endian.hpp"

#include "ffi_object.h"
#include "ffi_spritesheet.h"
#include "ffi_path.h"
#include "ffi_layer.h"
#include "ffi_shaders.h"

extern const AMX_NATIVE_INFO Graphics_Natives[];


MapObject * Lux_PawnEntity_GetObject(AMX * amx, uint32_t object_id )
{
	MapObject * object = NULL;
	Entity * wanted = Lux_PawnEntity_GetParent(amx);
	if ( wanted == NULL )
		return object;

	if ( lux::world->active_map )
	{
		if ( wanted->_mapid == 0 )
			object = lux::world->GetObject(object_id);
		else
			object = lux::world->active_map->GetObject(object_id);
	}
	return object;
}





/** Misc Functions */

/** pawnGraphicsType
* native GraphicsType();
*/
static cell AMX_NATIVE_CALL pawnGraphicsType(AMX *amx, const cell *params)
{
	return 0;
}

/** pawnSheetRef
* native SheetReference(sheet[], rev);
*/
static cell AMX_NATIVE_CALL pawnSheetRef(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 2 );

	std::string sheet_name = Lux_PawnEntity_GetString(amx, params[1]);

	Lux_FFI_Sheet_Reference( sheet_name.c_str(), (int8_t)params[2] );

	return 0;
}

/** pawnSheetReplace
* native SheetReplace(old[], new[]);
*/
static cell AMX_NATIVE_CALL pawnSheetReplace(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 2 );

	std::string old_sheet = Lux_PawnEntity_GetString(amx, params[1]);
	std::string new_sheet = Lux_PawnEntity_GetString(amx, params[2]);

	if ( old_sheet.length() && new_sheet.length() )
	{
		Lux_FFI_Sheet_Replace( old_sheet.c_str(), new_sheet.c_str() );
	}
	return 0;
}

/** pawnGraphicsDraw
* native GraphicsDraw(string[], type, x, y, z, w, h, c = 0xFFFFFFFF, string_size = sizeof string );
*/
static cell AMX_NATIVE_CALL pawnGraphicsDraw(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 8 );

	MapObject * new_object = new MapObject();
	new_object->sprite = Lux_PawnEntity_GetString(amx, params[1]);
	new_object->type = (uint8_t)params[2];
	new_object->position.x = params[3];
	new_object->position.y = params[4];
	new_object->SetZPos( (int32_t)params[5] );
	new_object->position.w = params[6];
	new_object->position.h = params[7];

	cell_colour colour;
	colour.hex = elix::endian::host32(params[8]);
	new_object->effects.primary_colour = colour.rgba;

	if ( lux::display->AddObjectToLayer(new_object->layer, new_object, false) )
	{
		return 1;
	}
	delete new_object;

	return 0;
}

/** Animation */

/** pawnAnimationGetLength
* native AnimationGetLength(sheet[], anim[]);
*/
static cell AMX_NATIVE_CALL pawnAnimationGetLength(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 2 );

	std::string sheet_name = Lux_PawnEntity_GetString(amx, params[1]);
	std::string sprite_name = Lux_PawnEntity_GetString(amx, params[2]);

	return Lux_FFI_Animation_Length( sheet_name.c_str(), sprite_name.c_str() );
}

/** pawnAnimationCreate
* native AnimationCreate(string[], length);
*/
static cell AMX_NATIVE_CALL pawnAnimationCreate(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 1 );

	std::string animation_name = Lux_PawnEntity_GetString(amx, params[1]);

	return Lux_FFI_Animation_Create( animation_name.c_str() );
}

/** pawnAnimationAddFrame
* native AnimationAddFrame(string[], sprite[], string_size = sizeof string, sprite_size = sizeof sprite);
*/
static cell AMX_NATIVE_CALL pawnAnimationAddFrame(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 3 );

	std::string animation_name = Lux_PawnEntity_GetString(amx, params[1]);
	std::string sprite_name = Lux_PawnEntity_GetString(amx, params[2]);

	return Lux_FFI_Animation_Insert( animation_name.c_str(), sprite_name.c_str(), params[3] );
}

/** Text */

/** pawnTextSprites
* native TextSprites(able);

*/
static cell AMX_NATIVE_CALL pawnTextSprites(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 2 );

	bool enable_text_font = !!params[1];
	std::string new_text_sheet = Lux_PawnEntity_GetString(amx, params[2]);

	Lux_FFI_Text_Sprites( enable_text_font, new_text_sheet.c_str() );

	return (cell)enable_text_font;
}

/** Polygon */

/** pawnPolygonCreate
* native PolygonCreate(string[]);
*/
static cell AMX_NATIVE_CALL pawnPolygonCreate(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 1 );
	std::string name = Lux_PawnEntity_GetString(amx, params[1]);

	return Lux_FFI_Polygon_Create( name.c_str() );
}

/** pawnPolygonAddpoint
* native PolygonAddpoint(string[], x, y, string_size = sizeof string);

*/
static cell AMX_NATIVE_CALL pawnPolygonAddpoint(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 3 );

	std::string name = Lux_PawnEntity_GetString(amx, params[1]);
	return Lux_FFI_Polygon_Add_Point( name.c_str(), params[2], params[3] );

}

/** Display Functions */

/** pawnObjectCreate
* native ObjectCreate(string[], type, x, y, z, w, h, c = 0xFFFFFFFF, global = false, string_size = sizeof string);
*
*/
static cell AMX_NATIVE_CALL pawnObjectCreate(AMX *amx, const cell *p )
{
	ASSERT_PAWN_PARAM( amx, p, 9 );

	uint32_t results = 0;
	std::string sprite;

	sprite = Lux_PawnEntity_GetString(amx, p[1]);

	results = Lux_FFI_Object_Create( (uint8_t)p[9], p[2], p[3], p[4], (int32_t)p[5], (uint16_t)p[6], (uint16_t)p[7], p[8], sprite.c_str() );

	return results;

}

/** pawnObjectPosition
* native ObjectPosition(id, x, y, z, w, h, pos = POSITION_MAP);
*/
static cell AMX_NATIVE_CALL pawnObjectPosition(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 7 );

	uint32_t object_id = params[1];
	int32_t x = params[2];
	int32_t y = params[3];
	int32_t z = params[4];
	uint16_t w = params[5];
	uint16_t h = params[6];

	return Lux_FFI_Object_Postion( object_id, x, y, z, w, h );
}

/** pawnObjectInfo
* native ObjectInfo(id, &w, &h);
*/
static cell AMX_NATIVE_CALL pawnObjectInfo(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 3 );

	cell * xptr, * yptr;
	uint16_t w;
	uint16_t h;
	uint32_t object_id = params[1];

	if ( Lux_FFI_Object_Info( object_id, &w, &h ) )
	{
		xptr = amx_Address(amx, params[2]);
		yptr = amx_Address(amx, params[3]);

		if ( xptr )
			*xptr = w;

		if ( yptr )
			*yptr = h;
		return 1;
	}
	return 0;
}

/** pawnObjectEffect
* native ObjectEffect(id, colour, rotate, scale_w, scale_h, flipmode, _STYLES:style, colour2 = 0xFFFFFFFF);
*/
static cell AMX_NATIVE_CALL pawnObjectEffect(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 8 );

	uint32_t object_id = params[1];
	uint32_t colour1 = params[2];
	uint32_t colour2 = params[8];

	uint16_t rotate = params[3];
	uint16_t scale_w = params[4];
	uint16_t scale_h = params[5];
	uint8_t flipmode = params[6];
	uint8_t style = params[7];

	return Lux_FFI_Object_Effect( object_id, colour1, colour2, rotate, scale_w, scale_h, flipmode, style );

}

/** pawnObjectReplace
* native ObjectReplace(id, string[], type);
*/
static cell AMX_NATIVE_CALL pawnObjectReplace(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 3 );

	std::string sprite;
	uint32_t object_id = (uint32_t)params[1];
	uint8_t type = (uint8_t)params[3];

	sprite = Lux_PawnEntity_GetString(amx, params[2]);

	return Lux_FFI_Object_Replace( object_id, type, sprite.c_str() );
}

/** pawnObjectToggle
* native ObjectToggle(id, show);
*/
static cell AMX_NATIVE_CALL pawnObjectToggle(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 2 );

	uint32_t object_id = (uint32_t)params[1];

	return Lux_FFI_Object_Flag( object_id, 6, (bool)params[2] );

}


/** pawnObjectFlag
* native ObjectFlag(id, key, value);
*/
static cell AMX_NATIVE_CALL pawnObjectFlag(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 3 );

	uint32_t object_id = (uint32_t)params[1];

	return Lux_FFI_Object_Flag( object_id, (uint8_t)params[2], (int16_t)params[3] );

}

/** pawnObjectDelete
* native ObjectDelete(id);
*/
static cell AMX_NATIVE_CALL pawnObjectDelete(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 1 );

	uint32_t object_id = (uint32_t)params[1];

	return Lux_FFI_Object_Delete( object_id );

}

/** pawnObjectFollowPath
* native ObjectFollowPath(object:id, speed, &x, &y, loop);
*/
static cell AMX_NATIVE_CALL pawnObjectFollowPath(AMX *amx, const cell *params)
{
	ASSERT_PAWN_PARAM( amx, params, 5 );

	uint32_t object_id = (uint32_t)params[1];
	int32_t fixed_speed = params[2];
	int16_t x = 0;
	int16_t y = 0;
	uint8_t loop = params[5];

	int32_t result = Lux_FFI_Path_Move_Object( object_id, fixed_speed, &x, &y, loop );

	if ( result >= 0 )
	{
		cell * xptr = NULL;

		xptr = amx_Address(amx, params[3]);
		if (xptr)
			*xptr = x;

		xptr = amx_Address(amx, params[4]);
		if (xptr)
			*xptr = y;
	}

	return result;

}


/** Camera Functions */
/** pawnCameraSetScroll
* native CameraSetScroll(bool:scroll);
*/
static cell AMX_NATIVE_CALL pawnCameraSetScroll(AMX *amx, const cell *params)
{
	lux::world->active_map->SetScrolling( (bool)params[1] );
	return 0;
}


/** pawnLayerSetRotation
* native LayerSetRotation(layer, roll, pitch, yaw);
*/
static cell AMX_NATIVE_CALL pawnLayerSetRotation(AMX *amx, const cell *params)
{
	Lux_FFI_Layer_Rotation( params[1], params[2], params[3], params[4] );

	return 0;
}

/** pawnLayerSetOffset
* native LayerSetOffset(layer, x, y);
*/
static cell AMX_NATIVE_CALL pawnLayerSetOffset(AMX *amx, const cell *params)
{
	Lux_FFI_Layer_Offset( params[1], params[2], params[3] );

	return 0;
}

/** pawnLayerSetEffect
* native LayerSetEffect(layer, x, y);
*/
static cell AMX_NATIVE_CALL pawnLayerSetEffect(AMX *amx, const cell *params)
{


	Lux_FFI_Layer_Apply_Shader( params[1], params[2] );

	return 0;
}

/** pawnLayerColour
* native LayerColour(layer, colour);
*/
static cell AMX_NATIVE_CALL pawnLayerColour(AMX *amx, const cell *params)
{
	Lux_FFI_Layer_Colour( params[1], params[2] );
	return 0;
}


const AMX_NATIVE_INFO Graphics_Natives[] = {
	/** Misc */
	{ "GraphicsDraw", pawnGraphicsDraw },
	{ "GraphicsType", pawnGraphicsType },
	{ "TextSprites", pawnTextSprites }, //native TextSprites(able);
	{ "SheetReference", pawnSheetRef }, //native SheetReference(sheet[], ref = 1);
	{ "SheetReplace", pawnSheetReplace },
	/** Animation */
	{ "AnimationGetLength", pawnAnimationGetLength }, ///native AnimationGetLength(sheet[], anim[]);
	{ "AnimationCreate", pawnAnimationCreate }, ///native AnimationCreate(string[]);
	{ "AnimationAddFrame", pawnAnimationAddFrame }, ///native AnimationAddFrame(string[], sprite[], string_size = sizeof string, sprite_size = sizeof sprite);
	/** Polygon */
	{ "PolygonCreate", pawnPolygonCreate }, ///native PolygonCreate(string[]);
	{ "PolygonAddPoint", pawnPolygonAddpoint }, ///native PolygonAddpoint(string[], x, y, string_size = sizeof string);
	/** Display Functions */
	{ "ObjectCreate", pawnObjectCreate }, ///native ObjectCreate(string[], type, c = 0xFFFFFFFF, string_size = sizeof string);
	{ "ObjectPosition", pawnObjectPosition }, ///native ObjectPosition(id, x, y, z, w, h);
	{ "ObjectEffect", pawnObjectEffect }, ///native ObjectEffect(id, colour = 0xFFFFFFFF, rotate, scale_w, scale_h, mirror, _STYLES:style, color2 = 0xFFFFFFFF);
	{ "ObjectReplace", pawnObjectReplace }, ///native ObjectReplace(id, string[], type, string_size = sizeof string);
	{ "ObjectDelete", pawnObjectDelete }, ///native ObjectDelete(id);
	{ "ObjectFlag", pawnObjectFlag }, ///native ObjectFlag(object:id, key, value);
	{ "ObjectToggle", pawnObjectToggle }, ///native ObjectToggle(object:id, show);
	{ "ObjectInfo", pawnObjectInfo }, ///native ObjectInfo(object:id, &w, &h);
	{ "ObjectFollowPath", pawnObjectFollowPath }, ///native ObjectFollowPath(object:id, speed,&x, &y);
	/** Camera Functions */
	{ "CameraSetScroll", pawnCameraSetScroll }, ///native CameraSetScroll(bool:scroll);
	{ "LayerSetRotation", pawnLayerSetRotation }, ///native LayerSetRotation(layer, roll, pitch, yaw);
	{ "LayerSetOffset", pawnLayerSetOffset }, ///native LayerSetOffset(layer, x, y);
	{ "LayerSetEffect", pawnLayerSetEffect }, ///native LayerSetEffect(layer, effect);
	{ "LayerColour", pawnLayerColour }, ///native LayerColour(layer, colour = 0xFFFFFFFF);
	{ 0, 0 }
};

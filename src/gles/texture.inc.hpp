/****************************
Copyright (c) 2012 Luke Salisbury
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
****************************/

#ifndef _GLES_TEXTURE_
	#define _GLES_TEXTURE_
	#include "gles.hpp"


	namespace lux {
		namespace OpenGLTexture {
			int32_t power( int32_t value )
			{
				int32_t result = 1 ;
				while ( result < value )
					result *= 2 ;
				return result;
			}

			bool create( Texture * texture, mem_pointer data, uint16_t w, uint16_t h, GLenum type )
			{
				glGenTextures(1, &texture->pointer );
				glBindTexture(GL_TEXTURE_2D, texture->pointer);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, type, data);

				return true;
			}
			bool destroy( Texture * texture )
			{
				glDeleteTextures(1, &texture->pointer );
				return true;
			}
		}
	}
#endif

#!/usr/bin/env python

# SPARK(pyspk) Demo Common Routines script
# original source code is by Julien Fryer
#
# ////////////////////////////////////////////////////////////////////////////////
# // SPARK particle engine                                                      //
# // Copyright (C) 2008-2009 - Julien Fryer - julienfryer@gmail.com             //
# //                                                                            //
# // This software is provided 'as-is', without any express or implied          //
# // warranty.  In no event will the authors be held liable for any damages     //
# // arising from the use of this software.                                     //
# //                                                                            //
# // Permission is granted to anyone to use this software for any purpose,      //
# // including commercial applications, and to alter it and redistribute it     //
# // freely, subject to the following restrictions:                             //
# //                                                                            //
# // 1. The origin of this software must not be misrepresented; you must not    //
# //    claim that you wrote the original software. If you use this software    //
# //    in a product, an acknowledgment in the product documentation would be   //
# //    appreciated but is not required.                                        //
# // 2. Altered source versions must be plainly marked as such, and must not be //
# //    misrepresented as being the original software.                          //
# // 3. This notice may not be removed or altered from any source distribution. //
# ////////////////////////////////////////////////////////////////////////////////
#
# translation C/C++ to Python and some modification
# by chromia <chromia@outlook.jp> in 2018
#

import sdl2
import ctypes
import OpenGL.GL as gl
import OpenGL.GLU as glu


def loadTexture(path, type, clamp, mipmap=False):
    img = sdl2.SDL_LoadBMP(path)
    if not img:
        print('Unable to load bitmap :', sdl2.SDL_GetError())
        return 0

    width = img.contents.w
    height = img.contents.h

    # converts from BGR to RGB
    if (type == gl.GL_RGB) or (type == gl.GL_RGBA):
        if type == gl.GL_RGB:
            offset = 3
        else:
            offset = 4
        imagesize = width*height*offset
        b = ctypes.cast(img.contents.pixels,
                        ctypes.POINTER(ctypes.c_byte))
        for i in range(0, imagesize, offset):
            b[i], b[i+2] = b[i+2], b[i]  # slow...

    index = int(gl.glGenTextures(1))
    gl.glBindTexture(gl.GL_TEXTURE_2D, index)
    gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_S, clamp)
    gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_T, clamp)
    gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MAG_FILTER,
                       gl.GL_LINEAR)

    if mipmap:
        gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MIN_FILTER,
                           gl.GL_LINEAR_MIPMAP_LINEAR)
        bytes = ctypes.cast(img.contents.pixels,
                            ctypes.POINTER(ctypes.c_byte))
        glu.gluBuild2DMipmaps(gl.GL_TEXTURE_2D, type, width, height,
                              type, gl.GL_UNSIGNED_BYTE, bytes)
    else:
        gl.glTexParameteri(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MIN_FILTER,
                           gl.GL_LINEAR)
        bytes = ctypes.cast(img.contents.pixels,
                            ctypes.POINTER(ctypes.c_byte))
        gl.glTexImage2D(gl.GL_TEXTURE_2D, 0, type, width, height,
                        0, type, gl.GL_UNSIGNED_BYTE, bytes)
    sdl2.SDL_FreeSurface(img)
    return index


def drawBox(AABBMin, AABBMax, r, g, b):
    gl.glDisable(gl.GL_TEXTURE_2D)
    gl.glBegin(gl.GL_LINES)
    gl.glColor3f(r, g, b)

    gl.glVertex3f(AABBMin.x, AABBMin.y, AABBMin.z)
    gl.glVertex3f(AABBMax.x, AABBMin.y, AABBMin.z)

    gl.glVertex3f(AABBMin.x, AABBMin.y, AABBMin.z)
    gl.glVertex3f(AABBMin.x, AABBMax.y, AABBMin.z)

    gl.glVertex3f(AABBMin.x, AABBMin.y, AABBMin.z)
    gl.glVertex3f(AABBMin.x, AABBMin.y, AABBMax.z)

    gl.glVertex3f(AABBMax.x, AABBMax.y, AABBMax.z)
    gl.glVertex3f(AABBMin.x, AABBMax.y, AABBMax.z)

    gl.glVertex3f(AABBMax.x, AABBMax.y, AABBMax.z)
    gl.glVertex3f(AABBMax.x, AABBMin.y, AABBMax.z)

    gl.glVertex3f(AABBMax.x, AABBMax.y, AABBMax.z)
    gl.glVertex3f(AABBMax.x, AABBMax.y, AABBMin.z)

    gl.glVertex3f(AABBMin.x, AABBMin.y, AABBMax.z)
    gl.glVertex3f(AABBMax.x, AABBMin.y, AABBMax.z)

    gl.glVertex3f(AABBMin.x, AABBMin.y, AABBMax.z)
    gl.glVertex3f(AABBMin.x, AABBMax.y, AABBMax.z)

    gl.glVertex3f(AABBMin.x, AABBMax.y, AABBMin.z)
    gl.glVertex3f(AABBMax.x, AABBMax.y, AABBMin.z)

    gl.glVertex3f(AABBMin.x, AABBMax.y, AABBMin.z)
    gl.glVertex3f(AABBMin.x, AABBMax.y, AABBMax.z)

    gl.glVertex3f(AABBMax.x, AABBMin.y, AABBMin.z)
    gl.glVertex3f(AABBMax.x, AABBMax.y, AABBMin.z)

    gl.glVertex3f(AABBMax.x, AABBMin.y, AABBMin.z)
    gl.glVertex3f(AABBMax.x, AABBMin.y, AABBMax.z)
    gl.glEnd()


def drawBoundingBox(group, r, g, b):
    if not group.isAABBComputingEnabled():
        return
    AABBMin = group.getAABBMin()
    AABBMax = group.getAABBMax()
    drawBox(AABBMin, AABBMax, r, g, b)


if __name__ == '__main__':
    pass

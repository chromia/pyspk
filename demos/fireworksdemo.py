#!/usr/bin/env python

# SPARK(pyspk) Fireworks Demo script
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
# and renaming from 'Tuto1' to 'Fireworks'
# by chromia <chromia@outlook.jp> in 2018
#

# standard libs
import ctypes
import math
from pathlib import Path
import time
# SDL
import sdl2
# pyopengl
import OpenGL.GL as gl
import OpenGL.GLU as glu
# SPARK(pyspk)
import pyspk as spk
from pyspk.GL import GLPointRenderer
from democommon import loadTexture

try:
    # FTGL
    import FTGL
    notext = False
except ImportError:
    notext = True


class BaseSystem:
    def __init__(self, screenheight, texture):
        # create Model
        model_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA
        model_mutable = spk.FLAG_ALPHA
        model_random = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE
        model = spk.Model.create(model_enable, model_mutable, model_random)
        model.setParam(spk.PARAM_ALPHA, 1.0, 0.0)  # This makes the particles fade out over time
        model.setLifeTime(1.0, 2.0)
        # create Renderer
        renderer = GLPointRenderer.create()
        renderer.setType(spk.POINT_SPRITE)
        renderer.enableWorldSize(True)
        GLPointRenderer.setPixelPerUnit(45.0 * math.pi / 180.0, screenheight)
        renderer.setSize(0.1)
        renderer.setTexture(texture)
        renderer.enableBlending(True)
        renderer.setBlendingFunctions(gl.GL_SRC_ALPHA, gl.GL_ONE)  # additive blending
        renderer.setTextureBlending(gl.GL_MODULATE)  # the texture is modulated with the particle's color
        renderer.enableRenderingHint(spk.DEPTH_TEST, False)  # the depth test is disabled
        self.renderer = renderer
        # create Zone
        source = spk.Point.create()
        # create Emitter
        emitter = spk.RandomEmitter.create()
        emitter.setForce(2.8, 3.2)
        emitter.setZone(source)
        emitter.setTank(500)
        emitter.setFlow(-1)  # Creates all the particles in the tank at the first frame
        # create Group
        group = spk.Group.create(model, 500)  # 500 particles is the maximum capacity of the group
        group.addEmitter(emitter)
        group.setGravity(spk.Vector3D(0.0, -1.0, 0.0))
        group.setFriction(2.0)
        group.setRenderer(renderer)
        # create System
        system = spk.System.create()
        system.addGroup(group)
        self.system = system
        # Defines which objects will be shared by all systems
        model.setShared(True)
        renderer.setShared(True)

    def createSystem(self, pos, color):
        # Creates a copy of the base system
        system = spk.copySystem(self.system)
        group = system.getGroup(0)
        # Updates the model with the given color
        model = group.getModel()
        model.setParam(spk.PARAM_RED, max(0.0, color.x - 0.25), min(1.0, color.x + 0.25))
        model.setParam(spk.PARAM_GREEN, max(0.0, color.y - 0.25), min(1.0, color.y + 0.25))
        model.setParam(spk.PARAM_BLUE, max(0.0, color.z - 0.25), min(1.0, color.z + 0.25))
        # Locates the zone of the emitter at the given position
        zone = group.getEmitter(0).getZone()
        zone.setPosition(pos)
        return system


class App:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        # init SDL
        sdl2.SDL_Init(sdl2.SDL_INIT_EVERYTHING)
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_DOUBLEBUFFER, 1)
        self.window = sdl2.SDL_CreateWindow(
            b"SPARK(&pyspk) Fireworks Demo",
            sdl2.SDL_WINDOWPOS_UNDEFINED,
            sdl2.SDL_WINDOWPOS_UNDEFINED,
            width, height,
            sdl2.SDL_WINDOW_SHOWN | sdl2.SDL_WINDOW_OPENGL)
        sdl2.SDL_CaptureMouse(True)

        # init OpenGL
        self.context = sdl2.SDL_GL_CreateContext(self.window)
        sdl2.SDL_GL_SetSwapInterval(0)
        gl.glClearColor(0.0, 0.0, 0.0, 1.0)
        surface = sdl2.SDL_GetWindowSurface(self.window)
        gl.glViewport(0, 0, surface.contents.w, surface.contents.h)
        # load Textures
        texture = loadTexture(b'res/flare.bmp', gl.GL_ALPHA, gl.GL_REPEAT)

        # init FTGL
        if not notext:
            font = FTGL.TextureFont('res/font.ttf')
            font.FaceSize(24)
            self.font = font

        # init SPARK(pyspk)
        # random seed initialization
        spk.setRandomSeed(int(time.time()))
        # step configuration
        spk.System.setClampStep(True, 0.1)  # clamp the step to 100 ms
        spk.System.useAdaptiveStep(0.001, 0.01)  # use an adaptive step from 1ms to 10ms (1000fps to 100fps)

        # the base system
        self.basesystem = BaseSystem(self.height, texture)

        print('SPARK FACTORY AFTER INIT :')
        spk.Factory.getInstance().traceAll()

        # init Variables
        self.running = True
        self.paused = False
        self.deltaTime = 0
        self.step = 0
        self.text = 2
        self.renderValue = 0
        self.angleY = 0.0
        self.camPosZ = 5.0
        self.spacePressed = -1.0
        self.systems = []
        self.nbParticles = ''
        self.fps = ''
        self.frames = [sdl2.SDL_GetTicks()-1]
        self.lasttime = self.frames[-1]

    def clear(self):
        # release resources
        if self.context:
            sdl2.SDL_GL_DeleteContext(self.context)
            self.context = None
        if self.window:
            sdl2.SDL_DestroyWindow(self.window)
            self.window = None

    def run(self):
        event = sdl2.SDL_Event()
        while self.running:
            while sdl2.SDL_PollEvent(ctypes.byref(event)) != 0:
                self.onEvent(event)
            # sdl2.SDL_Delay(10)  # uncomment this if you want to save CPU resources
            self.onLoop()
        self.onCleanup()
        sdl2.SDL_Quit()

    def onCleanup(self):
        # release SPARK(pyspk) resources
        print('SPARK FACTORY BEFORE DESTRUCTION :')
        spk.Factory.getInstance().traceAll()
        spk.Factory.getInstance().destroyAll()
        print('SPARK FACTORY AFTER DESTRUCTION :')
        spk.Factory.getInstance().traceAll()
        spk.Factory.destroyInstance()

        # release other resources
        self.clear()

    def onEvent(self, event):
        # interaction
        # quit with ESC or close button
        if event.type == sdl2.SDL_QUIT:
            self.running = False
        elif event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_ESCAPE:
            self.running = False
        # text switching with F1
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F1:
            self.text = (self.text-1) % 3
        # PAUSE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_PAUSE:
            self.paused = not self.paused
        # switching graphics with SPACE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_SPACE:
            self.spacePressed = 200.0
        if event.type == sdl2.SDL_KEYUP and event.key.keysym.sym == sdl2.SDLK_SPACE:
            self.spacePressed = -1.0
        # Camera rotation with mouse
        if event.type == sdl2.SDL_MOUSEMOTION:
            sensitive = 0.4
            self.angleY += event.motion.xrel * sensitive
        # Camera zooming with mouse wheel
        if event.type == sdl2.SDL_MOUSEWHEEL:
            if event.wheel.y < 0:
                self.camPosZ = min(10.0, self.camPosZ + 0.5)
            elif event.wheel.y > 0:
                self.camPosZ = max(0.5, self.camPosZ - 0.5)

    def convertHSV2RGB(self, hsv):
        """ Converts a HSV color to RGB
        h E [0,360]
        s E [0,1]
        v E [0,1]
        :type hsv: Vector3D
        :rtype: Vector3D
        """
        h, s, v = hsv.x, hsv.y, hsv.z
        hi = (h // 60.0) % 6
        f = h / 60.0 - hi
        p = v * (1.0 - s)
        q = v * (1.0 - f * s)
        t = v * (1.0 - (1.0 - f) * s)

        if hi == 0:
            return spk.Vector3D(v, t, p)
        elif hi == 1:
            return spk.Vector3D(q, v, p)
        elif hi == 2:
            return spk.Vector3D(p, v, t)
        elif hi == 3:
            return spk.Vector3D(p, q, v)
        elif hi == 4:
            return spk.Vector3D(t, p, v)
        else:
            return spk.Vector3D(v, p, q)

    def onLoop(self):
        # get elapsed time
        t = sdl2.SDL_GetTicks()
        self.deltaTime = t - self.frames[-1]

        if not self.paused:
            # if space is pressed, a new system is added
            if self.spacePressed >= 0.0:
                self.spacePressed += self.deltaTime

                if self.spacePressed >= 200.0:
                    color = spk.Vector3D(spk.random(0.0, 360.0), 0.8, 1.0)
                    position = spk.Vector3D(spk.random(-2.0, 2.0), spk.random(-2.0, 2.0), spk.random(-2.0, 2.0))
                    newsystem = self.basesystem.createSystem(position, self.convertHSV2RGB(color))
                    self.systems.append(newsystem)

                    self.spacePressed = 0.0

            systems = []
            for system in self.systems:
                # Updates the particle systems
                if system.update(self.deltaTime * 0.001):
                    # the system is alive
                    systems.append(system)
                else:
                    # If a system is sleeping, destroys it and removes from list
                    spk.destroySystem(system)
            self.systems = systems

        # rendering
        self.onRender()

        # time logging
        self.frames.append(t)
        while self.frames[-1] - self.frames[0] > 1000 and len(self.frames) > 2:
            self.frames.pop(0)

        # update text
        nbParticles = sum([s.getNbParticles() for s in self.systems])
        self.nbParticles = "NB PARTICLES : " + str(nbParticles)
        fps = ((len(self.frames)-1) * 1000.0) // (self.frames[-1] - self.frames[0])
        self.fps = "FPS : " + str(fps)

        if notext and t - self.lasttime >= 1000:
            # output informations to console
            print(self.fps, ',', self.nbParticles)
            self.lasttime = t

    def drawText(self, text, x, y):
        gl.glPushMatrix()
        gl.glTranslated(x, y, 0.0)
        self.font.Render(text)
        gl.glPopMatrix()

    def onRender(self):
        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)

        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        glu.gluPerspective(45, self.width / self.height, 0.01, 80.0)

        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()

        gl.glTranslated(0.0, 0.0, -self.camPosZ)  # apply camera zooming
        gl.glRotated(self.angleY, 0.0, 1.0, 0.0)  # apply camera rotation

        # draw particles
        for system in self.systems:
            system.render()

        # draw texts(F1 key)
        if not notext and self.text != 0:
            gl.glMatrixMode(gl.GL_MODELVIEW)
            gl.glLoadIdentity()
            gl.glMatrixMode(gl.GL_PROJECTION)
            gl.glLoadIdentity()
            glu.gluOrtho2D(0, self.width, 0, self.height)
            gl.glDisable(gl.GL_DEPTH_TEST)
            gl.glColor3d(1.0, 1.0, 1.0)
            if self.text == 2:
                self.drawText(self.nbParticles, 4.0, 40.0)
            self.drawText(self.fps, 4.0, 8.0)

        # refresh
        sdl2.SDL_GL_SwapWindow(self.window)


if __name__ == '__main__':
    if not Path('./res').exists():
        print('Runtime Error: resources are not found.')
        print('please place the resource directory(\'./res/\') to current directory.')
    else:
        w = 640
        h = 480
        App(width=w, height=h).run()

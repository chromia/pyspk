#!/usr/bin/env python

# SPARK(pyspk) Writing Demo script
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
# FTGL
import FTGL
# SPARK(pyspk)
import pyspk as spk
from pyspk.GL import GLPointRenderer, GLQuadRenderer
from democommon import loadTexture, drawBoundingBox


class App:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        # init SDL
        sdl2.SDL_Init(sdl2.SDL_INIT_EVERYTHING)
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_DOUBLEBUFFER, 1)
        self.window = sdl2.SDL_CreateWindow(
            b"SPARK(&pyspk) Writing Demo",
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
        # load texture
        texture = loadTexture(b'res/point.bmp', gl.GL_ALPHA, gl.GL_CLAMP)

        # init FTGL
        font = FTGL.TextureFont('res/font.ttf')
        font.FaceSize(24)
        self.font = font

        # init SPARK(pyspk)
        # random seed initialization
        spk.setRandomSeed(int(time.time()))
        # step configuration
        spk.System.setClampStep(True, 0.1)  # clamp the step to 100 ms
        spk.System.useAdaptiveStep(0.001, 0.01)  # use an adaptive step from 1ms to 10ms (1000fps to 100fps)
        # create Renderer
        renderer = GLQuadRenderer.create(1.0)  # type: GLQuadRenderer
        renderer.enableBlending(True)
        renderer.setBlendingFunctions(gl.GL_SRC_ALPHA, gl.GL_ONE)
        renderer.setTexturingMode(spk.TEXTURE_2D)
        renderer.setTexture(texture)
        renderer.setTextureBlending(gl.GL_MODULATE)
        renderer.setScale(0.05, 0.05)
        renderer.enableRenderingHint(spk.DEPTH_TEST, False)
        self.renderer = renderer
        self.basicRenderer = GLPointRenderer.create(1.0)  # bare renderer for comparison(F4)
        # create Model
        model_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | spk.FLAG_SIZE
        model_mutable = spk.FLAG_ALPHA | spk.FLAG_SIZE
        model = spk.Model.create(model_enable, model_mutable)  # type: spk.Model
        model.setParam(spk.PARAM_ALPHA, 5.0, 0.0)  # the particles will fade as the die
        model.setParam(spk.PARAM_SIZE, 1.0, 15.0)  # the particles will enlarge over time
        model.setLifeTime(5.0, 6.0)
        self.model = model
        # create Emitter
        # We set up a spheric emitter that emits in all direction with a very small force
        # in order to slightly displace the particles
        emitter = spk.RandomEmitter.create()
        emitter.setForce(0.01, 0.01)
        self.emitter = emitter  # uses it at later directly
        # create Group
        group = spk.Group.create(model, 14000)  # type: spk.Group
        group.setRenderer(renderer)
        group.setFriction(-0.3)  # negative friction: The particles will accelerate over time
        self.group = group
        # create System
        system = spk.System.create()  # type: spk.System
        system.addGroup(group)
        self.system = system

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
        self.angleX = 0.0
        self.camPosZ = 5.0
        self.add = False
        self.oldX = 0
        self.oldY = 0
        self.oldZ = 0
        self.offset = 0.0
        self.nbParticles = ''
        self.fps = ''
        self.frames = [sdl2.SDL_GetTicks()-1]

        # This computes the ratio to go from screen coordinates to universe coordinates
        self.screenToUniverse = 2.0 * self.camPosZ * math.tan(45.0 * 0.5 * math.pi / 180.0) / self.height

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
        # reset with DEL
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_DELETE:
            self.system.empty()
        # text switching with F1
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F1:
            self.text = (self.text-1) % 3
        # on/off Bounding Box with F2
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F2:
            self.group.enableAABBComputing(not self.group.isAABBComputingEnabled())
            if self.paused:
                self.system.computeAABB()
        # renderer switching with F4
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F4:
            renderers = [self.renderer, self.basicRenderer, None]
            self.renderValue = (self.renderValue+1) % 3
            self.group.setRenderer(renderers[self.renderValue])
        # PAUSE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_PAUSE:
            self.paused = not self.paused
        # reset particle's speed with SPACE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_SPACE:
            for i in range(self.group.getNbParticles()):
                velocity = self.group.getParticle(i).velocity()  # velocity() returns internal reference
                velocity.x = spk.random(-0.5, 0.5)
                velocity.y = spk.random(-0.5, 0.5)
                velocity.z = spk.random(-0.5, 0.5)
        # Camera zooming with mouse wheel
        if event.type == sdl2.SDL_MOUSEWHEEL:
            if event.wheel.y < 0:
                self.camPosZ = min(10.0, self.camPosZ + 0.5)
            elif event.wheel.y > 0:
                self.camPosZ = max(0.5, self.camPosZ - 0.5)

        # Writing with MouseButtons
        self.oldZ = self.camPosZ
        if event.type == sdl2.SDL_MOUSEBUTTONDOWN:
            if event.button.button == sdl2.SDL_BUTTON_LEFT:
                self.add = True
                self.oldX = (event.motion.x - self.width * 0.5) * self.screenToUniverse
                self.oldY = -(event.motion.y - self.height * 0.5) * self.screenToUniverse
        if event.type == sdl2.SDL_MOUSEBUTTONUP:
            if event.button.button == sdl2.SDL_BUTTON_LEFT:
                self.add = False
        if self.add and not self.paused:
            x = (event.motion.x - self.width * 0.5) * self.screenToUniverse
            y = -(event.motion.y - self.height * 0.5) * self.screenToUniverse
            self.offset = self.group.addParticles(start=spk.Vector3D(self.oldX, self.oldY, self.oldZ - 5.0),
                                                  end=spk.Vector3D(x, y, self.camPosZ - 5.0),
                                                  emitter=self.emitter,
                                                  step=0.025,
                                                  offset=self.offset)
            self.oldX = x
            self.oldY = y
            self.group.flushAddedParticles()

    def onLoop(self):
        # get elapsed time
        t = sdl2.SDL_GetTicks()
        self.deltaTime = t - self.frames[-1]

        if not self.paused:
            # Changes the color of the model over time
            self.step += self.deltaTime * 0.0005
            self.model.setParam(spk.PARAM_RED, 0.6 + 0.4 * math.sin(self.step))
            self.model.setParam(spk.PARAM_GREEN, 0.6 + 0.4 * math.sin(self.step + math.pi * 2.0 / 3.0))
            self.model.setParam(spk.PARAM_BLUE, 0.6 + 0.4 * math.sin(self.step + math.pi * 4.0 / 3.0))

            # Updates particle system
            self.system.update(self.deltaTime * 0.001)  # 1 being a second

        # rendering
        self.onRender()

        # time logging
        self.frames.append(t)
        while self.frames[-1] - self.frames[0] > 1000 and len(self.frames) > 2:
            self.frames.pop(0)

        # update text
        self.nbParticles = "NB PARTICLES : " + str(self.system.getNbParticles())
        fps = ((len(self.frames)-1) * 1000.0) // (self.frames[-1] - self.frames[0])
        self.fps = "FPS : " + str(fps)

    def drawText(self, text, x, y):
        gl.glPushMatrix()
        gl.glTranslated(x, y, 0.0)
        self.font.Render(text)
        gl.glPopMatrix()

    def onRender(self):
        gl.glDepthMask(gl.GL_TRUE)
        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)

        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        glu.gluPerspective(45, self.width / self.height, 0.01, 80.0)

        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()

        gl.glTranslated(0.0, 0.0, -self.camPosZ)  # apply camera zooming

        # draw particles
        self.system.render()

        # draw bounding box(F2 key)
        gl.glDisable(gl.GL_BLEND)
        drawBoundingBox(self.group, 1.0, 0.0, 0.0)

        # draw texts(F1 key)
        if self.text != 0:
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

#!/usr/bin/env python

# SPARK(pyspk) Galaxy Demo script
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
from pathlib import Path
# SDL
import sdl2
# pyopengl
import OpenGL.GL as gl
import OpenGL.GLU as glu
# SPARK(pyspk)
import pyspk as spk
from pyspk.GL import GLPointRenderer, GLQuadRenderer
from democommon import loadTexture, drawBoundingBox

try:
    # FTGL
    import FTGL
    notext = False
except ImportError:
    notext = True


class App:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        # init SDL
        sdl2.SDL_Init(sdl2.SDL_INIT_EVERYTHING)
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_DOUBLEBUFFER, 1)
        self.window = sdl2.SDL_CreateWindow(
            b"SPARK(&pyspk) Galaxy Demo",
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
        gl.glEnable(gl.GL_DEPTH_TEST)
        # load Textures
        texture = loadTexture(b'res/flare.bmp', gl.GL_ALPHA, gl.GL_CLAMP)

        # init FTGL
        if not notext:
            font = FTGL.TextureFont('res/font.ttf')
            font.FaceSize(24)
            self.font = font

        # init SPARK(pyspk)
        # step configuration
        spk.System.setClampStep(True, 0.1)  # clamp the step to 100 ms
        spk.System.useRealStep()  # no adjustment
        # create Renderer
        renderer = GLQuadRenderer.create()  # type: GLQuadRenderer
        renderer.setTexturingMode(spk.TEXTURE_2D)
        renderer.setTexture(texture)
        renderer.setTextureBlending(gl.GL_MODULATE)
        renderer.setScale(0.05, 0.05)
        renderer.setBlending(spk.BLENDING_ADD)
        renderer.enableRenderingHint(spk.DEPTH_WRITE, False)
        self.renderer = renderer
        self.basicRenderer = GLPointRenderer.create(1.0)  # bare renderer for comparison(F4)
        # create Model
        galaxy_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | spk.FLAG_SIZE
        galaxy_mutable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE
        galaxy_random = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_SIZE
        galaxy_interpolated = spk.FLAG_ALPHA
        galaxyModel = spk.Model.create(galaxy_enable, galaxy_mutable,
                                       galaxy_random, galaxy_interpolated)  # type: spk.Model
        galaxyModel.setParam(spk.PARAM_RED, 0.0, 0.3, 0.5, 0.5)
        galaxyModel.setParam(spk.PARAM_GREEN, 0.0, 0.3, 0.5, 0.5)
        galaxyModel.setParam(spk.PARAM_BLUE, 1.0, 0.1)
        galaxyModel.setParam(spk.PARAM_SIZE, 0.1, 5.0)
        galaxyModel.setLifeTime(35.0, 40.0)
        ip_alpha = galaxyModel.getInterpolator(spk.PARAM_ALPHA)
        ip_alpha.addEntry(0.0, 0.0)
        ip_alpha.addEntry(0.95, 0.6, 1.0)
        ip_alpha.addEntry(1.0, 0.0)
        star_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | spk.FLAG_SIZE
        star_mutable = spk.FLAG_NONE
        star_random = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_ALPHA | spk.FLAG_SIZE
        starModel = spk.Model.create(star_enable, star_mutable, star_random)  # type: spk.Model
        starModel.setParam(spk.PARAM_RED, 0.8, 1.0)
        starModel.setParam(spk.PARAM_GREEN, 0.4, 1.0)
        starModel.setParam(spk.PARAM_BLUE, 0.8, 1.0)
        starModel.setParam(spk.PARAM_ALPHA, 0.2, 1.0)
        starModel.setParam(spk.PARAM_SIZE, 0.1, 5.0)
        starModel.setImmortal(True)
        # create Emitter
        # 2 emitters in diagonal lines on Y=0 plane
        lineEmitter1 = spk.RandomEmitter.create()  # type: spk.RandomEmitter
        lineEmitter1.setZone(spk.Line.create(spk.Vector3D(-2.5, 0.0, -2.5), spk.Vector3D(2.5, 0.0, 2.5)))
        lineEmitter1.setFlow(100)
        lineEmitter1.setForce(0.0, 0.01)
        lineEmitter2 = spk.RandomEmitter.create()
        lineEmitter2.setZone(spk.Line.create(spk.Vector3D(-2.5, 0.0, 2.5), spk.Vector3D(2.5, 0.0, -2.5)))
        lineEmitter2.setFlow(100)
        lineEmitter2.setForce(0.0, 0.01)
        # create Modifier
        vortex = spk.Vortex.create()  # type: spk.Vortex
        vortex.setRotationSpeed(0.4, False)  # True->[rad/sec], False->[unit/src]
        vortex.setAttractionSpeed(0.04, True)
        vortex.setEyeRadius(0.05)
        vortex.enableParticleKilling(True)  # kill attracted particles
        # create Group
        galaxyGroup = spk.Group.create(galaxyModel, 8000)  # type: spk.Group
        galaxyGroup.addEmitter(lineEmitter1)
        galaxyGroup.addEmitter(lineEmitter2)
        galaxyGroup.setRenderer(renderer)
        galaxyGroup.addModifier(vortex)
        self.galaxyGroup = galaxyGroup
        starGroup = spk.Group.create(starModel, 1000)
        starGroup.setRenderer(renderer)
        skySphere = spk.Sphere(spk.Vector3D(), 16.0)
        starGroup.addParticles(1000, zone=skySphere, velocity=spk.Vector3D(), full=False)
        starGroup.flushAddedParticles()
        # create System
        galaxySystem = spk.System.create()  # type: spk.System
        galaxySystem.addGroup(galaxyGroup)
        self.galaxySystem = galaxySystem
        starSystem = spk.System.create()
        starSystem.addGroup(starGroup)
        self.starSystem = starSystem

        print('SPARK FACTORY AFTER INIT :')
        spk.Factory.getInstance().traceAll()

        # init Variables
        self.running = True
        self.paused = False
        self.deltaTime = 0
        self.step = 0
        self.text = 2
        self.renderValue = 0
        self.renderEnv = True
        self.angleY = 0.0
        self.angleX = 45.0
        self.camPosZ = 5.0
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
        # reset with DEL
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_DELETE:
            self.galaxySystem.empty()
        # text switching with F1
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F1:
            self.text = (self.text-1) % 3
        # on/off Bounding Box with F2
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F2:
            self.galaxyGroup.enableAABBComputing(not self.galaxyGroup.isAABBComputingEnabled())
            if self.paused:
                self.galaxySystem.computeAABB()
        # renderer switching with F4
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F4:
            renderers = [self.renderer, self.basicRenderer, None]
            self.renderValue = (self.renderValue+1) % 3
            self.galaxyGroup.setRenderer(renderers[self.renderValue])
            self.starGroup.setRenderer(renderers[self.renderValue])
        # on/off fog with F5
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F5:
            self.renderEnv = not self.renderEnv
        # PAUSE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_PAUSE:
            self.paused = not self.paused
        # Camera rotation with mouse
        if event.type == sdl2.SDL_MOUSEMOTION:
            sensitive = 0.4
            self.angleY += event.motion.xrel * sensitive
            self.angleX += event.motion.yrel * sensitive
            self.angleX = min(90.0, max(-90.0, self.angleX))
        # Camera zooming with mouse wheel
        if event.type == sdl2.SDL_MOUSEWHEEL:
            if event.wheel.y < 0:
                self.camPosZ = min(15.0, self.camPosZ + 0.5)
            elif event.wheel.y > 0:
                self.camPosZ = max(3.0, self.camPosZ - 0.5)

    def onLoop(self):
        # get elapsed time
        t = sdl2.SDL_GetTicks()
        self.deltaTime = t - self.frames[-1]

        if not self.paused:
            # update time of particle system
            self.galaxySystem.update(self.deltaTime * 0.001)  # normalize 1.0 as 1[sec]

        # rendering
        self.onRender()

        # time logging
        self.frames.append(t)
        while self.frames[-1] - self.frames[0] > 1000 and len(self.frames) > 2:
            self.frames.pop(0)

        # update text
        NbParticles = self.starSystem.getNbParticles() + self.galaxySystem.getNbParticles()
        self.nbParticles = "NB PARTICLES : " + str(NbParticles)
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
        gl.glDepthMask(gl.GL_TRUE)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)

        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        glu.gluPerspective(45, self.width / self.height, 0.01, 33.0)

        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()

        gl.glTranslated(0.0, 0.0, -self.camPosZ)  # apply camera zooming
        gl.glRotated(self.angleX, 1.0, 0.0, 0.0)  # apply camera rotation
        gl.glRotated(self.angleY, 0.0, 1.0, 0.0)

        # draw particles
        self.galaxySystem.render()
        if self.renderEnv:
            self.starSystem.render()

        # draw bounding box(F2 key)
        gl.glDisable(gl.GL_BLEND)
        drawBoundingBox(self.galaxyGroup, 1.0, 0.0, 0.0)

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

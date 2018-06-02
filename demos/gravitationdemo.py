#!/usr/bin/env python

# SPARK(pyspk) Gravitation Demo script
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
# SPARK(pyspk)
import pyspk as spk
from pyspk.GL import GLPointRenderer, GLLineTrailRenderer
from democommon import drawBoundingBox

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
            b"SPARK(&pyspk) Gravitation Demo",
            sdl2.SDL_WINDOWPOS_UNDEFINED,
            sdl2.SDL_WINDOWPOS_UNDEFINED,
            width, height,
            sdl2.SDL_WINDOW_SHOWN | sdl2.SDL_WINDOW_OPENGL)
        sdl2.SDL_CaptureMouse(True)

        # init OpenGL
        self.context = sdl2.SDL_GL_CreateContext(self.window)
        sdl2.SDL_GL_SetSwapInterval(0)
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_MULTISAMPLEBUFFERS, 1)
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_MULTISAMPLESAMPLES, 4)
        gl.glClearColor(0.0, 0.0, 0.0, 1.0)
        surface = sdl2.SDL_GetWindowSurface(self.window)
        gl.glViewport(0, 0, surface.contents.w, surface.contents.h)

        # init FTGL
        if not notext:
            font = FTGL.TextureFont('res/font.ttf')
            font.FaceSize(24)
            self.font = font

        # init SPARK(pyspk)
        # random seed initialization
        spk.setRandomSeed(int(time.time()))
        # step configuration
        spk.System.setClampStep(True, 0.01)  # clamp the step to 10 ms
        spk.System.useRealStep()
        # create Renderer
        self.NB_SEGMENTS = 32
        renderer = GLLineTrailRenderer.create()
        renderer.setBlending(spk.BLENDING_ADD)
        renderer.setDuration(2.0)
        renderer.setNbSamples(self.NB_SEGMENTS)
        renderer.enableRenderingHint(spk.DEPTH_TEST, False)
        self.renderer = renderer
        self.basicRenderer = GLPointRenderer.create(1.0)  # bare renderer for comparison(F4)
        # create Model
        massModel = spk.Model.create()
        massModel.setImmortal(True)
        particle_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA
        particle_mutable = spk.FLAG_ALPHA
        particle_random = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE
        particleModel = spk.Model.create(particle_enable, particle_mutable, particle_random)  # type: spk.Model
        particleModel.setParam(spk.PARAM_ALPHA, 0.1, 0.0)
        particleModel.setLifeTime(5.0, 8.0)
        self.particleModel = particleModel
        # create Emitter
        particleEmitter = spk.RandomEmitter.create()
        particleEmitter.setFlow(400)
        particleEmitter.setForce(0.0, 0.1)
        # create Modifier
        centerPointMass = spk.PointMass.create()
        centerPointMass.setMass(0.6)
        centerPointMass.setMinDistance(0.01)
        # create Group
        self.NB_POINT_MASS = 2
        massGroup = spk.Group.create(massModel, self.NB_POINT_MASS)
        massGroup.addModifier(centerPointMass)
        massGroup.setRenderer(None)
        self.massGroup = massGroup
        particleGroup = spk.Group.create(particleModel, 4100)  # type: spk.Group
        particleGroup.addEmitter(particleEmitter)
        particleGroup.setRenderer(renderer)
        self.particleGroup = particleGroup
        # create the point masses that will atract the particles
        self.pointMasses = []
        for i in range(self.NB_POINT_MASS):
            pointmass = spk.PointMass.create()
            pointmass.setMass(3.0)
            pointmass.setMinDistance(0.05)
            particleGroup.addModifier(pointmass)
            self.pointMasses.append(pointmass)
        # create System
        system = spk.System.create()  # type: spk.System
        system.addGroup(particleGroup)
        system.addGroup(massGroup)
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
        self.angleX = 45.0
        self.angleY = 0.0
        self.camPosZ = 10.0
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
            self.system.empty()
        # text switching with F1
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F1:
            self.text = (self.text-1) % 3
        # on/off Bounding Box with F2
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F2:
            self.particleGroup.enableAABBComputing(not self.particleGroup.isAABBComputingEnabled())
            self.massGroup.enableAABBComputing(not self.massGroup.isAABBComputingEnabled())
            if self.paused:
                self.system.computeAABB()
        # renderer switching with F4
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F4:
            renderers = [self.renderer, self.basicRenderer, None]
            self.renderValue = (self.renderValue+1) % 3
            self.particleGroup.setRenderer(renderers[self.renderValue])
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
        # Camera rotation with mouse
        if event.type == sdl2.SDL_MOUSEMOTION:
            sensitive = 0.4
            self.angleY += event.motion.xrel * sensitive
            self.angleX += event.motion.yrel * sensitive
        # Camera zooming with mouse wheel
        if event.type == sdl2.SDL_MOUSEWHEEL:
            if event.wheel.y < 0:
                self.camPosZ = min(18.0, self.camPosZ + 0.5)
            elif event.wheel.y > 0:
                self.camPosZ = max(0.5, self.camPosZ - 0.5)

    def onLoop(self):
        # get elapsed time
        t = sdl2.SDL_GetTicks()
        self.deltaTime = t - self.frames[-1]

        if not self.paused:
            # Changes the color of the model over time
            self.step += self.deltaTime * 0.0002
            color = spk.Vector3D(0.6 + 0.4 * math.sin(self.step),
                                 0.6 + 0.4 * math.sin(self.step + math.pi * 2.0 / 3.0),
                                 0.6 + 0.4 * math.sin(self.step + math.pi * 4.0 / 3.0))
            self.particleModel.setParam(spk.PARAM_RED, max(0.0, color.x - 0.25), min(1.0, color.x + 0.25))
            self.particleModel.setParam(spk.PARAM_GREEN, max(0.0, color.y - 0.25), min(1.0, color.y + 0.25))
            self.particleModel.setParam(spk.PARAM_BLUE, max(0.0, color.z - 0.25), min(1.0, color.z + 0.25))

            # If the number of particles in the mass group is 0 (at init or when the system is reinitialized),
            # a number of particles equal to NB_POINT_MASS is added
            if self.massGroup.getNbParticles() == 0:
                for i in range(self.NB_POINT_MASS):
                    pos = spk.Vector3D(2.0, 0.0, 0.0)
                    spd = spk.Vector3D(spk.random(-1.0, 0.0), spk.random(-0.5, 0.5), spk.random(-0.5, 0.5))
                    self.massGroup.addParticles(1, position=pos, velocity=spd)
                self.massGroup.flushAddedParticles()

            # Updates particle system
            self.system.update(self.deltaTime * 0.001)  # 1.0 defined as a second

            # the point masses are attached one by one to the particles in the massGroup so that they follow their moves
            for i in range(self.NB_POINT_MASS):
                self.pointMasses[i].setPosition(self.massGroup.getParticle(i).position())

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
        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)

        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        glu.gluPerspective(45, self.width / self.height, 0.01, 80.0)

        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()

        gl.glTranslated(0.0, 0.0, -self.camPosZ)  # apply camera zooming
        gl.glRotated(self.angleX, 1.0, 0.0, 0.0)  # apply camera rotation
        gl.glRotated(self.angleY, 0.0, 1.0, 0.0)

        # draw bounding box(F2 key)
        gl.glDisable(gl.GL_BLEND)
        drawBoundingBox(self.particleGroup, 1.0, 0.0, 0.0)
        drawBoundingBox(self.massGroup, 0.0, 1.0, 0.0)

        # draw particles
        self.system.render()

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

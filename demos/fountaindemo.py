#!/usr/bin/env python

# SPARK(pyspk) Fountain Demo script
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
            b"SPARK(&pyspk) Fountain Demo",
            sdl2.SDL_WINDOWPOS_UNDEFINED,
            sdl2.SDL_WINDOWPOS_UNDEFINED,
            width, height,
            sdl2.SDL_WINDOW_SHOWN | sdl2.SDL_WINDOW_OPENGL)
        sdl2.SDL_CaptureMouse(True)

        # init OpenGL
        self.context = sdl2.SDL_GL_CreateContext(self.window)
        sdl2.SDL_GL_SetSwapInterval(0)
        gl.glClearColor(0.0, 0.68, 0.85, 1.0)
        surface = sdl2.SDL_GetWindowSurface(self.window)
        gl.glViewport(0, 0, surface.contents.w, surface.contents.h)
        gl.glBlendFunc(gl.GL_SRC_ALPHA, gl.GL_ONE_MINUS_SRC_ALPHA)
        # init fog
        gl.glEnable(gl.GL_FOG)
        gl.glFogi(gl.GL_FOG_MODE, gl.GL_EXP2)
        gl.glFogfv(gl.GL_FOG_COLOR, [0.0, 0.68, 0.85, 1.0])
        gl.glFogf(gl.GL_FOG_DENSITY, 0.04)
        gl.glHint(gl.GL_PERSPECTIVE_CORRECTION_HINT, gl.GL_NICEST)
        # load Textures
        self.txGrass = loadTexture(b'res/grass.bmp', gl.GL_RGB, gl.GL_REPEAT, mipmap=True)
        self.txFountain = loadTexture(b'res/tile.bmp', gl.GL_RGB, gl.GL_REPEAT, mipmap=True)
        self.txWater = loadTexture(b'res/water.bmp', gl.GL_RGB, gl.GL_REPEAT, mipmap=True)
        txSplash = loadTexture(b'res/waterdrops.bmp', gl.GL_ALPHA, gl.GL_CLAMP)
        # create display lists
        self.createDisplayLists()

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
        # create Renderer
        renderer = GLQuadRenderer.create(1.0)  # type: GLQuadRenderer
        renderer.setScale(0.06, 0.06)
        renderer.setTexturingMode(spk.TEXTURE_2D)
        renderer.setTexture(txSplash)
        renderer.setBlending(spk.BLENDING_ALPHA)
        renderer.enableRenderingHint(spk.DEPTH_WRITE, False)
        self.renderer = renderer
        self.basicRenderer = GLPointRenderer.create(1.0)  # bare renderer for comparison(F4)
        # create Model
        model_enable = spk.FLAG_ALPHA | spk.FLAG_SIZE | spk.FLAG_ANGLE
        model_mutable = spk.FLAG_ALPHA | spk.FLAG_SIZE | spk.FLAG_ANGLE
        model_random = spk.FLAG_SIZE | spk.FLAG_ANGLE
        model = spk.Model.create(model_enable, model_mutable, model_random)  # type: spk.Model
        model.setParam(spk.PARAM_ALPHA, 0.2, 0.0)
        model.setParam(spk.PARAM_SIZE, 1.0, 1.0, 2.0, 8.0)
        model.setParam(spk.PARAM_ANGLE, 0.0, 4.0 * math.pi, 0.0, 4.0 * math.pi)
        model.setLifeTime(1.6, 2.2)
        self.model = model
        # create Emitter
        self.emitterZones = []
        self.emitters = []
        # splash to just above(x1)
        self.emitterZones.append(spk.Point.create(spk.Vector3D(0.0, 0.1, 0.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(0.0, 1.0, 0.0)))
        # splash from center of fountain(x4)
        self.emitterZones.append(spk.Point.create(spk.Vector3D(0.0, 0.1, 0.0)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(0.0, 0.1, 0.0)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(0.0, 0.1, 0.0)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(0.0, 0.1, 0.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(1.0, 3.0, 1.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(-1.0, 3.0, -1.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(-1.0, 3.0, 1.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(1.0, 3.0, -1.0)))
        # splash from the side of fountain(x8)
        self.emitterZones.append(spk.Point.create(spk.Vector3D(-1.6, 0.1, -1.6)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(1.6, 0.1, 1.6)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(1.6, 0.1, -1.6)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(-1.6, 0.1, 1.6)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(-2.26, 0.1, 0.0)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(2.26, 0.1, 0.0)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(0.0, 0.1, -2.26)))
        self.emitterZones.append(spk.Point.create(spk.Vector3D(0.0, 0.1, 2.26)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(1.0, 2.0, 1.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(-1.0, 2.0, -1.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(-1.0, 2.0, 1.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(1.0, 2.0, -1.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(1.41, 2.0, 0.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(-1.41, 2.0, 0.0)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(0.0, 2.0, 1.41)))
        self.emitters.append(spk.StraightEmitter.create(spk.Vector3D(0.0, 2.0, -1.41)))
        self.flow = [500.0,
                     600.0, 600.0, 600.0, 600.0,
                     900.0, 900.0, 900.0, 900.0, 900.0, 900.0, 900.0, 900.0]
        self.flowLow = [150.0,
                        200.0, 200.0, 200.0, 200.0,
                        250.0, 250.0, 250.0, 250.0, 250.0, 250.0, 250.0, 250.0]
        for i in range(len(self.emitters)):
            self.emitters[i].setZone(self.emitterZones[i])
            self.emitters[i].setFlow(self.flow[i])
            self.emitters[i].setForce(2.5, 4.0)
        self.emitters[0].setForce(3.0, 3.5)
        # no Modifier
        # create Group
        group = spk.Group.create(model, 20000)  # type: spk.Group
        for emitter in self.emitters:
            group.addEmitter(emitter)
        group.setRenderer(renderer)
        group.setCustomUpdate(self.splash)
        group.setGravity(spk.Vector3D(0.0, -2.2, 0.0))
        group.setFriction(0.7)
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
        self.totalTime = 0
        self.step = 0
        self.text = 2
        self.renderValue = 0
        self.renderEnv = True
        self.highGraphics = True
        self.angleY = 25.0
        self.angleX = 25.0
        self.camPosZ = 8.0
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
            self.group.enableAABBComputing(not self.group.isAABBComputingEnabled())
            if self.paused:
                self.system.computeAABB()
        # renderer switching with F4
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F4:
            renderers = [self.renderer, self.basicRenderer, None]
            self.renderValue = (self.renderValue+1) % 3
            self.group.setRenderer(renderers[self.renderValue])
        # on/off fog with F5
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F5:
            self.renderEnv = not self.renderEnv
            if self.renderEnv:
                gl.glClearColor(0.0, 0.68, 0.85, 1.0)
                gl.glEnable(gl.GL_FOG)
            else:
                gl.glClearColor(0.0, 0.0, 0.0, 0.0)
                gl.glDisable(gl.GL_FOG)
        # PAUSE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_PAUSE:
            self.paused = not self.paused
        # switching graphics with SPACE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_SPACE:
            self.highGraphics = not self.highGraphics
            if self.highGraphics:
                for i in range(len(self.emitters)):
                    self.emitters[i].setFlow(self.flow[i])
                self.model.setParam(spk.PARAM_ALPHA, 0.2, 0.0)
                self.model.setParam(spk.PARAM_SIZE, 1.0, 1.0, 2.0, 8.0)
            else:
                for i in range(len(self.emitters)):
                    self.emitters[i].setFlow(self.flowLow[i])
                self.model.setParam(spk.PARAM_ALPHA, 0.4, 0.0)
                self.model.setParam(spk.PARAM_SIZE, 1.5, 1.5, 2.0, 8.0)
        # Camera rotation with mouse
        if event.type == sdl2.SDL_MOUSEMOTION:
            sensitive = 0.4
            self.angleY += event.motion.xrel * sensitive
            self.angleX += event.motion.yrel * sensitive
            self.angleX = min(179.0, max(1.0, self.angleX))
        # Camera zooming with mouse wheel
        if event.type == sdl2.SDL_MOUSEWHEEL:
            if event.wheel.y < 0:
                self.camPosZ = min(18.0, self.camPosZ + 0.5)
            elif event.wheel.y > 0:
                self.camPosZ = max(6.0, self.camPosZ - 0.5)

    def onLoop(self):
        # get elapsed time
        t = sdl2.SDL_GetTicks()
        self.deltaTime = t - self.frames[-1]

        if not self.paused:
            # update emitters
            angle = self.totalTime * 2.0 * math.pi / 15000.0
            sinA = math.sin(angle)
            cosA = math.cos(angle)
            sinB = math.sin(angle * 10.0)

            for i in range(5):
                self.emitters[i].setForce(1.8 + 1.8 * sinB, 2.1 + 2.1 * sinB)
                if self.highGraphics:
                    self.emitters[i].setFlow((self.flow[i] * 0.5) + (self.flow[i] * 0.5) * sinB)
                else:
                    self.emitters[i].setFlow((self.flowLow[i] * 0.5) + (self.flowLow[i] * 0.5) * sinB)

            self.emitters[1].setDirection(spk.Vector3D(cosA - sinA, 3.0, sinA + cosA))
            self.emitters[2].setDirection(spk.Vector3D(-cosA + sinA, 3.0, -sinA - cosA))
            self.emitters[3].setDirection(spk.Vector3D(-cosA - sinA, 3.0, -sinA + cosA))
            self.emitters[4].setDirection(spk.Vector3D(cosA + sinA, 3.0, sinA - cosA))

            # update time of particle system
            self.system.update(self.deltaTime * 0.001)  # normalize 1.0 as 1[sec]

            self.totalTime += self.deltaTime

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
            print(self.fps, ',', self.nbParticles, ',', self.getGraphicsString())
            self.lasttime = t

    def drawText(self, text, x, y):
        gl.glPushMatrix()
        gl.glTranslated(x, y, 0.0)
        self.font.Render(text)
        gl.glPopMatrix()

    def getGraphicsString(self):
        if self.highGraphics:
            return "GRAPHICS: HIGH"
        else:
            return "GRAPHICS: LOW"

    def onRender(self):
        gl.glDepthMask(gl.GL_TRUE)
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)

        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        glu.gluPerspective(45, self.width / self.height, 0.01, 80.0)

        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()

        gl.glTranslated(0.0, 0.0, -self.camPosZ)  # apply camera zooming
        gl.glRotated(self.angleX, 1.0, 0.0, 0.0)  # apply camera rotation
        gl.glRotated(self.angleY, 0.0, 1.0, 0.0)

        # draw fountain
        if self.renderEnv:
            gl.glTexEnvi(gl.GL_TEXTURE_ENV, gl.GL_TEXTURE_ENV_MODE, gl.GL_MODULATE)
            gl.glCallList(self.DISPLAY_LIST_OBJECT)

            gl.glMatrixMode(gl.GL_TEXTURE)

            # scroll water texture
            angle = self.totalTime * 2.0 * math.pi / 12000.0
            sinA = math.sin(angle) * 0.2
            cosA = math.cos(angle) * 0.2

            gl.glRotatef(angle, 0.0, 0.0, 1.0)
            gl.glTranslatef(sinA, cosA, 1.0)
            gl.glScalef(1.0 + sinA * 0.2, 1.0 + cosA * 0.2, 1.0)

        # draw bounding box(F2 key)
        drawBoundingBox(self.group, 1.0, 0.0, 0.0)

        # draw fountain water
        if self.renderEnv:
            gl.glCallList(self.DISPLAY_LIST_WATER)
            gl.glLoadIdentity()

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

    @staticmethod
    def splash(particle, deltatime):
        if particle.position().y < 0.1:
            if particle.velocity().y > -0.5:
                return True  # kill

            # scatter
            pos = particle.position()
            pos.y = 0.1
            pos.x += spk.random(0.0, 0.2) - 0.1
            pos.z += spk.random(0.0, 0.2) - 0.1

            # reverse up/down
            spd = particle.velocity()
            spd.x = 0
            spd.y = -spk.random(0.1, 0.4) * spd.y
            spd.z = 0

            particle.setParamCurrentValue(spk.PARAM_ALPHA, 0.4)
            particle.setParamCurrentValue(spk.PARAM_SIZE, 0.0)
            particle.setLifeLeft(0.5)
        return False  # alive

    def createDisplayLists(self):
        self.DISPLAY_LIST_OBJECT = 1
        gl.glNewList(self.DISPLAY_LIST_OBJECT, gl.GL_COMPILE)

        gl.glDisable(gl.GL_CULL_FACE)
        gl.glDisable(gl.GL_BLEND)
        gl.glEnable(gl.GL_TEXTURE_2D)
        gl.glDisable(gl.GL_DEPTH_TEST)

        # grass
        gl.glBindTexture(gl.GL_TEXTURE_2D, self.txGrass)
        gl.glBegin(gl.GL_QUADS)
        gl.glColor3f(0.21, 0.64, 0.12)
        gl.glTexCoord2f(20.0, 20.0)
        gl.glVertex3f(40.0, 0.0, 40.0)
        gl.glTexCoord2f(20.0, -20.0)
        gl.glVertex3f(40.0, 0.0, -40.0)
        gl.glTexCoord2f(-20.0, -20.0)
        gl.glVertex3f(-40.0, 0.0, -40.0)
        gl.glTexCoord2f(-20.0, 20.0)
        gl.glVertex3f(-40.0, 0.0, 40.0)
        gl.glEnd()

        # bottom
        gl.glBindTexture(gl.GL_TEXTURE_2D, self.txFountain)
        gl.glBegin(gl.GL_TRIANGLE_FAN)
        gl.glColor4f(0.75, 0.75, 0.75, 1.0)
        gl.glTexCoord2f(0.0, 0.0)
        gl.glVertex3f(0.0, 0.0, 0.0)
        gl.glTexCoord2f(5.0, 0.0)
        gl.glVertex3f(2.5, 0.0, 0.0)
        gl.glTexCoord2f(3.54, -3.54)
        gl.glVertex3f(1.77, 0.0, -1.77)
        gl.glTexCoord2f(0.0, -5.0)
        gl.glVertex3f(0.0, 0.0, -2.5)
        gl.glTexCoord2f(-3.54, -3.54)
        gl.glVertex3f(-1.77, 0.0, -1.77)
        gl.glTexCoord2f(-5.0, 0.0)
        gl.glVertex3f(-2.5, 0.0, 0.0)
        gl.glTexCoord2f(-3.54, 3.54)
        gl.glVertex3f(-1.77, 0.0, 1.77)
        gl.glTexCoord2f(0.0, 5.0)
        gl.glVertex3f(0.0, 0.0, 2.5)
        gl.glTexCoord2f(3.54, 3.54)
        gl.glVertex3f(1.77, 0.0, 1.77)
        gl.glTexCoord2f(5.0, 0.0)
        gl.glVertex3f(2.5, 0.0, 0.0)
        gl.glEnd()

        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glEnable(gl.GL_CULL_FACE)

        # inner borders
        gl.glBegin(gl.GL_QUAD_STRIP)
        gl.glColor4f(0.5, 0.5, 0.5, 1.0)
        gl.glTexCoord2f(0.0, 0.0)
        gl.glVertex3f(2.5, 0.0, 0.0)
        gl.glTexCoord2f(0.0, 0.5)
        gl.glVertex3f(2.5, 0.20, 0.0)
        gl.glTexCoord2f(5.0, 0.0)
        gl.glVertex3f(1.77, 0.0, -1.77)
        gl.glTexCoord2f(5.0, 0.5)
        gl.glVertex3f(1.77, 0.20, -1.77)
        gl.glTexCoord2f(10.0, 0.0)
        gl.glVertex3f(0.0, 0.0, -2.5)
        gl.glTexCoord2f(10.0, 0.5)
        gl.glVertex3f(0.0, 0.20, -2.5)
        gl.glColor4f(0.85, 0.85, 0.85, 1.0)
        gl.glTexCoord2f(15.0, 0.0)
        gl.glVertex3f(-1.77, 0.0, -1.77)
        gl.glTexCoord2f(15.0, 0.5)
        gl.glVertex3f(-1.77, 0.20, -1.77)
        gl.glColor4f(1.0, 1.0, 1.0, 1.0)
        gl.glTexCoord2f(20.0, 0.00)
        gl.glVertex3f(-2.5, 0.0, 0.0)
        gl.glTexCoord2f(20.0, 0.5)
        gl.glVertex3f(-2.5, 0.20, 0.0)
        gl.glColor4f(0.85, 0.85, 0.85, 1.0)
        gl.glTexCoord2f(25.0, 0.0)
        gl.glVertex3f(-1.77, 0.0, 1.77)
        gl.glTexCoord2f(25.0, 0.5)
        gl.glVertex3f(-1.77, 0.20, 1.77)
        gl.glColor4f(0.5, 0.5, 0.5, 1.0)
        gl.glTexCoord2f(30.0, 0.0)
        gl.glVertex3f(0.0, 0.0, 2.5)
        gl.glTexCoord2f(30.0, 0.5)
        gl.glVertex3f(0.0, 0.20, 2.5)
        gl.glTexCoord2f(35.0, 0.0)
        gl.glVertex3f(1.77, 0.0, 1.77)
        gl.glTexCoord2f(35.0, 0.5)
        gl.glVertex3f(1.77, 0.20, 1.77)
        gl.glTexCoord2f(40.0, 0.0)
        gl.glVertex3f(2.5, 0.0, 0.0)
        gl.glTexCoord2f(40.0, 0.5)
        gl.glVertex3f(2.5, 0.20, 0.0)
        gl.glEnd()

        # outter borders
        gl.glBegin(gl.GL_QUAD_STRIP)
        gl.glColor4f(1.0, 1.0, 1.0, 1.0)
        gl.glTexCoord4f(0.0, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(2.6, 0.20, 0.0)
        gl.glTexCoord4f(0.0, 0.0, 0.0, 2.07)
        gl.glVertex3f(2.7, 0.0, 0.0)
        gl.glColor4f(0.85, 0.85, 0.85, 1.0)
        gl.glTexCoord4f(5.0 * 1.99, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(1.84, 0.20, -1.84)
        gl.glTexCoord4f(5.0 * 2.07, 0.0, 0.0, 2.07)
        gl.glVertex3f(1.91, 0.0, -1.91)
        gl.glColor4f(0.5, 0.5, 0.5, 1.0)
        gl.glTexCoord4f(10.0 * 1.99, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(0.0, 0.20, -2.6)
        gl.glTexCoord4f(10.0 * 2.07, 0.0, 0.0, 2.07)
        gl.glVertex3f(0.0, 0.0, -2.7)
        gl.glTexCoord4f(15.0 * 1.99, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(-1.84, 0.20, -1.84)
        gl.glTexCoord4f(15.0 * 2.07, 0.0, 0.0, 2.07)
        gl.glVertex3f(-1.91, 0.0, -1.91)
        gl.glTexCoord4f(20.0 * 1.99, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(-2.6, 0.20, 0.0)
        gl.glTexCoord4f(20.0 * 2.07, 0.0, 0.0, 2.07)
        gl.glVertex3f(-2.7, 0.0, 0.0)
        gl.glTexCoord4f(25.0 * 1.99, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(-1.84, 0.20, 1.84)
        gl.glTexCoord4f(25.0 * 2.07, 0.0, 0.0, 2.07)
        gl.glVertex3f(-1.91, 0.0, 1.91)
        gl.glTexCoord4f(30.0 * 1.99, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(0.0, 0.20, 2.6)
        gl.glTexCoord4f(30.0 * 2.07, 0.0, 0.0, 2.07)
        gl.glVertex3f(0.0, 0.0, 2.7)
        gl.glColor4f(0.85, 0.85, 0.85, 1.0)
        gl.glTexCoord4f(35.0 * 1.99, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(1.84, 0.20, 1.84)
        gl.glTexCoord4f(35.0 * 2.07, 0.0, 0.0, 2.07)
        gl.glVertex3f(1.91, 0.0, 1.91)
        gl.glColor4f(1.0, 1.0, 1.0, 1.0)
        gl.glTexCoord4f(40.0 * 1.99, 0.5 * 1.99, 0.0, 1.99)
        gl.glVertex3f(2.6, 0.20, 0.0)
        gl.glTexCoord4f(40.0 * 2.07, 0.0, 0.0, 2.07)
        gl.glVertex3f(2.7, 0.0, 0.0)
        gl.glEnd()

        gl.glDisable(gl.GL_CULL_FACE)

        # top borders
        gl.glBegin(gl.GL_QUAD_STRIP)
        gl.glColor4f(0.75, 0.75, 0.75, 1.0)
        gl.glTexCoord4f(0.0, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(2.5, 0.20, 0.0)
        gl.glTexCoord4f(0.0, 0.0, 0.0, 1.99)
        gl.glVertex3f(2.6, 0.20, 0.0)
        gl.glTexCoord4f(5.0 * 1.91, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(1.77, 0.20, -1.77)
        gl.glTexCoord4f(5.0 * 1.99, 0.0, 0.0, 1.99)
        gl.glVertex3f(1.84, 0.20, -1.84)
        gl.glTexCoord4f(10.0 * 1.91, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(0.0, 0.20, -2.5)
        gl.glTexCoord4f(10.0 * 1.99, 0.0, 0.0, 1.99)
        gl.glVertex3f(0.0, 0.20, -2.6)
        gl.glTexCoord4f(15.0 * 1.91, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(-1.77, 0.20, -1.77)
        gl.glTexCoord4f(15.0 * 1.99, 0.0, 0.0, 1.99)
        gl.glVertex3f(-1.84, 0.20, -1.84)
        gl.glTexCoord4f(20.0 * 1.91, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(-2.5, 0.20, 0.0)
        gl.glTexCoord4f(20.0 * 1.99, 0.0, 0.0, 1.99)
        gl.glVertex3f(-2.6, 0.20, 0.0)
        gl.glTexCoord4f(25.0 * 1.91, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(-1.77, 0.20, 1.77)
        gl.glTexCoord4f(25.0 * 1.99, 0.0, 0.0, 1.99)
        gl.glVertex3f(-1.84, 0.20, 1.84)
        gl.glTexCoord4f(30.0 * 1.91, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(0.0, 0.20, 2.5)
        gl.glTexCoord4f(30.0 * 1.99, 0.0, 0.0, 1.99)
        gl.glVertex3f(0.0, 0.20, 2.6)
        gl.glTexCoord4f(35.0 * 1.91, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(1.77, 0.20, 1.77)
        gl.glTexCoord4f(35.0 * 1.99, 0.0, 0.0, 1.99)
        gl.glVertex3f(1.84, 0.20, 1.84)
        gl.glTexCoord4f(40.0 * 1.91, 0.25 * 1.91, 0.0, 1.91)
        gl.glVertex3f(2.5, 0.20, 0.0)
        gl.glTexCoord4f(40.0 * 1.99, 0.0, 0.0, 1.99)
        gl.glVertex3f(2.6, 0.20, 0.0)
        gl.glEnd()

        gl.glEndList()

        self.DISPLAY_LIST_WATER = 2
        gl.glNewList(self.DISPLAY_LIST_WATER, gl.GL_COMPILE)

        # water
        gl.glEnable(gl.GL_BLEND)
        gl.glEnable(gl.GL_TEXTURE_2D)

        gl.glBindTexture(gl.GL_TEXTURE_2D, self.txWater)
        gl.glBegin(gl.GL_TRIANGLE_FAN)
        gl.glColor4f(1.0, 1.0, 1.0, 0.3)
        gl.glTexCoord2f(0.0, 0.0)
        gl.glVertex3f(0.0, 0.1, 0.0)
        gl.glTexCoord2f(5.0, 0.0)
        gl.glVertex3f(2.5, 0.1, 0.0)
        gl.glTexCoord2f(3.54, -3.54)
        gl.glVertex3f(1.77, 0.1, -1.77)
        gl.glTexCoord2f(0.0, -5.0)
        gl.glVertex3f(0.0, 0.1, -2.5)
        gl.glTexCoord2f(-3.54, -3.54)
        gl.glVertex3f(-1.77, 0.1, -1.77)
        gl.glTexCoord2f(-5.0, 0.0)
        gl.glVertex3f(-2.5, 0.1, 0.0)
        gl.glTexCoord2f(-3.54, 3.54)
        gl.glVertex3f(-1.77, 0.1, 1.77)
        gl.glTexCoord2f(0.0, 5.0)
        gl.glVertex3f(0.0, 0.1, 2.5)
        gl.glTexCoord2f(3.54, 3.54)
        gl.glVertex3f(1.77, 0.1, 1.77)
        gl.glTexCoord2f(5.0, 0.0)
        gl.glVertex3f(2.5, 0.1, 0.0)
        gl.glEnd()

        gl.glEndList()


if __name__ == '__main__':
    if not Path('./res').exists():
        print('Runtime Error: resources are not found.')
        print('please place the resource directory(\'./res/\') to current directory.')
    else:
        w = 640
        h = 480
        App(width=w, height=h).run()

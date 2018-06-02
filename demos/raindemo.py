#!/usr/bin/env python

# SPARK(pyspk) Rain Demo script
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
import math
import time
# SDL
import sdl2
# pyopengl
import OpenGL.GL as gl
import OpenGL.GLU as glu
# SPARK(pyspk)
import pyspk as spk
from pyspk.GL import GLPointRenderer, GLQuadRenderer, GLLineRenderer
from democommon import loadTexture, drawBoundingBox

try:
    # FTGL
    import FTGL
    notext = False
except ImportError:
    notext = True


class App:

    def param_i(self, mi, ma):
        return int(mi + self.rainRatio * (ma - mi))

    def param_f(self, mi, ma):
        return mi + self.rainRatio * (ma - mi)

    def __init__(self, width, height):
        self.width = width
        self.height = height

        # init SDL
        sdl2.SDL_Init(sdl2.SDL_INIT_EVERYTHING)
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_DOUBLEBUFFER, 1)
        self.window = sdl2.SDL_CreateWindow(
            b"SPARK(&pyspk) Rain Demo",
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
        gl.glClearColor(0.8, 0.8, 0.8, 0.0)
        surface = sdl2.SDL_GetWindowSurface(self.window)
        gl.glViewport(0, 0, surface.contents.w, surface.contents.h)
        # gl.glDisable(gl.GL_DEPTH)
        gl.glDisable(gl.GL_DEPTH_TEST)
        # load Textures
        self.txPaving = loadTexture(b'res/paving.bmp', gl.GL_RGB, gl.GL_REPEAT, mipmap=True)
        txSplash = loadTexture(b'res/waterdrops.bmp', gl.GL_ALPHA, gl.GL_CLAMP)
        # init fog
        gl.glEnable(gl.GL_FOG)
        gl.glFogi(gl.GL_FOG_MODE, gl.GL_EXP2)

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
        self.sizeRatio = self.width / 1440
        self.basicRenderer = GLPointRenderer.create(1.0)  # bare renderer for comparison(F4)
        dropRenderer = GLPointRenderer.create()
        dropRenderer.setType(spk.POINT_CIRCLE)
        dropRenderer.setSize(2.0 * self.sizeRatio)
        dropRenderer.enableBlending(True)
        self.dropRenderer = dropRenderer
        rainRenderer = GLLineRenderer.create()
        rainRenderer.setLength(-0.1)
        rainRenderer.enableBlending(True)
        self.rainRenderer = rainRenderer
        splashRenderer = GLQuadRenderer.create()
        splashRenderer.setScale(0.05, 0.05)
        splashRenderer.setTexturingMode(spk.TEXTURE_2D)
        splashRenderer.setTexture(txSplash)
        splashRenderer.enableBlending(True)
        splashRenderer.enableRenderingHint(spk.DEPTH_WRITE, False)
        self.splashRenderer = splashRenderer
        # create Model
        rain_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | spk.FLAG_MASS
        rain_mutable = spk.FLAG_NONE
        rain_random = spk.FLAG_MASS
        rainModel = spk.Model.create(rain_enable, rain_mutable, rain_random)
        rainModel.setParam(spk.PARAM_ALPHA, 0.2)
        rainModel.setImmortal(True)
        self.rainModel = rainModel
        dropModel = spk.Model.create(rain_enable, rain_mutable, rain_random)  # same as rain
        dropModel.setParam(spk.PARAM_ALPHA, 0.6)
        self.dropModel = dropModel
        splash_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | spk.FLAG_SIZE | spk.FLAG_ANGLE
        splash_mutable = spk.FLAG_SIZE | spk.FLAG_ALPHA
        splash_random = spk.FLAG_SIZE | spk.FLAG_ANGLE
        splashModel = spk.Model.create(splash_enable, splash_mutable, splash_random)
        splashModel.setParam(spk.PARAM_ANGLE, 0.0, 2.0 * math.pi)
        splashModel.setParam(spk.PARAM_ALPHA, 1.0, 0.0)
        self.splashModel = splashModel
        # create Emitter
        rainZone = spk.Ring.create(spk.Vector3D(0.0, 5.0, 0.0))
        self.rainZone = rainZone
        rainEmitter = spk.SphericEmitter.create(spk.Vector3D(0.0, -1.0, 0.0), 0.0, 0.03 * math.pi)
        rainEmitter.setZone(rainZone)
        self.rainEmitter = rainEmitter
        self.dropEmitter = spk.SphericEmitter.create(spk.Vector3D(0.0, 1.0, 0.0), 0.0, 0.2 * math.pi)
        # create Group
        gravity = spk.Vector3D(0.0, -2.0, 0.0)
        rainGroup = spk.Group.create(rainModel, 8000)
        rainGroup.setCustomUpdate(lambda p, t: self.killRain(p, t))
        rainGroup.setRenderer(rainRenderer)
        rainGroup.addEmitter(rainEmitter)
        rainGroup.setFriction(0.7)
        rainGroup.setGravity(gravity)
        self.rainGroup = rainGroup
        dropGroup = spk.Group.create(dropModel, 16000)
        dropGroup.setRenderer(dropRenderer)
        dropGroup.setFriction(0.7)
        dropGroup.setGravity(gravity)
        self.dropGroup = dropGroup
        splashGroup = spk.Group.create(splashModel, 2400)
        splashGroup.setRenderer(splashRenderer)
        self.splashGroup = splashGroup
        # create System
        system = spk.System.create()
        system.addGroup(splashGroup)
        system.addGroup(dropGroup)
        system.addGroup(rainGroup)
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
        self.renderEnv = True
        self.angleY = 0.0
        self.angleX = 12.0
        self.posX = 0.0
        self.posZ = 0.0
        self.rainRatio = 0.5
        self.recompute = True
        self.nbParticles = ''
        self.fps = ''
        self.strRainRate = ''
        self.frames = [sdl2.SDL_GetTicks()-1]
        self.lasttime = self.frames[-1]

    def killRain(self, particle, deltatime):
        if particle.position().y <= 0.0:
            particle.position().set(particle.position().x, 0.01, particle.position().z)
            self.splashGroup.addParticles(1, position=particle.position(),
                                          velocity=spk.Vector3D())
            self.dropGroup.addParticles(self.param_i(2, 8), position=particle.position(),
                                        emitter=self.dropEmitter)
            return True
        return False

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
            rainRenderers = [self.rainRenderer, self.basicRenderer, None]
            dropRenderers = [self.dropRenderer, self.basicRenderer, None]
            splashRenderers = [self.splashRenderer, self.basicRenderer, None]
            self.renderValue = (self.renderValue+1) % 3
            self.rainGroup.setRenderer(rainRenderers[self.renderValue])
            self.dropGroup.setRenderer(dropRenderers[self.renderValue])
            self.splashGroup.setRenderer(splashRenderers[self.renderValue])
        # on/off fog with F5
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F5:
            self.renderEnv = not self.renderEnv
            if self.renderEnv:
                color = self.param_f(0.8, 0.2)
                gl.glClearColor(color, color, color, 1.0)
                gl.glEnable(gl.GL_FOG)
            else:
                gl.glClearColor(0.0, 0.0, 0.0, 1.0)
                gl.glDisable(gl.GL_FOG)
        # PAUSE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_PAUSE:
            self.paused = not self.paused
        # Camera rotation with mouse
        if event.type == sdl2.SDL_MOUSEMOTION:
            sensitive = 0.4
            self.angleY += event.motion.xrel * sensitive
            self.angleX += event.motion.yrel * sensitive
            self.angleX = min(90.0, max(-90.0, self.angleX))

        if event.type == sdl2.SDL_KEYDOWN:
            key = event.key.keysym.sym
            if key == sdl2.SDLK_PLUS or key == sdl2.SDLK_KP_PLUS:
                self.rainRatio += 0.01
                self.rainRatio = min(1.0, self.rainRatio)
                self.recompute = True
            elif key == sdl2.SDLK_MINUS or key == sdl2.SDLK_KP_MINUS:
                self.rainRatio -= 0.01
                self.rainRatio = max(0.0, self.rainRatio)
                self.recompute = True
            move = False
            moveRate = 0.01
            if key == sdl2.SDLK_UP or key == sdl2.SDLK_w:
                self.posX += moveRate * self.deltaTime * math.sin(self.angleY * math.pi / 180.0)
                self.posZ += -moveRate * self.deltaTime * math.cos(self.angleY * math.pi / 180.0)
                move = True
            if key == sdl2.SDLK_DOWN or key == sdl2.SDLK_s:
                self.posX += -moveRate * self.deltaTime * math.sin(self.angleY * math.pi / 180.0)
                self.posZ += moveRate * self.deltaTime * math.cos(self.angleY * math.pi / 180.0)
                move = True
            if key == sdl2.SDLK_LEFT or key == sdl2.SDLK_a:
                self.posX += -moveRate * self.deltaTime * math.sin(self.angleY * math.pi / 180.0)
                self.posZ += -moveRate * self.deltaTime * math.cos(self.angleY * math.pi / 180.0)
                move = True
            if key == sdl2.SDLK_RIGHT or key == sdl2.SDLK_d:
                self.posX += moveRate * self.deltaTime * math.sin(self.angleY * math.pi / 180.0)
                self.posZ += moveRate * self.deltaTime * math.cos(self.angleY * math.pi / 180.0)
                move = True
            if move:
                self.rainZone.setPosition(spk.Vector3D(self.posX, 5.0, self.posZ))

    def onLoop(self):
        # get elapsed time
        t = sdl2.SDL_GetTicks()
        self.deltaTime = t - self.frames[-1]

        if not self.paused:
            # if the ratio rate has changed, recompute the particule system parameters
            if self.recompute:
                self.strRainRate = 'RAINRATE: ' + str(int(self.rainRatio * 100.0)) + '%'
                if self.renderEnv:
                    color = self.param_f(0.8, 0.2)
                    gl.glClearColor(color, color, color, 1.0)

                self.rainModel.setParam(spk.PARAM_RED, self.param_f(1.0, 0.40))
                self.rainModel.setParam(spk.PARAM_GREEN, self.param_f(1.0, 0.40))
                self.rainModel.setParam(spk.PARAM_BLUE, self.param_f(1.0, 0.42))
                self.rainModel.setParam(spk.PARAM_MASS, self.param_f(0.4, 0.8), self.param_f(0.8, 1.6))

                self.dropModel.setParam(spk.PARAM_RED, self.param_f(1.0, 0.40))
                self.dropModel.setParam(spk.PARAM_GREEN, self.param_f(1.0, 0.40))
                self.dropModel.setParam(spk.PARAM_BLUE, self.param_f(1.0, 0.42))
                self.dropModel.setParam(spk.PARAM_MASS, self.param_f(0.4, 0.8), self.param_f(3.0, 4.0))
                self.dropModel.setLifeTime(self.param_f(0.05, 0.3), self.param_f(0.1, 0.5))

                self.splashModel.setParam(spk.PARAM_RED, self.param_f(1.0, 0.40))
                self.splashModel.setParam(spk.PARAM_GREEN, self.param_f(1.0, 0.40))
                self.splashModel.setParam(spk.PARAM_BLUE, self.param_f(1.0, 0.42))
                self.splashModel.setParam(spk.PARAM_SIZE, 0.0, 0.0, self.param_f(0.375, 2.25), self.param_f(0.75, 3.78))
                self.splashModel.setLifeTime(self.param_f(0.2, 0.3), self.param_f(0.4, 0.5))

                self.rainEmitter.setFlow(self.param_f(0.0, 4800.0))
                self.rainEmitter.setForce(self.param_f(3.0, 5.0), self.param_f(6.0, 10.0))
                self.rainZone.setRadius(0.0, self.param_f(20.0, 5.0))
                # rainZone.setDimension(Vector3D(self.param_f(40.0, 10.0), 0.0, self.param_f(40.0, 10.0)))

                self.dropEmitter.setForce(self.param_f(0.1, 1.0), self.param_f(0.2, 2.0))

                self.dropRenderer.setSize(self.param_f(1.0, 3.0) * self.sizeRatio)
                self.rainRenderer.setWidth(self.param_f(1.0, 4.0) * self.sizeRatio)
                self.recompute = False

            # Updates particle system
            self.system.update(self.deltaTime * 0.001)  # 1.0 defined as a second

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
            print(self.fps, ',', self.nbParticles, ',', self.strRainRate)
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
        glu.gluPerspective(45, self.width / self.height, 0.01, 20.0)

        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()

        gl.glRotated(self.angleX, 1.0, 0.0, 0.0)  # apply camera rotation
        gl.glRotated(self.angleY, 0.0, 1.0, 0.0)
        gl.glTranslated(-self.posX, -1.0, -self.posZ)

        if self.renderEnv:
            c = self.param_f(0.8, 0.2)
            gl.glFogfv(gl.GL_FOG_COLOR, [c, c, c, 1.0])
            dist = self.param_f(20.0, 5.0)
            gl.glFogf(gl.GL_FOG_DENSITY, 2.0 / dist)

            # Renders floor
            gl.glDisable(gl.GL_BLEND)
            gl.glEnable(gl.GL_TEXTURE_2D)
            gl.glTexEnvi(gl.GL_TEXTURE_ENV, gl.GL_TEXTURE_ENV_MODE, gl.GL_MODULATE)
            gl.glBindTexture(gl.GL_TEXTURE_2D, self.txPaving)
            gl.glBegin(gl.GL_QUADS)
            gl.glColor3f(self.param_f(1.0, 0.3), self.param_f(1.0, 0.3), self.param_f(1.0, 0.3))
            gl.glTexCoord2f(dist + self.posX, dist + self.posZ)
            gl.glVertex3f(dist + self.posX, 0.0, dist + self.posZ)
            gl.glTexCoord2f(dist + self.posX, -dist + self.posZ)
            gl.glVertex3f(dist + self.posX, 0.0, -dist + self.posZ)
            gl.glTexCoord2f(-dist + self.posX, -dist + self.posZ)
            gl.glVertex3f(-dist + self.posX, 0.0, -dist + self.posZ)
            gl.glTexCoord2f(-dist + self.posX, dist + self.posZ)
            gl.glVertex3f(-dist + self.posX, 0.0, dist + self.posZ)
            gl.glEnd()
            gl.glDisable(gl.GL_TEXTURE_2D)

        # draw bounding box(F2 key)
        gl.glLineWidth(1.0)
        drawBoundingBox(self.rainGroup, 1.0, 0.0, 0.0)
        drawBoundingBox(self.dropGroup, 0.0, 1.0, 0.0)
        drawBoundingBox(self.splashGroup, 0.0, 0.0, 1.0)

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
                self.drawText(self.strRainRate, 4.0, 72.0)
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

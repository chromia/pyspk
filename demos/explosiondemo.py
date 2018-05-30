#!/usr/bin/env python

# SPARK(pyspk) Explosion Demo script
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
from democommon import loadTexture


class ExplosionSystem:
    def __init__(self, system=None, screenheight=0):
        if system:
            # copy
            self.system = system
            return
        assert(screenheight != 0)

        #
        # Texture
        #

        # create textures for OpenGL
        txExplosion = loadTexture(b'res/explosion.bmp', gl.GL_ALPHA, gl.GL_CLAMP)
        txFlash = loadTexture(b'res/flash.bmp', gl.GL_RGB, gl.GL_CLAMP)
        txSpark1 = loadTexture(b'res/spark1.bmp', gl.GL_RGB, gl.GL_CLAMP)
        txSpark2 = loadTexture(b'res/point.bmp', gl.GL_ALPHA, gl.GL_CLAMP)
        txWave = loadTexture(b'res/wave.bmp', gl.GL_RGBA, gl.GL_CLAMP)

        #
        # Renderer
        #
        # common setting function
        def commonSettings(renderer, texture, txblending, ptblending):
            renderer.setTexturingMode(spk.TEXTURE_2D)
            renderer.setTexture(texture)
            renderer.setTextureBlending(txblending)
            renderer.setBlending(ptblending)
            renderer.enableRenderingHint(spk.DEPTH_WRITE, False)  # bacause I'll sort particles later
            renderer.setShared(True)  # means that this resource is reused between particle systems
        # create Smoke renderer
        smokeRenderer = GLQuadRenderer.create()  # type: spk.GL.GLQuadRenderer
        commonSettings(smokeRenderer, txExplosion, gl.GL_MODULATE, spk.BLENDING_ALPHA)
        smokeRenderer.setAtlasDimensions(2, 2)  # texture is divided by four(2x2)
        # create Flame renderer
        flameRenderer = GLQuadRenderer.create()
        commonSettings(flameRenderer, txExplosion, gl.GL_MODULATE, spk.BLENDING_ADD)
        flameRenderer.setAtlasDimensions(2, 2)
        # create Flash renderer
        flashRenderer = GLQuadRenderer.create()
        commonSettings(flashRenderer, txFlash, gl.GL_REPLACE, spk.BLENDING_ADD)
        # create Spark(line) renderer
        spark1Renderer = GLQuadRenderer.create()
        commonSettings(spark1Renderer, txSpark1, gl.GL_REPLACE, spk.BLENDING_ADD)
        spark1Renderer.setOrientation(spk.DIRECTION_ALIGNED)  # consideration about direction
        spark1Renderer.setScale(0.05, 1.0)  # narrow rectangle
        # create Spark(point) renderer
        GLPointRenderer.setPixelPerUnit(45.0 * math.pi / 180.0, screenheight)
        spark2Renderer = GLPointRenderer.create()  # type: spk.GL.GLPointRenderer
        spark2Renderer.setType(spk.POINT_SPRITE)
        spark2Renderer.setTexture(txSpark2)
        spark2Renderer.setTextureBlending(gl.GL_MODULATE)
        spark2Renderer.setBlending(spk.BLENDING_ADD)
        spark2Renderer.enableRenderingHint(spk.DEPTH_WRITE, False)
        spark2Renderer.enableWorldSize(True)
        spark2Renderer.setSize(0.02)
        spark2Renderer.setShared(True)
        # create Wave renderer
        waveRenderer = GLQuadRenderer.create()  # type: spk.GL.GLQuadRenderer
        commonSettings(waveRenderer, txWave, gl.GL_MODULATE, spk.BLENDING_ALPHA)
        # enable alpha-test to cut center area of texture image
        waveRenderer.enableRenderingHint(spk.ALPHA_TEST, True)
        waveRenderer.setAlphaTestThreshold(0.0)
        waveRenderer.setOrientation(spk.FIXED_ORIENTATION)  # fix direction
        waveRenderer.lookVector.set(0.0, 1.0, 0.0)  # emit along Y axis
        waveRenderer.upVector.set(1.0, 0.0, 0.0)

        #
        # Model
        #
        # create Smoke model
        smoke_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | \
            spk.FLAG_SIZE | spk.FLAG_ANGLE | spk.FLAG_TEXTURE_INDEX  # parameters to use
        smoke_mutable = spk.FLAG_SIZE | spk.FLAG_ANGLE  # paramters to be changed
        smoke_random = spk.FLAG_SIZE | spk.FLAG_ANGLE | spk.FLAG_TEXTURE_INDEX  # paramters to be changed randomly
        smoke_interpolated = spk.FLAG_ALPHA  # paramters applying interpolation
        smokeModel = spk.Model.create(smoke_enable, smoke_mutable, smoke_random, smoke_interpolated)  # type: spk.Model
        smokeModel.setParam(spk.PARAM_RED, 0.2)  # set color(RGB)
        smokeModel.setParam(spk.PARAM_GREEN, 0.2)
        smokeModel.setParam(spk.PARAM_BLUE, 0.2)
        smokeModel.setParam(spk.PARAM_SIZE, 0.6, 0.8, 1.0, 1.4)  # [min,max]on birth, [min,max]on death
        smokeModel.setParam(spk.PARAM_TEXTURE_INDEX, 0.0, 4.0)  # select a texture from divided by four
        smokeModel.setParam(spk.PARAM_ANGLE, 0.0, math.pi * 0.5, 0.0, math.pi * 0.5)  # azimuth angle
        smokeModel.setLifeTime(2.5, 3.0)
        smokeModel.setShared(True)
        # interpolator is a utility to make parameter curve
        # if you provide (x1, y1),(x2, y2) ... (xn, yn) paramters (n>=2),
        # interpolator fills the intermediate.
        # x is the timeline. By default(spk.INTERPOLATOR_LIFETIME), domain of x is [0,1].
        ip_alpha = smokeModel.getInterpolator(spk.PARAM_ALPHA)  # type: spk.Interpolator
        ip_alpha.addEntry(0.0, 0.0)  # (x1,y1)
        ip_alpha.addEntry(0.4, 0.4, 0.6)  # (x2, y2min, y2max) y is randomly selected from [y2min,y2max]
        ip_alpha.addEntry(0.6, 0.4, 0.6)  # (x3, y3min, y3max)
        ip_alpha.addEntry(1.0, 0.0)  # (x4, y4)
        # create Flame model
        flame_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | \
            spk.FLAG_SIZE | spk.FLAG_ANGLE | spk.FLAG_TEXTURE_INDEX
        flame_mutable = spk.FLAG_ANGLE | spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE
        flame_random = spk.FLAG_ANGLE | spk.FLAG_TEXTURE_INDEX
        flame_interpolated = spk.FLAG_SIZE | spk.FLAG_ALPHA
        flameModel = spk.Model.create(flame_enable, flame_mutable, flame_random, flame_interpolated)
        flameModel.setParam(spk.PARAM_RED, 1.0, 0.2)
        flameModel.setParam(spk.PARAM_GREEN, 0.5, 0.2)
        flameModel.setParam(spk.PARAM_BLUE, 0.2, 0.2)
        flameModel.setParam(spk.PARAM_TEXTURE_INDEX, 0.0, 4.0)
        flameModel.setParam(spk.PARAM_ANGLE, 0.0, math.pi * 0.5, 0.0, math.pi * 0.5)
        flameModel.setLifeTime(1.5, 2.0)
        flameModel.setShared(True)
        ip_size = flameModel.getInterpolator(spk.PARAM_SIZE)
        ip_size.addEntry(0.0, 0.25)
        ip_size.addEntry(0.02, 0.6, 0.8)
        ip_size.addEntry(1.0, 1.0, 1.4)
        ip_alpha = flameModel.getInterpolator(spk.PARAM_ALPHA)
        ip_alpha.addEntry(0.5, 1.0)
        ip_alpha.addEntry(1.0, 0.0)
        # create Flash model
        flash_enable = spk.FLAG_ALPHA | spk.FLAG_SIZE | spk.FLAG_ANGLE
        flash_mutable = spk.FLAG_NONE
        flash_random = spk.FLAG_ANGLE
        flash_interpolated = spk.FLAG_ALPHA | spk.FLAG_SIZE
        flashModel = spk.Model.create(flash_enable, flash_mutable, flash_random, flash_interpolated)
        flashModel.setParam(spk.PARAM_ANGLE, 0.0, 2.0 * math.pi)
        flashModel.setLifeTime(0.5, 0.5)
        flashModel.setShared(True)
        ip_size = flashModel.getInterpolator(spk.PARAM_SIZE)
        ip_size.addEntry(0.0, 0.25)
        ip_size.addEntry(0.1, 1.0, 2.0)
        ip_alpha = flashModel.getInterpolator(spk.PARAM_ALPHA)
        ip_alpha.addEntry(0.0, 1.0)
        ip_alpha.addEntry(0.4, 0.0)
        # create Spark(line) model
        spark1Model = spk.Model.create(spk.FLAG_SIZE | spk.FLAG_ALPHA, spk.FLAG_ALPHA, spk.FLAG_SIZE)
        spark1Model.setParam(spk.PARAM_ALPHA, 1.0, 0.0)
        spark1Model.setParam(spk.PARAM_SIZE, 0.2, 0.4)
        spark1Model.setLifeTime(0.2, 1.0)
        spark1Model.setShared(True)
        # create Spark(point) model
        spark2Model = spk.Model.create(spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA,
                                       spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA, spk.FLAG_GREEN)
        spark2Model.setParam(spk.PARAM_ALPHA, 1.0, 0.0)
        spark2Model.setParam(spk.PARAM_RED, 1.0)
        spark2Model.setParam(spk.PARAM_GREEN, 1.0, 1.0, 0.3, 1.0)
        spark2Model.setParam(spk.PARAM_BLUE, 0.7, 0.3)
        spark2Model.setLifeTime(1.0, 3.0)
        spark2Model.setShared(True)
        # create Wave model
        waveModel = spk.Model.create(spk.FLAG_ALPHA | spk.FLAG_SIZE, spk.FLAG_SIZE | spk.FLAG_ALPHA)
        waveModel.setParam(spk.PARAM_SIZE, 0.0, 4.0)
        waveModel.setParam(spk.PARAM_ALPHA, 0.2, 0.0)
        waveModel.setLifeTime(0.8, 0.8)
        waveModel.setShared(True)

        #
        # Emitter
        #
        zero = spk.Vector3D()  # center of explosion
        smokeSphere = spk.Sphere.create(zero, 0.6)  # the range of smoke particles
        explosionSphere = spk.Sphere.create(zero, 0.4)  # the range of flame,spark,wave particles
        flashSphere = spk.Sphere.create(zero, 0.1)  # the range of flash particles
        # create Smoke emitter
        smokeEmitter = spk.RandomEmitter.create()
        smokeEmitter.setZone(smokeSphere, False)
        smokeEmitter.setFlow(-1)  # infinite [particles/frame]
        smokeEmitter.setTank(15)  # upper limit of particle creation
        smokeEmitter.setForce(0.02, 0.04)
        # create Flame emitter
        flameEmitter = spk.NormalEmitter.create()
        flameEmitter.setZone(explosionSphere)
        flameEmitter.setFlow(-1)
        flameEmitter.setTank(15)
        flameEmitter.setForce(0.06, 0.1)
        # create Flas emitter
        flashEmitter = spk.StaticEmitter.create()
        flashEmitter.setZone(flashSphere)
        flashEmitter.setFlow(-1)
        flashEmitter.setTank(3)
        # create Spark(line) emitter
        spark1Emitter = spk.NormalEmitter.create()
        spark1Emitter.setZone(explosionSphere)
        spark1Emitter.setFlow(-1)
        spark1Emitter.setTank(20)
        spark1Emitter.setForce(2.0, 3.0)
        # create Spark(point) emitter
        spark2Emitter = spk.NormalEmitter.create()
        spark2Emitter.setZone(explosionSphere)
        spark2Emitter.setFlow(-1)
        spark2Emitter.setTank(400)
        spark2Emitter.setForce(0.4, 0.8)
        # create Wave emitter
        waveEmitter = spk.StaticEmitter.create()
        waveEmitter.setZone(spk.Point.create(zero))
        waveEmitter.setFlow(-1)
        waveEmitter.setTank(1)

        # no Modifier

        #
        # Group
        #
        # create Smoke group
        smokeGroup = spk.Group.create(smokeModel, 15)
        smokeGroup.addEmitter(smokeEmitter)
        smokeGroup.setRenderer(smokeRenderer)
        smokeGroup.setGravity(spk.Vector3D(0.0, 0.05, 0.0))
        # create Flame group
        flameGroup = spk.Group.create(flameModel, 15)
        flameGroup.addEmitter(flameEmitter)
        flameGroup.setRenderer(flameRenderer)
        # create Flash group
        flashGroup = spk.Group.create(flashModel, 3)
        flashGroup.addEmitter(flashEmitter)
        flashGroup.setRenderer(flashRenderer)
        # create Spark(line) group
        spark1Group = spk.Group.create(spark1Model, 20)
        spark1Group.addEmitter(spark1Emitter)
        spark1Group.setRenderer(spark1Renderer)
        spark1Group.setGravity(spk.Vector3D(0.0, -1.5, 0.0))
        # create Spark(point) group
        spark2Group = spk.Group.create(spark2Model, 400)
        spark2Group.addEmitter(spark2Emitter)
        spark2Group.setRenderer(spark2Renderer)
        spark2Group.setGravity(spk.Vector3D(0.0, -0.3, 0.0))
        spark2Group.setFriction(0.4)
        # create Wave group
        waveGroup = spk.Group.create(waveModel, 1)
        waveGroup.addEmitter(waveEmitter)
        waveGroup.setRenderer(waveRenderer)

        #
        # System
        #
        system = spk.System.create()
        system.addGroup(waveGroup)
        system.addGroup(smokeGroup)
        system.addGroup(flameGroup)
        system.addGroup(flashGroup)
        system.addGroup(spark1Group)
        system.addGroup(spark2Group)
        self.system = system

    def __del__(self):
        spk.destroySystem(self.system)

    def clone(self, pos):
        newsystem = spk.copySystem(self.system)  # type: spk.System
        newsystem.setTransformPosition(pos)
        newsystem.updateTransform()
        return ExplosionSystem(newsystem)


class App:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        # init SDL
        sdl2.SDL_Init(sdl2.SDL_INIT_EVERYTHING)
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_DOUBLEBUFFER, 1)
        self.window = sdl2.SDL_CreateWindow(
            b"SPARK(&pyspk) Explosion Demo",
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
        # create system template
        self.explosion = ExplosionSystem(screenheight=self.height)
        self.explosions = []

        print('SPARK FACTORY AFTER INIT :')
        spk.Factory.getInstance().traceAll()

        # init Variables
        self.running = True
        self.paused = False
        self.deltaTime = 0
        self.step = 0
        self.text = 2
        self.renderValue = 0
        self.angleX = 0.0
        self.angleY = 0.0
        self.camPosZ = 5.0
        self.spacePressed = 0.0
        self.nbParticles = ''
        self.fps = ''
        self.frames = [sdl2.SDL_GetTicks()-1]

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
        # kill all systems expressly
        self.explosions = []

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
        # PAUSE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_PAUSE:
            self.paused = not self.paused
        # new explosion has occured with SPACE or mouse click
        if (event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_SPACE) or \
           ((event.type == sdl2.SDL_MOUSEBUTTONDOWN) and
           (event.button.button == sdl2.SDL_BUTTON_LEFT or event.button.button == sdl2.SDL_BUTTON_RIGHT)):
            self.spacePressed = 1000.0
        if (event.type == sdl2.SDL_KEYUP and event.key.keysym.sym == sdl2.SDLK_SPACE) or \
           ((event.type == sdl2.SDL_MOUSEBUTTONUP) and
           (event.button.button == sdl2.SDL_BUTTON_LEFT or event.button.button == sdl2.SDL_BUTTON_RIGHT)):
            self.spacePressed = -1.0
        # Camera rotation with mouse
        if event.type == sdl2.SDL_MOUSEMOTION:
            sensitive = 0.4
            self.angleY += event.motion.xrel * sensitive
            self.angleX += event.motion.yrel * sensitive
            self.angleX = max(self.angleX, -90.0)
            self.angleX = min(self.angleX, 90.0)
        # Camera zooming with mouse wheel
        if event.type == sdl2.SDL_MOUSEWHEEL:
            if event.wheel.y < 0:
                self.camPosZ = min(10.0, self.camPosZ + 0.5)
            elif event.wheel.y > 0:
                self.camPosZ = max(0.5, self.camPosZ - 0.5)

    def onLoop(self):
        # get elapsed time
        t = sdl2.SDL_GetTicks()
        self.deltaTime = t - self.frames[-1]

        if not self.paused:
            # create new particle system with SPACE key
            if self.spacePressed >= 0.0:
                self.spacePressed += self.deltaTime
                if self.spacePressed >= 1000.0:
                    position = spk.Vector3D(spk.random(-2.0, 2.0), spk.random(-2.0, 2.0), spk.random(-2.0, 2.0))
                    self.explosions.append(self.explosion.clone(position))
                    self.spacePressed = 0.0

        # calclate camera position and distances with each explosion
        # so draw in the order: far -> near
        cosX = math.cos(self.angleX * math.pi / 180.0)
        sinX = math.sin(self.angleX * math.pi / 180.0)
        cosY = math.cos(self.angleY * math.pi / 180.0)
        sinY = math.sin(self.angleY * math.pi / 180.0)
        cz = self.camPosZ
        campos = spk.Vector3D(-cosX*sinY*cz, sinX*cz, cosX*cosY*cz)

        def compareDist(explosion):
            return -spk.getSqrDist(explosion.system.getWorldTransformPos(), campos)
        self.explosions.sort(key=compareDist)

        explosions = []
        for explosion in self.explosions:
            # update time of particle system
            alive = explosion.system.update(self.deltaTime * 0.001)
            if alive:
                explosions.append(explosion)
            else:
                pass  # remove dead systems from self.systems
        self.explosions = explosions

        # rendering
        self.onRender()

        # time logging
        self.frames.append(t)
        while self.frames[-1] - self.frames[0] > 1000 and len(self.frames) > 2:
            self.frames.pop(0)

        # update text
        nbParticles = sum([s.system.getNbParticles() for s in self.explosions])
        self.nbParticles = "NB PARTICLES : " + str(nbParticles)
        fps = ((len(self.frames)-1) * 1000.0) // (self.frames[-1] - self.frames[0])
        self.fps = "FPS : " + str(fps)

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
        gl.glRotated(self.angleX, 1.0, 0.0, 0.0)  # apply camera rotation
        gl.glRotated(self.angleY, 0.0, 1.0, 0.0)

        # draw particles
        for explosion in self.explosions:
            explosion.system.render()

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
                self.drawText(str(self.spacePressed), 4.0, 72.0)
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

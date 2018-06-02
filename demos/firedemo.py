#!/usr/bin/env python

# SPARK(pyspk) Fire Demo script
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
# PyWavefront
import pywavefront.visualization

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
            b"SPARK(&pyspk) Fire Demo",
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
        # load textures
        txFire = loadTexture(b'res/fire2.bmp', gl.GL_ALPHA, gl.GL_CLAMP, mipmap=True)
        txSmoke = loadTexture(b'res/explosion.bmp', gl.GL_ALPHA, gl.GL_CLAMP)
        # lighting
        light_ambient = [0.15, 0.15, 0.25, 1.0]
        light_diffuse = [1.0, 0.75, 0.25, 1.0]
        gl.glLightfv(gl.GL_LIGHT0, gl.GL_AMBIENT, light_ambient)
        gl.glLightfv(gl.GL_LIGHT0, gl.GL_DIFFUSE, light_diffuse)
        gl.glLightf(gl.GL_LIGHT0, gl.GL_QUADRATIC_ATTENUATION, 20.0)
        mat_ambient = [0.2, 0.2, 0.2, 1.0]
        mat_diffuse = [1.0, 1.0, 1.0, 1.0]
        gl.glMaterialfv(gl.GL_FRONT_AND_BACK, gl.GL_AMBIENT, mat_ambient)
        gl.glMaterialfv(gl.GL_FRONT_AND_BACK, gl.GL_DIFFUSE, mat_diffuse)
        gl.glLightModelfv(gl.GL_LIGHT_MODEL_AMBIENT, light_ambient)
        gl.glLightModelf(gl.GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0)

        # init FTGL
        if not notext:
            font = FTGL.TextureFont('res/font.ttf')
            font.FaceSize(24)
            self.font = font

        # init PyWavefront
        self.scene = pywavefront.Wavefront('./res/SceneFireCamp.obj')

        # init SPARK(pyspk)
        # random seed initialization
        spk.setRandomSeed(int(time.time()))
        # step configuration
        spk.System.setClampStep(True, 0.1)  # clamp the step to 100 ms
        spk.System.useAdaptiveStep(0.001, 0.01)  # use an adaptive step from 1ms to 10ms (1000fps to 100fps)

        # create Renderer
        fireRenderer = GLQuadRenderer.create()  # type: GLQuadRenderer
        fireRenderer.setScale(0.3, 0.3)
        fireRenderer.setTexturingMode(spk.TEXTURE_2D)
        fireRenderer.setTexture(txFire)
        fireRenderer.setTextureBlending(gl.GL_MODULATE)
        fireRenderer.setBlending(spk.BLENDING_ADD)
        fireRenderer.enableRenderingHint(spk.DEPTH_WRITE, False)
        fireRenderer.setAtlasDimensions(2, 2)
        self.fireRenderer = fireRenderer
        smokeRenderer = GLQuadRenderer.create()  # type: GLQuadRenderer
        smokeRenderer.setScale(0.3, 0.3)
        smokeRenderer.setTexturingMode(spk.TEXTURE_2D)
        smokeRenderer.setTexture(txSmoke)
        smokeRenderer.setTextureBlending(gl.GL_MODULATE)
        smokeRenderer.setBlending(spk.BLENDING_ALPHA)
        smokeRenderer.enableRenderingHint(spk.DEPTH_WRITE, False)
        smokeRenderer.setAtlasDimensions(2, 2)
        self.smokeRenderer = smokeRenderer
        self.basicRenderer = GLPointRenderer.create(1.0)  # bare renderer for comparison(F4)

        # create Model
        fire_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | \
            spk.FLAG_SIZE | spk.FLAG_ANGLE | spk.FLAG_TEXTURE_INDEX
        fire_mutable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_ALPHA | spk.FLAG_ANGLE
        fire_random = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_TEXTURE_INDEX | spk.FLAG_ANGLE
        fire_interpolated = spk.FLAG_SIZE
        fireModel = spk.Model.create(fire_enable, fire_mutable, fire_random, fire_interpolated)  # type: spk.Model
        fireModel.setParam(spk.PARAM_RED, 0.8, 0.9, 0.8, 0.9)  # [min,max]on birth, [min,max]on death
        fireModel.setParam(spk.PARAM_GREEN, 0.5, 0.6, 0.5, 0.6)
        fireModel.setParam(spk.PARAM_BLUE, 0.3)
        fireModel.setParam(spk.PARAM_ALPHA, 0.4, 0.0)  # on birth, on death
        minAngle, maxAngle = 0.0, 2.0 * math.pi
        fireModel.setParam(spk.PARAM_ANGLE, minAngle, maxAngle, minAngle, maxAngle)
        fireModel.setParam(spk.PARAM_TEXTURE_INDEX, 0.0, 4.0)
        fireModel.setLifeTime(1.0, 1.5)
        ip_size = fireModel.getInterpolator(spk.PARAM_SIZE)
        ip_size.addEntry(0.5, 2.0, 5.0)
        ip_size.addEntry(1.0, 0.0)
        self.fireModel = fireModel
        smoke_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE | spk.FLAG_ALPHA | \
            spk.FLAG_SIZE | spk.FLAG_ANGLE | spk.FLAG_TEXTURE_INDEX
        smoke_mutable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_SIZE | spk.FLAG_ANGLE
        smoke_random = spk.FLAG_TEXTURE_INDEX | spk.FLAG_ANGLE
        smoke_interpolated = spk.FLAG_ALPHA
        smokeModel = spk.Model.create(smoke_enable, smoke_mutable, smoke_random, smoke_interpolated)  # type: spk.Model
        smokeModel.setParam(spk.PARAM_RED, 0.3, 0.2)
        smokeModel.setParam(spk.PARAM_GREEN, 0.25, 0.2)
        smokeModel.setParam(spk.PARAM_BLUE, 0.2)
        smokeModel.setParam(spk.PARAM_ALPHA, 0.2, 0.0)
        smokeModel.setParam(spk.PARAM_SIZE, 5.0, 10.0)
        smokeModel.setParam(spk.PARAM_TEXTURE_INDEX, 0.0, 4.0)
        smokeModel.setParam(spk.PARAM_ANGLE, 0.0, 2.0 * math.pi, 0.0, 2.0 * math.pi)
        smokeModel.setLifeTime(5.0, 5.0)
        ip_alpha = smokeModel.getInterpolator(spk.PARAM_ALPHA)
        ip_alpha.addEntry(0.0, 0.0)
        ip_alpha.addEntry(0.2, 0.2)
        ip_alpha.addEntry(1.0, 0.0)
        self.smokeModel = smokeModel

        # create Emitter
        fireEmitter1 = spk.StraightEmitter.create(spk.Vector3D(0.0, 1.0, 0.0))
        fireEmitter1.setZone(spk.Sphere.create(spk.Vector3D(0.0, -1.0, 0.0), 0.5))
        fireEmitter1.setFlow(40)
        fireEmitter1.setForce(1.0, 2.5)
        fireEmitter2 = spk.StraightEmitter.create(spk.Vector3D(1.0, 0.6, 0.0))
        fireEmitter2.setZone(spk.Sphere.create(spk.Vector3D(0.15, -1.2, 0.075), 0.1))
        fireEmitter2.setFlow(15)
        fireEmitter2.setForce(0.5, 1.5)
        fireEmitter3 = spk.StraightEmitter.create(spk.Vector3D(-0.6, 0.8, -0.8))
        fireEmitter3.setZone(spk.Sphere.create(spk.Vector3D(-0.375, -1.15, -0.375), 0.3))
        fireEmitter3.setFlow(15)
        fireEmitter3.setForce(0.5, 1.5)
        fireEmitter4 = spk.StraightEmitter.create(spk.Vector3D(-0.8, 0.5, 0.2))
        fireEmitter4.setZone(spk.Sphere.create(spk.Vector3D(-0.255, -1.2, 0.225), 0.2))
        fireEmitter4.setFlow(10)
        fireEmitter4.setForce(0.5, 1.5)
        fireEmitter5 = spk.StraightEmitter.create(spk.Vector3D(0.1, 0.8, -1.0))
        fireEmitter5.setZone(spk.Sphere.create(spk.Vector3D(-0.075, -1.2, -0.3), 0.2))
        fireEmitter5.setFlow(10)
        fireEmitter5.setForce(0.5, 1.5)
        smokeEmitter = spk.SphericEmitter.create(spk.Vector3D(0.0, 1.0, 0.0), 0.0, 0.5 * math.pi)
        smokeEmitter.setZone(spk.Sphere.create(spk.Vector3D(), 1.2))
        smokeEmitter.setFlow(25)
        smokeEmitter.setForce(0.5, 1.0)

        # no Modifier

        # create Group
        fireGroup = spk.Group.create(fireModel, 135)  # type: spk.Group
        fireGroup.addEmitter(fireEmitter1)
        fireGroup.addEmitter(fireEmitter2)
        fireGroup.addEmitter(fireEmitter3)
        fireGroup.addEmitter(fireEmitter4)
        fireGroup.addEmitter(fireEmitter5)
        fireGroup.setRenderer(fireRenderer)
        fireGroup.setGravity(spk.Vector3D(0.0, 3.0, 0.0))
        self.fireGroup = fireGroup
        smokeGroup = spk.Group.create(smokeModel, 135)  # type: spk.Group
        smokeGroup.addEmitter(smokeEmitter)
        smokeGroup.setRenderer(smokeRenderer)
        smokeGroup.setGravity(spk.Vector3D(0.0, 0.4, 0.0))
        self.smokeGroup = smokeGroup

        # create System
        system = spk.System.create()  # type: spk.System
        system.addGroup(fireGroup)
        system.addGroup(smokeGroup)
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
        self.angleY = 30.0
        self.angleX = 25.0
        self.camPosZ = 12.0
        self.smoke = True
        self.lightTime = 0
        self.nbParticles = ''
        self.fps = ''
        self.strSmoke = 'SMOKE: ON'
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
            self.fireGroup.enableAABBComputing(not self.fireGroup.isAABBComputingEnabled())
            self.smokeGroup.enableAABBComputing(not self.smokeGroup.isAABBComputingEnabled())
            if self.paused:
                self.system.computeAABB()
        # renderer switching with F4
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F4:
            fireRenderers = [self.fireRenderer, self.basicRenderer, None]
            smokeRenderers = [self.smokeRenderer, self.basicRenderer, None]
            self.renderValue = (self.renderValue+1) % 3
            self.fireGroup.setRenderer(fireRenderers[self.renderValue])
            self.smokeGroup.setRenderer(smokeRenderers[self.renderValue])
        # on/off objects with F5
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F5:
            self.renderEnv = not self.renderEnv
        # PAUSE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_PAUSE:
            self.paused = not self.paused
        # on/off smoke with SPACE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_SPACE:
            self.smoke = not self.smoke
            if self.smoke:
                self.smokeGroup.getEmitter(0).setActive(True)
                self.strSmoke = 'SMOKE: ON'
            else:
                self.smokeGroup.getEmitter(0).setActive(False)
                self.strSmoke = 'SMOKE: OFF'
        # Camera rotation with mouse
        if event.type == sdl2.SDL_MOUSEMOTION:
            sensitive = 0.4
            self.angleY += event.motion.xrel * sensitive
            self.angleX += event.motion.yrel * sensitive
            self.angleX = min(45.0, max(1.0, self.angleX))
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
            # update time of particle system
            self.system.update(self.deltaTime * 0.001)  # normalize 1.0 as 1[sec]

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
            print(self.fps, ',', self.nbParticles, ',', self.strSmoke)
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
        gl.glRotated(self.angleX, 1.0, 0.0, 0.0)  # apply camera rotation
        gl.glRotated(self.angleY, 0.0, 1.0, 0.0)

        if self.renderEnv:
            # draw 3D object(ground and camp-fire)
            light_position = [0.0, 1.0, 0.0, 1.0]
            if not self.paused:
                # Light blinking simulation
                self.lightTime += self.deltaTime
                if self.lightTime >= 50:
                    lightIntensity = 1.0 + (-spk.random(0.0, 0.05) * 5.0)
                    self.lightTime = 0
                    light_position[0] = spk.random(-0.5, 0.5)
                    light_position[1] = 0.5 + spk.random(-0.5, 0.5)
                    light_position[2] = spk.random(-0.5, 0.5)
                    gl.glLightf(gl.GL_LIGHT0, gl.GL_QUADRATIC_ATTENUATION, 15.0 / lightIntensity)

            gl.glDisable(gl.GL_BLEND)
            gl.glEnable(gl.GL_DEPTH_TEST)

            gl.glLightfv(gl.GL_LIGHT0, gl.GL_POSITION, light_position)

            gl.glEnable(gl.GL_LIGHTING)
            gl.glEnable(gl.GL_LIGHT0)
            gl.glShadeModel(gl.GL_SMOOTH)
            gl.glColor4d(1.0, 1.0, 1.0, 1.0)
            gl.glPushMatrix()
            gl.glTranslatef(0.0, -1.5, 0.0)
            gl.glScalef(0.01, 0.01, 0.01)
            self.scene.draw()  # rendering of 3D objects
            gl.glPopMatrix()
            gl.glDisable(gl.GL_LIGHTING)
            gl.glDisable(gl.GL_LIGHT0)

        # draw bounding box(F2 key)
        gl.glDisable(gl.GL_TEXTURE_2D)
        gl.glDisable(gl.GL_BLEND)
        drawBoundingBox(self.fireGroup, 1.0, 0.0, 0.0)
        drawBoundingBox(self.smokeGroup, 0.0, 1.0, 0.0)

        # draw particles
        spk.GL.GLRenderer.saveGLStates()
        self.system.render()
        spk.GL.GLRenderer.restoreGLStates()

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
                self.drawText(self.strSmoke, 4.0, 72.0)
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

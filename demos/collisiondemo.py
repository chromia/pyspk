#!/usr/bin/env python

# SPARK(pyspk) Collision Demo script
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
from pyspk.GL import GLPointRenderer
from democommon import loadTexture, drawBoundingBox, drawBox


class App:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        # init SDL
        sdl2.SDL_Init(sdl2.SDL_INIT_EVERYTHING)
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_DOUBLEBUFFER, 1)
        self.window = sdl2.SDL_CreateWindow(
            b"SPARK(&pyspk) Collision Demo",
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
        # load Texture
        txParticle = loadTexture(b'res/ball2.bmp', gl.GL_RGBA, gl.GL_CLAMP)
        self.txCrate = loadTexture(b'res/crate.bmp', gl.GL_RGB, gl.GL_REPEAT)
        self.txFloor = loadTexture(b'res/floor.bmp', gl.GL_RGB, gl.GL_REPEAT)
        self.txWall = loadTexture(b'res/wall.bmp', gl.GL_RGB, gl.GL_REPEAT)

        # init FTGL
        font = FTGL.TextureFont('res/font.ttf')
        font.FaceSize(24)
        self.font = font

        # init SPARK(pyspk)
        # create Renderer
        spk.setRandomSeed(int(time.time()))
        renderer = GLPointRenderer.create(1.0)  # type: GLPointRenderer
        renderer.setType(spk.POINT_SPRITE)  # using Point Sprite(require >=OpenGL2.0)
        renderer.setTexture(txParticle)  # use the texture on drawing
        renderer.enableWorldSize(True)  # true: unit is adjusted with the screen, false: unit is pixel(fixed)
        GLPointRenderer.setPixelPerUnit(45.0 * math.pi / 180.0, self.height)
        renderer.setSize(0.05)
        renderer.setBlending(spk.BLENDING_NONE)  # blending method between particles(no blending)
        renderer.enableRenderingHint(spk.ALPHA_TEST, True)  # notifying not to use depth info
        renderer.setAlphaTestThreshold(0.8)
        self.renderer = renderer
        self.basicRenderer = GLPointRenderer.create(1.0)  # bare renderer for comparison(F4)
        # create Model
        model_enable = spk.FLAG_RED | spk.FLAG_GREEN | spk.FLAG_BLUE  # use color channels(RGB)
        model = spk.Model.create(model_enable)  # type: spk.Model
        model.setLifeTime(10.0, 10.0)  # particle's life time is 10[sec]
        self.model = model
        # create Emitter
        emit_dir = spk.Vector3D(0.0, 0.0, -1.0)
        emit_center = spk.Vector3D(0.0, 2.0, 4.475)
        minangle = 0.0*math.pi  # zenith angle(min)
        maxangle = 0.5*math.pi  # zenith angle(max)
        emitter = spk.SphericEmitter.create(emit_dir, minangle, maxangle)  # type: spk.SphericEmitter
        emitter.setZone(spk.Point.create(emit_center), True)  # particles are randomly placed in the specified zone
        emitter.setFlow(300)  # max number of particles
        emitter.setForce(1.5, 2.5)  # emittion power
        # create Group
        maxparticle = 3100  # upper limit number of particles
        group = spk.Group.create(model, maxparticle)  # type: spk.Group
        group.addEmitter(emitter)
        group.setRenderer(renderer)
        group.setCustomBirth(self.assignColor)
        group.setGravity(spk.Vector3D(0.0, -0.8, 0.0))
        group.setFriction(0.2)
        self.group = group
        # create Room objects
        self.ROOM_X = 7.0
        self.ROOM_Y = 3.0
        self.ROOM_Z = 9.0
        self.boxes = []
        self.partitions = []
        self.initRoom(group)  # obstacles
        self.createDisplayLists()  # display list for OpenGL drawing
        # create System
        system = spk.System.create()  # type: spk.System
        system.addGroup(group)
        self.system = system

        print('SPARK FACTORY AFTER INIT :')
        spk.Factory.getInstance().traceAll()

        # init Variables
        self.running = True
        self.enableBoxDrawing = False
        self.paused = False
        self.deltaTime = 0
        self.step = 0
        self.text = 2
        self.renderValue = 0
        self.renderEnv = True
        self.angleY = 90.0
        self.angleX = 45.0
        self.camPosZ = 10.0
        self.nbParticles = ''
        self.fps = ''
        self.frames = [sdl2.SDL_GetTicks()-1]

    def initRoom(self, parent):
        # creating room objects and register it to system
        # structure tree of the room is below:
        # root
        #   +-partition1(7)
        #   | +-partition0(6)
        #   | |  +-box1(1)
        #   | |  +-box2(2)
        #   | |  +-box3(3)
        #   | |  +-box4(4)
        #   | +-box5(5)
        #   +-room(0)
        CRATE_DIM2 = 1.0
        CRATE_DIM = CRATE_DIM2 * 0.5
        obstacles = []

        # room
        box = spk.AABox.create()  # type: spk.AABox
        box.setPosition(spk.Vector3D(0.0, self.ROOM_Y * 0.5, 0.0))
        box.setDimension(spk.Vector3D(self.ROOM_X - 0.05, self.ROOM_Y - 0.05, self.ROOM_Z - 0.05))
        self.boxes.append(box)
        obstacle = spk.Obstacle.create()  # type: spk.Obstacle
        obstacle.setZone(box)
        obstacle.setBouncingRatio(0.8)
        obstacle.setFriction(0.95)
        obstacles.append(obstacle)

        # crates
        centers = []
        centers.append(spk.Vector3D(0.0, CRATE_DIM, 0.0))
        centers.append(spk.Vector3D(CRATE_DIM2, CRATE_DIM, 0.2))
        centers.append(spk.Vector3D(CRATE_DIM2, CRATE_DIM, 0.2 + CRATE_DIM2))
        centers.append(spk.Vector3D(CRATE_DIM2, CRATE_DIM2 + CRATE_DIM, 0.5))
        centers.append(spk.Vector3D(-1.6, CRATE_DIM, -0.8))
        self.crate_positions = centers
        for pos in centers:
            box = spk.AABox.create()  # type: spk.AABox
            box.setPosition(pos)
            box.setDimension(spk.Vector3D(CRATE_DIM2 + 0.05, CRATE_DIM2 + 0.05, CRATE_DIM2 + 0.05))
            self.boxes.append(box)
            obstacle = spk.Obstacle.create()  # type: spk.Obstacle
            obstacle.setZone(box)
            obstacle.setBouncingRatio(0.8)
            obstacle.setFriction(0.95)
            obstacles.append(obstacle)

        # partitioning(grouping)
        # 1. calculate min&max along each axis)
        vMin0 = spk.Vector3D(10000, 10000, 10000)  # box1-4(nb:4)
        vMax0 = spk.Vector3D(-10000, -10000, -10000)
        vMin1 = spk.Vector3D(10000, 10000, 10000)  # box1-5(nb:5)
        vMax1 = spk.Vector3D(-10000, -10000, -10000)
        # 1.1. calc vMin0 & vMax0
        for pos in centers[:-1]:  # exclude last box
            vMin0.x = min(vMin0.x, pos.x)
            vMin0.y = min(vMin0.y, pos.y)
            vMin0.z = min(vMin0.z, pos.z)
            vMax0.x = max(vMax0.x, pos.x)
            vMax0.y = max(vMax0.y, pos.y)
            vMax0.z = max(vMax0.z, pos.z)
        # 1.2 calc vMin1 & vMax1
        lastboxpos = centers[-1]
        vMin1.x = min(vMin0.x, lastboxpos.x)
        vMin1.y = min(vMin0.y, lastboxpos.y)
        vMin1.z = min(vMin0.z, lastboxpos.z)
        vMax1.x = max(vMax0.x, lastboxpos.x)
        vMax1.y = max(vMax0.y, lastboxpos.y)
        vMax1.z = max(vMax0.z, lastboxpos.z)
        # 2. register {box1-4} as partition0
        box0 = spk.AABox.create()  # type: spk.AABox
        box0.setPosition((vMin0 + vMax0) / 2.0)
        box0.setDimension(vMax0 - vMin0 + CRATE_DIM2 + 0.05)
        self.partitions.append(box0)
        partition0 = spk.ModifierGroup.create(box0, spk.INSIDE_ZONE)  # type: spk.ModifierGroup
        partition0.addModifier(obstacles[1])
        partition0.addModifier(obstacles[2])
        partition0.addModifier(obstacles[3])
        partition0.addModifier(obstacles[4])
        # 3. register {box1-5} as partition1
        box1 = spk.AABox.create()  # type: spk.AABox
        box1.setPosition((vMin1 + vMax1) / 2.0)
        box1.setDimension(vMax1 - vMin1 + CRATE_DIM2 + 0.05)
        self.partitions.append(box1)
        partition1 = spk.ModifierGroup.create(box1, spk.INSIDE_ZONE)  # type: spk.ModifierGroup
        partition1.addModifier(partition0)
        partition1.addModifier(obstacles[5])
        # 4. register all to root
        parent.addModifier(obstacles[0])
        parent.addModifier(partition1)

    @staticmethod
    def assignColor(particle):
        # called on time the birth of new particle
        # select particle color randomly from four
        color = spk.randint(0, 4)
        if color == 0:  # RED
            particle.setParamCurrentValue(spk.PARAM_RED, 0.8)
            particle.setParamCurrentValue(spk.PARAM_GREEN, 0.0)
            particle.setParamCurrentValue(spk.PARAM_BLUE, 0.0)
        elif color == 1:  # GREEN
            particle.setParamCurrentValue(spk.PARAM_RED, 0.0)
            particle.setParamCurrentValue(spk.PARAM_GREEN, 0.8)
            particle.setParamCurrentValue(spk.PARAM_BLUE, 0.0)
        elif color == 2:  # BLUE
            particle.setParamCurrentValue(spk.PARAM_RED, 0.0)
            particle.setParamCurrentValue(spk.PARAM_GREEN, 0.0)
            particle.setParamCurrentValue(spk.PARAM_BLUE, 0.8)
        else:  # YELLOW
            particle.setParamCurrentValue(spk.PARAM_RED, 0.8)
            particle.setParamCurrentValue(spk.PARAM_GREEN, 0.8)
            particle.setParamCurrentValue(spk.PARAM_BLUE, 0.0)

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
        # on/off Obstacles with F5
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F5:
            self.renderEnv = not self.renderEnv
        # on/off border of crates&room with F6
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_F6:
            self.enableBoxDrawing = not self.enableBoxDrawing
        # PAUSE
        if event.type == sdl2.SDL_KEYDOWN and event.key.keysym.sym == sdl2.SDLK_PAUSE:
            self.paused = not self.paused
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

        # draw obstacles
        if self.renderEnv:
            gl.glDisable(gl.GL_BLEND)
            gl.glTexEnvi(gl.GL_TEXTURE_ENV, gl.GL_TEXTURE_ENV_MODE, gl.GL_MODULATE)
            gl.glCallList(self.DISPLAY_LIST_WORLD)

        # draw bounding box(F2 key)
        drawBoundingBox(self.group, 1.0, 0.0, 0.0)

        # draw outlines of the boxes(F6 key)
        if self.enableBoxDrawing:
            for box in self.boxes:  # blue for crates
                drawBox(box.getPosition() - box.getDimension() * 0.5,
                        box.getPosition() + box.getDimension() * 0.5, 0.0, 0.0, 1.0)
            for box in self.partitions:  # green for partitions
                drawBox(box.getPosition() - box.getDimension() * 0.5,
                        box.getPosition() + box.getDimension() * 0.5, 0.0, 1.0, 0.0)

        # draw particles
        gl.glDisable(gl.GL_LIGHTING)
        self.system.render()

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

    def createDisplayLists(self):
        TEXTURE_SIZE = 2.0
        CRATE_DIM2 = 1.0
        CRATE_DIM = CRATE_DIM2 * 0.5

        X = self.ROOM_X * 0.5
        Y = self.ROOM_Y
        Z = self.ROOM_Z * 0.5

        self.DISPLAY_LIST_WORLD = 1
        gl.glNewList(self.DISPLAY_LIST_WORLD, gl.GL_COMPILE)

        gl.glEnable(gl.GL_CULL_FACE)
        gl.glDisable(gl.GL_BLEND)
        gl.glEnable(gl.GL_TEXTURE_2D)

        # drawing floor
        gl.glBindTexture(gl.GL_TEXTURE_2D, self.txFloor)
        gl.glBegin(gl.GL_QUADS)
        gl.glColor3d(0.6, 0.6, 0.6)
        gl.glTexCoord2d(X / TEXTURE_SIZE, Z / TEXTURE_SIZE)
        gl.glVertex3d(X, 0.0, Z)
        gl.glTexCoord2d(X / TEXTURE_SIZE, -Z / TEXTURE_SIZE)
        gl.glVertex3d(X, 0.0, -Z)
        gl.glTexCoord2d(-X / TEXTURE_SIZE, -Z / TEXTURE_SIZE)
        gl.glVertex3d(-X, 0.0, -Z)
        gl.glTexCoord2d(-X / TEXTURE_SIZE, Z / TEXTURE_SIZE)
        gl.glVertex3d(-X, 0.0, Z)
        gl.glEnd()

        # drawing wall&ceiling
        gl.glBindTexture(gl.GL_TEXTURE_2D, self.txWall)
        gl.glBegin(gl.GL_QUADS)

        gl.glColor3d(0.2, 0.2, 0.2)
        gl.glTexCoord2d(X / TEXTURE_SIZE, Y / TEXTURE_SIZE)
        gl.glVertex3d(X, Y, Z)
        gl.glTexCoord2d(X / TEXTURE_SIZE, 0.0)
        gl.glVertex3d(X, 0.0, Z)
        gl.glTexCoord2d(-X / TEXTURE_SIZE, 0.0)
        gl.glVertex3d(-X, 0.0, Z)
        gl.glTexCoord2d(-X / TEXTURE_SIZE, Y / TEXTURE_SIZE)
        gl.glVertex3d(-X, Y, Z)

        gl.glColor3d(0.6, 0.6, 0.6)
        gl.glTexCoord2d(-X / TEXTURE_SIZE, Y / TEXTURE_SIZE)
        gl.glVertex3d(-X, Y, -Z)
        gl.glTexCoord2d(-X / TEXTURE_SIZE, 0.0)
        gl.glVertex3d(-X, 0.0, -Z)
        gl.glTexCoord2d(X / TEXTURE_SIZE, 0.0)
        gl.glVertex3d(X, 0.0, -Z)
        gl.glTexCoord2d(X / TEXTURE_SIZE, Y / TEXTURE_SIZE)
        gl.glVertex3d(X, Y, -Z)

        gl.glColor3d(0.4, 0.4, 0.4)
        gl.glTexCoord2d(Y / TEXTURE_SIZE, -Z / TEXTURE_SIZE)
        gl.glVertex3d(X, Y, -Z)
        gl.glTexCoord2d(0.0, -Z / TEXTURE_SIZE)
        gl.glVertex3d(X, 0.0, -Z)
        gl.glTexCoord2d(0.0, Z / TEXTURE_SIZE)
        gl.glVertex3d(X, 0.0, Z)
        gl.glTexCoord2d(Y / TEXTURE_SIZE, Z / TEXTURE_SIZE)
        gl.glVertex3d(X, Y, Z)

        gl.glColor3d(0.4, 0.4, 0.4)
        gl.glTexCoord2d(Y / TEXTURE_SIZE, Z / TEXTURE_SIZE)
        gl.glVertex3d(-X, Y, Z)
        gl.glTexCoord2d(0.0, Z / TEXTURE_SIZE)
        gl.glVertex3d(-X, 0.0, Z)
        gl.glTexCoord2d(0.0, -Z / TEXTURE_SIZE)
        gl.glVertex3d(-X, 0.0, -Z)
        gl.glTexCoord2d(Y / TEXTURE_SIZE, -Z / TEXTURE_SIZE)
        gl.glVertex3d(-X, Y, -Z)

        gl.glColor3d(0.3, 0.3, 0.3)
        gl.glTexCoord2d(-X / TEXTURE_SIZE, Z / TEXTURE_SIZE)
        gl.glVertex3d(-X, Y, Z)
        gl.glTexCoord2d(-X / TEXTURE_SIZE, -Z / TEXTURE_SIZE)
        gl.glVertex3d(-X, Y, -Z)
        gl.glTexCoord2d(X / TEXTURE_SIZE, -Z / TEXTURE_SIZE)
        gl.glVertex3d(X, Y, -Z)
        gl.glTexCoord2d(X / TEXTURE_SIZE, Z / TEXTURE_SIZE)
        gl.glVertex3d(X, Y, Z)
        gl.glEnd()

        # drawing crates
        gl.glBindTexture(gl.GL_TEXTURE_2D, self.txCrate)
        gl.glBegin(gl.GL_QUADS)

        for pos in self.crate_positions:
            X = pos.x
            Y = pos.y
            Z = pos.z

            gl.glColor3d(0.2, 0.2, 0.2)
            gl.glTexCoord2d(1.0, 1.0)
            gl.glVertex3d(X + CRATE_DIM, Y + CRATE_DIM, Z - CRATE_DIM)
            gl.glTexCoord2d(1.0, 0.0)
            gl.glVertex3d(X + CRATE_DIM, Y - CRATE_DIM, Z - CRATE_DIM)
            gl.glTexCoord2d(0.0, 0.0)
            gl.glVertex3d(X - CRATE_DIM, Y - CRATE_DIM, Z - CRATE_DIM)
            gl.glTexCoord2d(0.0, 1.0)
            gl.glVertex3d(X - CRATE_DIM, Y + CRATE_DIM, Z - CRATE_DIM)

            gl.glColor3d(1.0, 1.0, 1.0)
            gl.glTexCoord2d(0.0, 1.0)
            gl.glVertex3d(X - CRATE_DIM, Y + CRATE_DIM, Z + CRATE_DIM)
            gl.glTexCoord2d(0.0, 0.0)
            gl.glVertex3d(X - CRATE_DIM, Y - CRATE_DIM, Z + CRATE_DIM)
            gl.glTexCoord2d(1.0, 0.0)
            gl.glVertex3d(X + CRATE_DIM, Y - CRATE_DIM, Z + CRATE_DIM)
            gl.glTexCoord2d(1.0, 1.0)
            gl.glVertex3d(X + CRATE_DIM, Y + CRATE_DIM, Z + CRATE_DIM)

            gl.glColor3d(0.6, 0.6, 0.6)
            gl.glTexCoord2d(1.0, 0.0)
            gl.glVertex3d(X - CRATE_DIM, Y + CRATE_DIM, Z - CRATE_DIM)
            gl.glTexCoord2d(0.0, 0.0)
            gl.glVertex3d(X - CRATE_DIM, Y - CRATE_DIM, Z - CRATE_DIM)
            gl.glTexCoord2d(0.0, 1.0)
            gl.glVertex3d(X - CRATE_DIM, Y - CRATE_DIM, Z + CRATE_DIM)
            gl.glTexCoord2d(1.0, 1.0)
            gl.glVertex3d(X - CRATE_DIM, Y + CRATE_DIM, Z + CRATE_DIM)

            gl.glColor3d(0.6, 0.6, 0.6)
            gl.glTexCoord2d(1.0, 1.0)
            gl.glVertex3d(X + CRATE_DIM, Y + CRATE_DIM, Z + CRATE_DIM)
            gl.glTexCoord2d(0.0, 1.0)
            gl.glVertex3d(X + CRATE_DIM, Y - CRATE_DIM, Z + CRATE_DIM)
            gl.glTexCoord2d(0.0, 0.0)
            gl.glVertex3d(X + CRATE_DIM, Y - CRATE_DIM, Z - CRATE_DIM)
            gl.glTexCoord2d(1.0, 0.0)
            gl.glVertex3d(X + CRATE_DIM, Y + CRATE_DIM, Z - CRATE_DIM)

            gl.glColor3d(0.8, 0.8, 0.8)
            gl.glTexCoord2d(1.0, 1.0)
            gl.glVertex3d(X + CRATE_DIM, Y + CRATE_DIM, Z + CRATE_DIM)
            gl.glTexCoord2d(1.0, 0.0)
            gl.glVertex3d(X + CRATE_DIM, Y + CRATE_DIM, Z - CRATE_DIM)
            gl.glTexCoord2d(0.0, 0.0)
            gl.glVertex3d(X - CRATE_DIM, Y + CRATE_DIM, Z - CRATE_DIM)
            gl.glTexCoord2d(0.0, 1.0)
            gl.glVertex3d(X - CRATE_DIM, Y + CRATE_DIM, Z + CRATE_DIM)

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

#pragma once

//Registration function for boost.python
//Call in the order: base -> derived

//Core
extern void init_SPK_DEF_h();
extern void init_SPK_Vector3D_h();
extern void init_SPK_Registerable_h();
extern void init_SPK_Transformable_h();
extern void init_SPK_Factory_h();
extern void init_SPK_Interpolator_h();
extern void init_SPK_Model_h();
extern void init_SPK_BufferHandler_h();
extern void init_SPK_Renderer_h();
extern void init_SPK_Emitter_h();
extern void init_SPK_Modifier_h();
extern void init_SPK_Zone_h();
extern void init_SPK_Particle_h();
extern void init_SPK_Group_h();
extern void init_SPK_System_h();

//Extensions/Emitters
extern void init_SPK_NormalEmitter_h();
extern void init_SPK_RandomEmitter_h();
extern void init_SPK_SphericEmitter_h();
extern void init_SPK_StaticEmitter_h();
extern void init_SPK_StraightEmitter_h();

//Extensions/Modifiers
extern void init_SPK_Collision_h();
extern void init_SPK_Destroyer_h();
extern void init_SPK_LinearForce_h();
extern void init_SPK_ModifierGroup_h();
extern void init_SPK_Obstacle_h();
extern void init_SPK_PointMass_h();
extern void init_SPK_Rotator_h();
extern void init_SPK_Vortex_h();

//Extensions/Renderers
extern void init_SPK_LineRendererInterface_h();
extern void init_SPK_PointRendererInterface_h();
extern void init_SPK_QuadRendererInterface_h();
extern void init_SPK_Oriented2DRendererInterface_h();
extern void init_SPK_Oriented3DRendererInterface_h();

//Extensions/Zones
extern void init_SPK_AABox_h();
extern void init_SPK_Cylinder_h();
extern void init_SPK_Line_h();
extern void init_SPK_Plane_h();
extern void init_SPK_Point_h();
extern void init_SPK_Ring_h();
extern void init_SPK_Sphere_h();

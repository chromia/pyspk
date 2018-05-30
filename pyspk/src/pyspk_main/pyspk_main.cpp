#include <boost/python.hpp>
#include "pyspk_main.hpp"

BOOST_PYTHON_MODULE(_pyspk)
{
	//Registering class definitions
	//Attention: base class must be registered prior to derived class. if not, a runtime error will occur.
	//各クラス定義の登録を実行
	//注意: 基底クラスは先に登録しないと実行時エラー

	//Core
	init_SPK_DEF_h();
	init_SPK_Registerable_h();
	init_SPK_Transformable_h();
	init_SPK_BufferHandler_h();
	init_SPK_Vector3D_h();
	init_SPK_Particle_h();
	init_SPK_Factory_h();
	init_SPK_Interpolator_h();
	init_SPK_Model_h();			//req Registerable
	init_SPK_Group_h();			//req Registerable, Transformable
	init_SPK_Zone_h();			//req Registerable, Transformable
	init_SPK_Renderer_h();		//req Registerable, BufferHandler
	init_SPK_Emitter_h();		//req Registerable, Transformable
	init_SPK_Modifier_h();		//req Registerable, Transformable, BufferHandler	
	init_SPK_System_h();		//req Registerable, Transformable

	////Extensions/Emitters
	init_SPK_NormalEmitter_h();		//req Emitter
	init_SPK_RandomEmitter_h();		//req Emitter
	init_SPK_SphericEmitter_h();	//req Emitter
	init_SPK_StaticEmitter_h();		//req Emitter
	init_SPK_StraightEmitter_h();	//req Emitter

	////Extensions/Modifiers
	init_SPK_Collision_h();			//req Modifier
	init_SPK_Destroyer_h();			//req Modifier
	init_SPK_LinearForce_h();		//req Modifier
	init_SPK_ModifierGroup_h();		//req Modifier
	init_SPK_Obstacle_h();			//req Modifier
	init_SPK_PointMass_h();			//req Modifier
	init_SPK_Rotator_h();			//req Modifier
	init_SPK_Vortex_h();			//req Modifier

	////Extension/Renderers
	init_SPK_LineRendererInterface_h();
	init_SPK_PointRendererInterface_h();
	init_SPK_QuadRendererInterface_h();
	init_SPK_Oriented2DRendererInterface_h();
	init_SPK_Oriented3DRendererInterface_h();

	////Extensions/Zones
	init_SPK_AABox_h();			//req Zone
	init_SPK_Cylinder_h();		//req Zone
	init_SPK_Line_h();			//req Zone
	init_SPK_Plane_h();			//req Zone
	init_SPK_Point_h();			//req Zone
	init_SPK_Ring_h();			//req Zone
	init_SPK_Sphere_h();		//req Zone
}

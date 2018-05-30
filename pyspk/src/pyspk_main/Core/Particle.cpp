// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Particle.h


#include <Core/SPK_Particle.h>
#include <Core/SPK_Group.h>
#include <boost/python.hpp>


void init_SPK_Particle_h() {
	boost::python::class_<SPK::Particle, boost::noncopyable>("Particle", boost::python::no_init)
		.def("setParamCurrentValue", &SPK::Particle::setParamCurrentValue,
			":brief: Sets the current value for the given parameter\n"
			"\n"
			"Note that the method will have no effect if the parameter is not enabled in the Particle's Model.\n"
			"In this case, the method returns false.\n"
			"\n"
			":param type: the parameter to set\n"
			":param value: the value of the parameter\n"
			":return: True if the parameter can be set, False otherwise (if the parameter is not enabled)\n")
		.def("setParamFinalValue", &SPK::Particle::setParamFinalValue,
			":brief: Sets the final value for the given parameter\n"
			"\n"
			"The final value is the value the parameter will have at the end of the Particle's life.\n"
			"Note that the method will have no effect if the parameter is not mutable in the Particle's Model.\n"
			"In this case, the method returns false and setParamCurrentValue(ModelParam,float) should be used.\n"
			"\n"
			":param type: the parameter to set\n"
			":param value: the value of the parameter\n"
			":return: True if the parameter can be set, False otherwise (if the parameter is not mutable)\n")
		.def("changeParamCurrentValue", &SPK::Particle::changeParamCurrentValue,
			":brief: Changes the current value for the given parameter\n"
			"\n"
			"The delta is added to the current value of the parameter.\n"
			"For more information see setParamCurrentValue(ModelParam,float).\n"
			"\n"
			":param type: the parameter to set\n"
			":param delta: the delta\n"
			":return: True if the parameter can be changed, False otherwise (if the parameter is not enabled)\n"
			":since: 1.02.00\n")
		.def("changeParamFinalValue", &SPK::Particle::changeParamFinalValue,
			":brief: Changes the final value for the given parameter\n"
			"\n"
			"The delta is added to the final value of the parameter.\n"
			"For more information see setParamFinalValue(ModelParam,float).\n"
			"\n"
			":param type: the parameter to set\n"
			":param delta: the delta\n"
			":return: True if the parameter can be changed, False otherwise (if the parameter is not mutable)\n"
			":since: 1.02.00\n")
		.def("setLifeLeft", &SPK::Particle::setLifeLeft,
			":brief: Sets the life left of the Particle.\n"
			"\n"
			"When the Particle's life reaches 0, the Particle is inactivated.\n"
			"\n"
			":param life: the amount of life left of the Particle\n")
		.def("position", static_cast<SPK::Vector3D &(SPK::Particle::*)()>(&SPK::Particle::position),
			":brief: Gets the position of the Particle\n"
			":return: the position of this Particle\n"
			":since: 1.02.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		//.def("position", static_cast<const SPK::Vector3D &(SPK::Particle::*)() const>(&SPK::Particle::position), boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("velocity", static_cast<SPK::Vector3D &(SPK::Particle::*)()>(&SPK::Particle::velocity),
			":brief: Gets the velocity of the Particle\n"
			":return: the velocity of this Particle\n"
			":since: 1.02.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		//.def("velocity", static_cast<const SPK::Vector3D &(SPK::Particle::*)() const>(&SPK::Particle::velocity), boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("oldPosition", static_cast<SPK::Vector3D &(SPK::Particle::*)()>(&SPK::Particle::oldPosition),
			":brief: Gets the old position of the Particle\n"
			":return: the old position of this Particle\n"
			":since: 1.02.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		//.def("oldPosition", static_cast<const SPK::Vector3D &(SPK::Particle::*)() const>(&SPK::Particle::oldPosition), boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getParamCurrentValue", &SPK::Particle::getParamCurrentValue,
			":brief: Gets the current value for the given parameter\n"
			"\n"
			"Note that if the the parameter is not enabled in the Particle's Model, the default value for the parameter is returned.\n"
			"\n"
			":param type: the parameter to get the value\n"
			":return: the current value of the parameter\n")
		.def("getParamFinalValue", &SPK::Particle::getParamFinalValue,
			":brief: Gets the final value for the given parameter\n"
			"\n"
			"Note that if the the parameter is not enabled in the Particle's Model, the default value for the parameter is returned.\n"
			"If the parameter is enabled but not mutable, the current value is returned.\n"
			"\n"
			":param type: the parameter to get the value\n"
			":return: the current value of the parameter\n")
		.def("getModel", &SPK::Particle::getModel,
			":brief: Gets the Model of this Particle\n"
			":return: A pointer on the Model of this Particle\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getGroup", &SPK::Particle::getGroup,
			":brief: Gets the group of this Particle\n"
			":return: A pointer on the Group of this Particle\n"
			":since: 1.02.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getIndex", &SPK::Particle::getIndex,
			":brief: Gets the index of this Particle in its Group\n"
			":return: the index of thi Particle in its Group\n"
			":since: 1.03.00\n")
		.def("getLifeLeft", &SPK::Particle::getLifeLeft,
			":brief: Gets the amount of life left of the Particle\n"
			"\n"
			"The life left of the Particle is the time left before the Particle dies.\n"
			"Note that in case of immortal particles, this value does not evolve.\n"
			"\n"
			":return: the amount of life left\n")
		.def("getAge", &SPK::Particle::getAge,
			":brief: Gets the age of the Particle\n"
			"\n"
			"The age of a Particle starts at zero when it is initialized and evolve at each update.\n"
			"Note that even immortal particles gets older.\n"
			"\n"
			":return: the age of the particle\n"
			":since: 1.03.00\n")
		.def("getDistanceFromCamera", &SPK::Particle::getDistanceFromCamera,
			":brief: Gets the distance of this Particle from the camera.\n"
			"\n"
			"Note that the correct distance is only returned if the Group of this Particles has its distance computation enabled.\n"
			"\n"
			":return: the distance of this Particle from the camera\n"
			":since: 1.01.00\n")
		.def("getSqrDistanceFromCamera", &SPK::Particle::getSqrDistanceFromCamera,
			":brief: Gets the square distance of this Particle from the camera.\n"
			"\n"
			"Note that the correct distance is only returned if the Group of this Particles has its distance computation enabled.\n"
			"This method is faster than getDistanceFromCamera() and should be used instead when possible.\n"
			"\n"
			":return: the square distance of this Particle from the camera\n"
			":since: 1.01.00\n")
		.def("isNewBorn", &SPK::Particle::isNewBorn,
			":brief: Tells whether this Particle was initialized at its latest update or not\n"
			"\n"
			"A call to this method is equivalent to <i>getAge() == 0.0</i>\n"
			"\n"
			":return: True if this Particle was initialized at its latest update, False if not\n"
			":since: 1.03.00\n")
		.def("isAlive", &SPK::Particle::isAlive,
			":brief: Tells whether this Particle is alive or not\n"
			"\n"
			"A call to this method is equivalent to <i>getLifeLeft > 0.0</i>\n"
			"\n"
			":return: True if this Particle is alive, False if it is dead\n"
			":since: 1.04.00\n")
		.def("init", &SPK::Particle::init,
			":brief: Initializes the Particle\n"
			"\n"
			"When a Particle is initialized, all its parameters are reinitialized as well as its life.\n")
		.def("kill", &SPK::Particle::kill,
			":brief: Kills this Particle\n"
			"\n"
			"This method is equivalent to a call to setLifeLeft(float) with life being 0.\n"
			"\n"
			":since: 1.01.00\n")
		.def("getR", &SPK::Particle::getR,
			":brief: direct accessor for Red color component")
		.def("getG", &SPK::Particle::getG,
			":brief: direct accessor for Green color component")
		.def("getB", &SPK::Particle::getB,
			":brief: direct accessor for Blue color component")
		;
}

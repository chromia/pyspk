// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Emitter.h


#include "Core/SPK_Emitter.h"
#include "Core/SPK_Group.h"
#include "Core/SPK_Particle.h"
#include <boost/python.hpp>


class EmitterWrapper :
	public SPK::Emitter,
	public boost::python::wrapper<SPK::Emitter>
{
public:
	using Emitter = SPK::Emitter;

	std::string getClassName() const
	{
		return this->get_override("getClassName")();
	}
	Registerable * clone(bool createBase) const
	{
		return this->get_override("clone")(createBase);
	}

	SPK::Registerable * findByName(const std::string & name) {
		if (auto findByName = this->get_override("findByName")) {
			return findByName(name);
		}
		else {
			return Emitter::findByName(name);
		}
	}
	void registerChildren(bool registerAll) {
		if (auto registerChildren = this->get_override("_registerChildren")) {
			registerChildren(registerAll);
		}
		else {
			Emitter::registerChildren(registerAll);
		}
	}
	void copyChildren(const SPK::Registerable & object, bool createBase) {
		if (auto copyChildren = this->get_override("_copyChildren")) {
			copyChildren(object, createBase);
		}
		else {
			Emitter::copyChildren(object, createBase);
		}
	}
	void destroyChildren(bool keepChildren) {
		if (auto destroyChildren = this->get_override("_destroyChildren")) {
			destroyChildren(keepChildren);
		}
		else {
			Emitter::destroyChildren(keepChildren);
		}
	}
	void propagateUpdateTransform() {
		if (auto propagateUpdateTransform = this->get_override("_propagateUpdateTransform")) {
			propagateUpdateTransform();
		}
		else {
			Emitter::propagateUpdateTransform();
		}
	}

	void generateVelocity(SPK::Particle& particle, float speed) const
	{
		this->get_override("generateVelocity")(particle, speed);
	}
};
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(EmittersetZoneOverloads0, SPK::Emitter::setZone, 1, 2)

void init_SPK_Emitter_h() {

	void (SPK::Emitter::*gen)(SPK::Particle&) const = &SPK::Emitter::generateVelocity;

	boost::python::class_<EmitterWrapper, boost::python::bases<SPK::Registerable, SPK::Transformable>, boost::noncopyable>("Emitter")
		.def("setActive", &SPK::Emitter::setActive,
			":brief: Sets this Emitter active or not.\n"
			"\n"
			"An inactive Emitter will not emit in its parent Group during update.\n"
			"However it can still be used manually by the user.\n"
			"\n"
			":param active: True to activate this Emitter, False to deactivate it\n"
			":since: 1.05.00\n")
		.def("setTank", &SPK::Emitter::setTank,
			":brief: Sets the number of particles in this Emitter's tank\n"
			"\n"
			"Each time the Emitter is updated, the number of particles emitted is deduced from the Emitter tank.\n"
			"When the tank reaches 0, the Emitter will not emit any longer until it is refilled.\n"
			"\n"
			"A number of -1 (or any negative number) means the Emitter has an infinite tank which will never be empty.\n"
			"\n"
			":param tank: the number of particles in this Emitters's tank\n")
		.def("changeTank", &SPK::Emitter::changeTank,
			":brief: Changes the number of particles in this Emitters's tank\n"
			"\n"
			"The new number of particles in the tank is equal to : <i>number of particles in the tank + n</i>.\n"
			"This method has no effect for Emitters with infinite tank (a negative number of particles) and an Emitter cannot become infinite with this method (the new number is clamped to 0).\n"
			"\n"
			":param deltaTank: the number to add to the current tank\n")
		.def("setFlow", &SPK::Emitter::setFlow,
			":brief: Sets the flow of this Emitter\n"
			"\n"
			"The flow is in the unit : nb of particle per step.\n"
			"A flow of -1 (or any negative number) indicates an infinite flow which means all particles in the Emitters(s tank are generated instantly.\n"
			"Note that if both the flow and the tank are infinite, nothing will happen.\n"
			"\n"
			":param flow: the flow of this Emitter\n")
		.def("changeFlow", &SPK::Emitter::changeFlow,
			":brief: Changes the flow of particles of this Emitter\n"
			"\n"
			"The new flow is equal to : <i>flow of the Emitter + deltaFlow</i>.\n"
			"This method has no effect for Emitters with infinite flow (a negative flow of particles) and an Emitter's flow cannot become infinite with this method (the new flow is clamped to 0).\n"
			"\n"
			":param deltaFlow: the number to add to the current flow\n")
		.def("setForce", &SPK::Emitter::setForce,
			":brief: Sets the force of this Emitter\n"
			"\n"
			"The force of the Emitter vary for each launch of a Particle between a minimum and a maximum.\n"
			"To have a fixed force for the Emitter, just have <i>min = max</i>.\n"
			"\n"
			"the speed at which a Particle will be launched is equal to : <i>force / weight of the Particle</i>.\n"
			"\n"
			":param min: the minimum force of the Emitter\n"
			":param max: the maximum force of the Emitter\n")
		.def("setZone", &SPK::Emitter::setZone, EmittersetZoneOverloads0(
			":brief: Sets the Zone of this Emitter\n"
			"\n"
			"If the Zone is NULL, the default Zone will be used (A Point centered at the origin)\n"
			"\n"
			":param zone: the Zone of this Emitter\n"
			":param full: True to generate particles in the whole Zone, False to generate particles only at the Zone borders.\n"))
		.def("isActive", &SPK::Emitter::isActive,
			":brief: Tells whether this Emitter is active or not\n"
			":return: True if this Emitter is active, False if is is inactive\n"
			":since: 1.05.00\n")
		.def("getTank", &SPK::Emitter::getTank,
			":brief: Gets the number of particles in this Emitter's tank\n"
			":return: the number of particles in this Emitters's tanl\n")
		.def("getFlow", &SPK::Emitter::getFlow,
			":brief: Gets the flow of this Emitter\n"
			":return: the flow of this Emitter\n")
		.def("getForceMin", &SPK::Emitter::getForceMin,
			":brief: Gets the minimum force of this Emitter\n"
			":return: the minimum force of this Emitter\n")
		.def("getForceMax", &SPK::Emitter::getForceMax,
			":brief: Gets the maximum force of this Emitter\n"
			":return: the maximum force of this Emitter\n")
		.def("getZone", &SPK::Emitter::getZone, 
			":brief: Gets the Zone of this Emitter\n"
			":return: the Zone of this Emitter\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("isFullZone", &SPK::Emitter::isFullZone,
			":brief: Tells whether this Emitter emits in the whole Zone or only at its borders\n"
			":return: True if this EMitter emits in the whole Zone, False if it is only at its borders\n")
		.def("isSleeping", &SPK::Emitter::isSleeping,
			":brief: Tells whether this Emitter is sleeping or not\n"
			"\n"
			"An Emitter is considered as sleeping if his flow or his tank is equal to 0.\n"
			"\n"
			":return: True if this Emitter is sleeping, False if it is active\n"
			":since: 1.03.00\n")
		.def("emit", &SPK::Emitter::emit,
			":brief: Emits a Particle from this Emitter\n"
			"\n"
			"The Particle's velocity is updated with a call to generateVelocity(Particle&).\n"
			"The Particle's position is updated with a call to Zone::generatePosition(Particle&) of the Emitter's Zone.\n"
			"\n"
			"Note that this will not decrease the number of particles in the Emitter's tank.\n"
			"To do it, the user has to manually make a call to changeNumber(-1) after this call.\n"
			"\n"
			":param particle: the Particle to emit from this Emitter\n")
		.def("generateVelocity", gen,
			":brief: Generates the velocity of the Particle\n"
			"\n"
			"The velocity of the Particle is updated in function of the Emitter's nature and parameters.\n"
			"Unlike emit() the position of the Particle remains unchanged.\n"
			"\n"
			":param particle: the Particle whose velocity has to be updated\n")
		.def("findByName", &SPK::Emitter::findByName, boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		;
}

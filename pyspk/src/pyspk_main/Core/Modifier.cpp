// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Modifier.h


#include <Core/SPK_Modifier.h>
#include <boost/python.hpp>


class ModifierWrapper :
	public SPK::Modifier,
	public boost::python::wrapper<SPK::Modifier>
{
public:
	using SPK::Modifier::Modifier;

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
			return Modifier::findByName(name);
		}
	}
	void registerChildren(bool registerAll) {
		if (auto registerChildren = this->get_override("_registerChildren")) {
			registerChildren(registerAll);
		}
		else {
			Modifier::registerChildren(registerAll);
		}
	}
	void copyChildren(const SPK::Registerable & object, bool createBase) {
		if (auto copyChildren = this->get_override("_copyChildren")) {
			copyChildren(object, createBase);
		}
		else {
			Modifier::copyChildren(object, createBase);
		}
	}
	void destroyChildren(bool keepChildren) {
		if (auto destroyChildren = this->get_override("_destroyChildren")) {
			destroyChildren(keepChildren);
		}
		else {
			Modifier::destroyChildren(keepChildren);
		}
	}
	void propagateUpdateTransform() {
		if (auto propagateUpdateTransform = this->get_override("_propagateUpdateTransform")) {
			propagateUpdateTransform();
		}
		else {
			Modifier::propagateUpdateTransform();
		}
	}

	void modify(SPK::Particle& particle, float deltaTime) const
	{
		this->get_override("modify")(particle, deltaTime);
	}
};
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ModifiersetZoneOverloads0, SPK::Modifier::setZone, 1, 2)

void init_SPK_Modifier_h() {
	boost::python::class_<ModifierWrapper, boost::python::bases<SPK::Registerable, SPK::Transformable, SPK::BufferHandler>, boost::noncopyable>("Modifier", boost::python::init<boost::python::optional<int, SPK::ModifierTrigger, bool, bool, SPK::Zone *>>(
			":brief: Constructor of Modifier\n"
			":param availableTriggers: the available triggers for this Modifier (OR-ed)\n"
			":param trigger: the default trigger of the Modifier\n"
			":param needsIntersection: True if the computation of the intersection is needed, False otherwise\n"
			":param needsNormal: True if the computation of the normal is needed, False otherwise\n"
			":param zone: the Zone of the Modifier\n"))
		.def("setActive", &SPK::Modifier::setActive,
			":brief: Sets this Modifier active or not.\n"
			"\n"
			"An inactive Modifier will not affect its parent Group during update.\n"
			"However it can still be used manually by the user.\n"
			"\n"
			":param active: True to activate this Modifier, False to deactivate it\n"
			":since: 1.03.00\n")
		.def("setZone", &SPK::Modifier::setZone, ModifiersetZoneOverloads0(
			":brief: Sets the Zone of this Modifier\n"
			"\n"
			"If the Zone is None, the Zone is considered to be the entire universe.\n"
			"\n"
			":param zone: the Zone of this Modifier\n"
			":param full: True to consider the Zone as a full object so that particles are not allowed to be within\n"))
		.def("setTrigger", &SPK::Modifier::setTrigger,
			":brief: Sets the trigger of this Modifier\n"
			"\n"
			"if the trigger is not one of the available triggers (see getAvailableTriggers()) for this Modifier,\n"
			"nothing happens and False is returned else the trigger is set and True is returned.\n"
			"\n"
			":param trigger: the trigger of this Modifier\n"
			":return: True if the trigger can be set, False otherwise\n")
		.def("setLocalToSystem", &SPK::Modifier::setLocalToSystem,
			":brief: Sets whether to consider this modifier local to a system or not\n"
			"\n"
			"A local modifier is transformed when its system is transformed, a non local one will not.\n"
			"\n"
			":param local: True to consider the modifier local, False not to\n"
			":since: 1.03.02\n")
		.def("isActive", &SPK::Modifier::isActive,
			":brief: Tells whether this Modifier is active or not\n"
			":return: True if this Modifier is active, False if is is inactive\n"
			":since: 1.03.00\n")
		.def("getZone", &SPK::Modifier::getZone,
			":brief: Gets the Zone of this Modifier\n"
			":return: the Zone of this Modifier\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getTrigger", &SPK::Modifier::getTrigger,
			":brief: Gets the trigger of this Modifier\n"
			":return: the trigger of this Modifier\n")
		.def("getAvailableTriggers", &SPK::Modifier::getAvailableTriggers,
			":brief: Gets a flag containing all the available triggers for this Modifier\n"
			":return: a flag containing all the available triggers for this Modifier\n")
		.def("isFullZone", &SPK::Modifier::isFullZone,
			":brief: Tells whether the Zone of this Modifier is considerered to be full or not\n"
			":return: True if the Zone of this Modifier is considerered to be full, False if not\n")
		.def("isLocalToSystem", &SPK::Modifier::isLocalToSystem,
			":brief: Tells whether this modifier is considered as being local to a system or not\n"
			"\n"
			"A local modifier is transformed when its system is transformed, a non local one will not.\n"
			"\n"
			":return: True if it is local, False if not\n"
			":since: 1.03.02\n")
		.def("findByName", &SPK::Modifier::findByName, boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		;
	{
		boost::python::enum_<SPK::ModifierTrigger>("ModifierTrigger", 
			":enum: ModifierTrigger\n"
			":brief: Constants defining the triggers for Modifier\n"
			)
			.value("ALWAYS", SPK::ALWAYS)
			.value("INSIDE_ZONE", SPK::INSIDE_ZONE)
			.value("OUTSIDE_ZONE", SPK::OUTSIDE_ZONE)
			.value("INTERSECT_ZONE", SPK::INTERSECT_ZONE)
			.value("ENTER_ZONE", SPK::ENTER_ZONE)
			.value("EXIT_ZONE", SPK::EXIT_ZONE)
			.export_values()
			;
	}
}

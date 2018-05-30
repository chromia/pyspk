// generate by pypp
// original source code: SPK_Registerable.h


#include <Core/SPK_Registerable.h>
#include <boost/python.hpp>


class RegisterableWrapper :
	public SPK::Registerable,
	public boost::python::wrapper<SPK::Registerable>
{
public:
	using Registerable = SPK::Registerable;

	std::string getClassName() const
	{
		return this->get_override("getClassName")();
	}

	SPK::Registerable * findByName(const std::string & name) {
		if (auto findByName = this->get_override("findByName")) {
			return findByName(name);
		}
		else {
			return Registerable::findByName(name);
		}
	}
	void registerChildren(bool registerAll) {
		if (auto registerChildren = this->get_override("_registerChildren")) {
			registerChildren(registerAll);
		}
		else {
			Registerable::registerChildren(registerAll);
		}
	}
	void copyChildren(const SPK::Registerable & object, bool createBase) {
		if (auto copyChildren = this->get_override("_copyChildren")) {
			copyChildren(object, createBase);
		}
		else {
			Registerable::copyChildren(object, createBase);
		}
	}
	void destroyChildren(bool keepChildren) {
		if (auto destroyChildren = this->get_override("_destroyChildren")) {
			destroyChildren(keepChildren);
		}
		else {
			Registerable::destroyChildren(keepChildren);
		}
	}

	Registerable * clone(bool createBase) const
	{
		return this->get_override("clone")(createBase);
	}
};

void init_SPK_Registerable_h() {

	boost::python::scope().attr("NO_ID") = 0;
	boost::python::scope().attr("NO_NAME") = "";

	boost::python::class_<RegisterableWrapper, boost::noncopyable>("Registerable")
		.def("setShared", &SPK::Registerable::setShared,
			":brief: Makes this Registerable shared or not\n\n"
			"\n"
			"By default, a Registerable is not shared\n\n"
			":param shared: True to make this Registerable shared, False to make it unshared\n")
		.def("setDestroyable", &SPK::Registerable::setDestroyable,
			":brief: Makes this Registerable destroyable or not\n"
			"\n"
			"A non destroyable Registerable cannot be destroyed internally.\n"
			"\n"
			":param destroyable: True to make this Registerable destroyable, False to make it undestroyable\n")
		.def("setName", &SPK::Registerable::setName,
			":brief: Sets the name of this Registerable\n"
			"\n"
			"The name is an easy to find registerable in a tree.\n"
			"See getName() and findByName(const std::string&)\n"
			"\n"
			"A constant NO_NAME exists to give no name to the registerable(an empty string)\n"
			"\n"
			":param name: the name of this registerable\n"
			":since: 1.05.00\n")
		.def("getSPKID", &SPK::Registerable::getSPKID,
			":brief: Gets the ID of this Registerable\n"
			"\n"
			"If this Registerable is unregistered, NO_ID is returned.\n"
			"Else an SPK_ID is returned. This ID uniquely identifies the Registerable.\n"
			"\n"
			":return: the ID of this Registerable or NO_ID if it is not registered\n"
			":since: 1.05.04\n")
		.def("getID", &SPK::Registerable::getID,
			":brief: Gets the ID of this Registerable\n"
			":return: the ID of this Registerable or NO_ID if it is not registered\n"
			":deprecated: 1.05.04 Use getSPKID instead\n")
		.def("getNbReferences", &SPK::Registerable::getNbReferences,
			":brief: Gets the number of references of this Registerable\n"
			"\n"
			"The number of references defines the number of times a registered Registerable\n"
			"is references within all the registered Registerable.\n"
			"\n"
			"0 is always returned for an unregistered Registerable.\n"
			"\n"
			":return: the number of references of this Registerable\n")
		.def("isRegistered", &SPK::Registerable::isRegistered,
			":brief: Tells whether this Registerable is registered or not\n"
			":return: True if this Registerable is registered, False if not\n")
		.def("isShared", &SPK::Registerable::isShared,
			":brief: Tells whether this Registerable is shared or not\n"
			":return: True if this Registerable is shared, False if not\n")
		.def("isDestroyable", &SPK::Registerable::isDestroyable,
			":brief: Tells whether this Registerable is destroyable or not\n"
			":return: True if this Registerable is destroyable, False if not\n")
		.def("getName", &SPK::Registerable::getName, 
			":brief: Gets the name of this registerable\n"
			"\n"
			"The name is an easy to find registerable in a tree.\n"
			"See setName(const std::string&) and findByName(const std::string&)\n"
			"\n"
			":return: the name of this registerable\n"
			":since: 1.05.00\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getClassName", boost::python::pure_virtual(&SPK::Registerable::getClassName),
			":brief: Gets the name of the class of the object\n"
			"\n"
			"This method is implemented in non abstract derived class of Registerable with the macro SPK_IMPLEMENT_REGISTERABLE(ClassName).\n"
			"\n"
			":return: the name of the class of the object as a std::string\n")
		.def("findByName", &SPK::Registerable::findByName,
			":brief: Finds a registerable with its name recursively from this registerable\n"
			"\n"
			"If the name is not found, NULL is returned.\n"
			"If the several objects with the same name exists, only the first one is returned.\n"
			"\n"
			"Note that the name of the registerable itself is already tested.\n"
			"\n"
			":param name: the name of the registerable to find\n"
			":return: the first registerable with that name within this registerable or NULL if none is found\n"
			":since: 1.05.00\n",
			boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		;
}

// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Factory.h


#include <Core/SPK_Factory.h>
#include <boost/python.hpp>


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SPKFactorydestroyOverloads0, SPK::SPKFactory::destroy, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SPKFactorydestroyOverloads1, SPK::SPKFactory::destroy, 1, 2)


void init_SPK_Factory_h() {
	boost::python::class_<SPK::SPKFactory, boost::noncopyable>("Factory", boost::python::no_init)
		.def("getInstance", &SPK::SPKFactory::getInstance, 
			":brief: Returns the unique instance of the SPKFactory\n"
			":return: the unique instance of the SPKFactory\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("destroyInstance", &SPK::SPKFactory::destroyInstance)
		.def("getNbObjects", &SPK::SPKFactory::getNbObjects,
			":brief: Returns the number of Regiterable objects registered in the SPKFactory\n"
			":return: the number of Regiterable objects registered in the SPKFactoty\n")
		.def("create", &SPK::SPKFactory::create,
			":brief: Creates a registered Registerable from the passed Registerable\n"
			":param base: The Registerable to create the new registered Registerable from\n"
			":return: the ID of the new registered object\n")
		.def("copy", static_cast<SPK::Registerable *(SPK::SPKFactory::*)(unsigned long)>(&SPK::SPKFactory::copy), 
			":brief: Creates a new Registerable object which is a copy of the object at the given ID\n"
			"\n"
			"If no Registerable with this ID is registered, None is returned.\n"
			"\n"
			":param ID: the ID of the Registerable to copy\n"
			":return: a registered copy of the Registerable or None if the passed ID is not registered\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("copy", static_cast<SPK::Registerable *(SPK::SPKFactory::*)(const SPK::Registerable *)>(&SPK::SPKFactory::copy),
			":brief: Creates a new Registerable object which is a copy of the object\n"
			"\n"
			"If the passed Registerable is None or not registered, None is returned.\n"
			"Note that this function call internally <i>copy(registerable.getSPKID())</i>.\n"
			"\n"
			":param registerable: the registered Registerable to copy\n"
			":return: a registered copy of the Registerable or None if the passed object is not registered\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("get", &SPK::SPKFactory::get,
			":brief: Gets the Registerable of the given ID\n"
			"\n"
			"If the ID is not registered, None is returned\n"
			"\n"
			":param ID: the ID of the Registerable to get\n"
			":return: the Registerable with the passed ID or None if the ID is not registered\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("destroy", static_cast<bool(SPK::SPKFactory::*)(unsigned long, bool)>(&SPK::SPKFactory::destroy), SPKFactorydestroyOverloads0(
			":brief: Destroys the Registerable with the given ID and all its destroyable children\n"
			"\n"
			"If the ID is not registered, nothing is destroyed and false is returned.\n"
			"\n"
			"The checkNbReferences boolean tells the factory if the number of references of the Registerable to be destroyed\n"
			"has to be checked.\n"
			"If set to True, the Registerable will be destroyed only if the number or references within the SPKFactory\n"
			"(ie in all registered object in the SPKFactory) is 0.\n"
			"If set to False, the Registerable will be destroyed in any case. Meaning that any reference within the SPKFactory\n"
			"becomes invalid.\n"
			"\n"
			":param ID: the ID of the Registerable to destroy\n"
			":param checkNbReferences: Frue to destroy only a Registerable with no references in the SPKFactory (safer), False not to perform the check\n"
			":return: True if the Registerable has been deleted, False if not\n"))
		.def("destroy", static_cast<bool(SPK::SPKFactory::*)(SPK::Registerable *, bool)>(&SPK::SPKFactory::destroy), SPKFactorydestroyOverloads1(
			":brief: Destroys the Registerable and all its destroyable children\n"
			"\n"
			"If the Registerable is None or is not registered, nothing is destroyed and False is returned.\n"
			"Note that this function call internally <i>destroy(registerable.getSPKID())</i>.\n"
			"\n"
			"see destroy(SPK_ID,bool) for more information.\n"
			"\n"
			":param registerable: the Registerable to destroy\n"
			":param checkNbReferences: True to destroy only a Registerable with no references in the SPKFactory (safer), False not to perform the check\n"
			":return: True if the Registerable has been deleted, False if not\n"))
		.def("destroyAll", &SPK::SPKFactory::destroyAll,
			":brief: Destroys all the registered Registerable in the SPKFactory")
		.def("trace", static_cast<void(SPK::SPKFactory::*)(unsigned long)>(&SPK::SPKFactory::trace),
			":brief: Trace information on the Registerable with the given ID\n"
			":param ID: the ID of the Registerable to trace\n")
		.def("trace", static_cast<void(SPK::SPKFactory::*)(const SPK::Registerable *)>(&SPK::SPKFactory::trace),
			":brief: Trace information on the Registerable\n"
			"\n"
			"Note that this function call internally <i>trace(registerable.getSPKID())</i>.\n"
			"\n"
			":param registerable: the Registerable to trace\n")
		.def("traceAll", &SPK::SPKFactory::traceAll,
			":brief: Trace information on all the registered Registerable within the SPKFactory")
		.def("findByName", &SPK::SPKFactory::findByName, 
			":brief: Finds a registerable by name in the factory\n"
			"\n"
			"Note that this method only checks registerables in the SPKFactory.\n"
			"This method does not call the Registerable::findByName(const string&) of the registerables to check recursively.\n"
			"\n"
			":param name: the name of the registerable to find in the factory\n"
			":return: the first registerable with that name or None of none is found\n"
			":since: 1.05.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.staticmethod("getInstance")
		.staticmethod("destroyInstance")
		;
}

// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_NormalEmitter.h


#include <Extensions/Emitters/SPK_NormalEmitter.h>
#include <boost/python.hpp>


class NormalEmitterWrapper :
    public SPK::NormalEmitter,
    public boost::python::wrapper<SPK::NormalEmitter>
{
public:
    using SPK::NormalEmitter::NormalEmitter;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return NormalEmitter::getClassName();
        }
    }
    SPK::Registerable * findByName (const std::string & name) {
        if ( auto findByName = this->get_override("findByName") ) {
            return findByName(name);
        } else {
            return NormalEmitter::findByName(name);
        }
    }
    void registerChildren (bool registerAll) {
        if ( auto registerChildren = this->get_override("_registerChildren") ) {
            registerChildren(registerAll);
        } else {
            NormalEmitter::registerChildren(registerAll);
        }
    }
    void copyChildren (const SPK::Registerable & object, bool createBase) {
        if ( auto copyChildren = this->get_override("_copyChildren") ) {
            copyChildren(object, createBase);
        } else {
            NormalEmitter::copyChildren(object, createBase);
        }
    }
    void destroyChildren (bool keepChildren) {
        if ( auto destroyChildren = this->get_override("_destroyChildren") ) {
            destroyChildren(keepChildren);
        } else {
            NormalEmitter::destroyChildren(keepChildren);
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::NormalEmitter::create, 0, 2)

void init_SPK_NormalEmitter_h() {
    boost::python::class_<NormalEmitterWrapper, boost::python::bases<SPK::Emitter>, std::shared_ptr<SPK::NormalEmitter>>("NormalEmitter",
		boost::python::init<boost::python::optional<SPK::Zone *, bool>>(
			":brief: Constructor of NormalEmitter\n"
			":param normalZone: the Zone used to compute normals (None to used the Emitter Zone)\n"
			":param inverted: True to invert the normals, False otherwise\n"))
        .def("getClassName", &SPK::NormalEmitter::getClassName)
        .def("create", &SPK::NormalEmitter::create, create_overload(
			":brief: Creates and registers a new NormalEmitter\n"
			":param normalZone: the Zone used to compute normals (None to used the Emitter Zone)\n"
			":param inverted: True to invert the normals, False otherwise\n"
			":return: A new registered NormalEmitter\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .def("setInverted", &SPK::NormalEmitter::setInverted,
			":brief: Sets whether normals are inverted or not\n"
			":param: inverted True to use inverted normals, False not to\n")
        .def("setNormalZone", &SPK::NormalEmitter::setNormalZone,
			":brief: the Zone used to compute normals\n"
			"\n"
			"Note that if the normal zone is None, the Emitter Zone is used.\n"
			"\n"
			":param zone: the Zone used to compute normals (None to used the Emitter Zone)\n")
        .def("isInverted", &SPK::NormalEmitter::isInverted,
			":brief: Tells whether normals are inverted for this NormalEmitter\n"
			":return: True if normals are inverted, False if not\n")
        .def("getNormalZone", &SPK::NormalEmitter::getNormalZone,
			":brief: Gets the normal Zone of this NormalEmitter\n"
			":return: the normal Zone of this NormalEmitter\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
        .def("findByName", &SPK::NormalEmitter::findByName, boost::python::return_value_policy<boost::python::return_opaque_pointer>())
        .staticmethod("create")
        ;
}

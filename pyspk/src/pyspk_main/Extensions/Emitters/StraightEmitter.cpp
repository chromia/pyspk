// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_StraightEmitter.h


#include <Extensions/Emitters/SPK_StraightEmitter.h>
#include <boost/python.hpp>


class StraightEmitterWrapper :
    public SPK::StraightEmitter,
    public boost::python::wrapper<SPK::StraightEmitter>
{
public:
    using SPK::StraightEmitter::StraightEmitter;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return StraightEmitter::getClassName();
        }
    }
    void innerUpdateTransform () {
        if ( auto innerUpdateTransform = this->get_override("_innerUpdateTransform") ) {
            innerUpdateTransform();
        } else {
            StraightEmitter::innerUpdateTransform();
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::StraightEmitter::create, 0, 1)

void init_SPK_StraightEmitter_h() {
    boost::python::class_<StraightEmitterWrapper, boost::python::bases<SPK::Emitter>, std::shared_ptr<SPK::StraightEmitter>>("StraightEmitter", 
		boost::python::init<boost::python::optional<const SPK::Vector3D &>>(
			":brief: The constructor of StraightEmitter\n"
			":param direction: the direction of the StraighEmitter\n"
			))
        .def("getClassName", &SPK::StraightEmitter::getClassName)
        .def("create", &SPK::StraightEmitter::create, create_overload(
			":brief: Creates and registers a new StraightEmitter\n"
			":param direction: the direction of the StraighEmitter\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .def("setDirection", &SPK::StraightEmitter::setDirection,
			":brief: Sets the direction of this StraightEmitter\n"
			"\n"
			"Note that it is not necessary to provide a normalized Vector3D.\n"
			"This Vector3D only indicates a direction, its norm does not matter.\n"
			"\n"
			":param direction: the direction of this StraightEmitter\n")
        .def("getDirection", &SPK::StraightEmitter::getDirection,
			":brief: Gets the direction of this StraightEmitter\n"
			":return: the direction of this StraightEmitter\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getTransformedDirection", &SPK::StraightEmitter::getTransformedDirection,
			":brief: Gets the transformed direction of this StraightEmitter\n"
			":return: the transformed direction of this StraightEmitter\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .staticmethod("create")
        ;
}

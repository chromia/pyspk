// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Vortex.h


#include <Extensions/Modifiers/SPK_Vortex.h>
#include <boost/python.hpp>


class VortexWrapper :
    public SPK::Vortex,
    public boost::python::wrapper<SPK::Vortex>
{
public:
    using SPK::Vortex::Vortex;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return Vortex::getClassName();
        }
    }
    void innerUpdateTransform () {
        if ( auto innerUpdateTransform = this->get_override("_innerUpdateTransform") ) {
            innerUpdateTransform();
        } else {
            Vortex::innerUpdateTransform();
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Vortex::create, 0, 4)

void init_SPK_Vortex_h() {
    boost::python::class_<VortexWrapper, boost::python::bases<SPK::Modifier>, std::shared_ptr<SPK::Vortex>>("Vortex",
		boost::python::init<boost::python::optional<const SPK::Vector3D &, const SPK::Vector3D &, float, float>>(
			":brief: Constructor of vortex\n"
			":param position: the position of the eye\n"
			":param direction: the direction of the eye\n"
			":param rotationSpeed: the speed of rotation\n"
			":param attractionSpeed: the speed of attraction\n"))
        .def("getClassName", &SPK::Vortex::getClassName)
        .def("create", &SPK::Vortex::create, create_overload(
			":brief: Creates and registers a new Vortex\n"
			":param position: the position of the eye\n"
			":param direction: the direction of the eye\n"
			":param rotationSpeed: the speed of rotation\n"
			":param attractionSpeed: the speed of attraction\n"
			":return: a new registered vortex\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .def("setPosition", &SPK::Vortex::setPosition,
			":brief: Sets the position of the eye of the vortex\n"
			"\n"
			"The eye of the vortex is an infinite line defined by a position and a direction.\n"
			"\n"
			":param position: the position of the eye of the vortex\n")
        .def("setDirection", &SPK::Vortex::setDirection,
			":brief: Sets the direction of the eye of the vortex\n"
			"\n"
			"The eye of the vortex is an infinite line defined by a position and a direction.\n"
			"Note that the direction is normalized internally.\n"
			"\n"
			":param direction: the direction of the eye of the vortex\n")
        .def("setRotationSpeed", &SPK::Vortex::setRotationSpeed,
			":brief: Sets the rotation speed of the vortex\n"
			"\n"
			"The rotation speed can either be in units per second or in radians per second (angular).\n"
			"In the case the rotation speed is angular, all particles are rotated around the eye at the same speed.\n"
			"In the other case, the more particles are close to the eye, the faster they rotate.\n"
			"\n"
			"The sens of rotation is defined by the sens of the speed (and depends also on whether we are in a right or left handed system).\n"
			"A speed of 0 means no rotation.\n"
			"\n"
			":param rotationSpeed: the speed of rotation (either in units per unit of time or in radians per unit of time dependent on the second parameter)\n"
			":param angular:  True to have the rotation in radians per unit of time, False to have it in unit per unit of time.\n")
        .def("setAttractionSpeed", &SPK::Vortex::setAttractionSpeed,
			":brief: Sets the attraction speed of the vortex\n"
			"\n"
			"The attraction speed is the speed that move particles towards the eye of the vortex.\n"
			"It can either be constant or linear (function of the distance of the particle from the eye).\n"
			"\n"
			"A constant attraction speed is defined in units per unit of time,\n"
			"a linear attraction speed is also defined in units per unit of time and the value is the speed at a distance of 1.0 from the eye.\n"
			"\n"
			"A negative speed means particles are repelled by the eye.\n"
			"A speed of 0.0 means particles are neither attracted nor repelled by the eye of the vortex.\n"
			"\n"
			":param attractionSpeed: the attraction speed of the vortex in units per unit of time\n"
			":param linear: True to set the speed function of the distance from the eye, False to set it constant\n")
        .def("setEyeRadius", &SPK::Vortex::setEyeRadius,
			":brief: Sets the eye radius\n"
			"\n"
			"Note that an negative radius is inverted internally\n"
			"\n"
			":param eyeRadius: the eye radius\n")
        .def("enableParticleKilling", &SPK::Vortex::enableParticleKilling,
			":brief: Tells whether particles is the eye must be killed or not\n"
			":param kill: True to kill particles in the eye, False not to\n")
        .def("getPosition", &SPK::Vortex::getPosition,
			":brief: Gets the position of the eye\n"
			":return: the position of the eye\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getDirection", &SPK::Vortex::getDirection,
			":brief: Gets the direction of the eye\n"
			":return: the direction of the eye (normalized)\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getTransformedPosition", &SPK::Vortex::getTransformedPosition,
			":brief: Gets the transformed position of the eye\n"
			":return: the transformed position of the eye\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getTransformedDirection", &SPK::Vortex::getTransformedDirection,
			":brief: Gets the transformed direction of the eye\n"
			":return: the transformed direction of the eye (normalized)\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getRotationSpeed", &SPK::Vortex::getRotationSpeed,
			":brief: Gets the rotation speed\n"
			":return: the rotation speed\n")
        .def("getAttractionSpeed", &SPK::Vortex::getAttractionSpeed,
			":brief: Gets the attraction speed\n"
			":return: the attraction speed\n")
        .def("isRotationSpeedAngular", &SPK::Vortex::isRotationSpeedAngular,
			":brief: Tells whether rotation speed is angular\n"
			":return: True if rotation speed is angular, False if not\n")
        .def("isAttractionSpeedLinear", &SPK::Vortex::isAttractionSpeedLinear,
			":brief: Tells whether attraction speed is function of the distance from the eye\n"
			":return: True if attraction speed is linear, False if it is constant\n")
        .def("getEyeRadius", &SPK::Vortex::getEyeRadius,
			":brief: Returns the eye radius\n"
			":return: the eye radius\n")
        .def("isParticleKillingEnabled", &SPK::Vortex::isParticleKillingEnabled,
			":brief: Tells whether particles are killed when in the eye\n"
			":return: True if particles are killed in the eye, False if not\n")
        .staticmethod("create")
        ;
}

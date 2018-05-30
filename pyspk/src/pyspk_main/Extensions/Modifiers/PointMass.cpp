// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_PointMass.h


#include <Extensions/Modifiers/SPK_PointMass.h>
#include <boost/python.hpp>


class PointMassWrapper :
    public SPK::PointMass,
    public boost::python::wrapper<SPK::PointMass>
{
public:
    using SPK::PointMass::PointMass;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return PointMass::getClassName();
        }
    }
    void innerUpdateTransform () {
        if ( auto innerUpdateTransform = this->get_override("_innerUpdateTransform") ) {
            innerUpdateTransform();
        } else {
            PointMass::innerUpdateTransform();
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::PointMass::create, 0, 4)

void init_SPK_PointMass_h() {
    boost::python::class_<PointMassWrapper, boost::python::bases<SPK::Modifier>, std::shared_ptr<SPK::PointMass>>("PointMass",
		boost::python::init<boost::python::optional<SPK::Zone *, SPK::ModifierTrigger, float, float>>(
			":brief: Constructor of PointMass\n"
			":param zone: the Zone of the PointMass\n"
			":param trigger: the trigger of the PointMass\n"
			":param mass: the mass of the PointMass\n"
			":param minDistance: the minimum distance of the PointMass\n"))
        .def("getClassName", &SPK::PointMass::getClassName)
        .def("create", &SPK::PointMass::create, create_overload(
			":brief: Creates and registers a new PointMass\n"
			":param zone: the Zone of the PointMass\n"
			":param trigger: the trigger of the PointMass\n"
			":param mass: the mass of the PointMass\n"
			":param minDistance: the minimum distance of the PointMass\n"
			":return: A new registered PointMass\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .def("setPosition", &SPK::PointMass::setPosition,
			":brief: Sets the delta position from the position of the zone (or origin if no zone set)\n"
			":param pos: the delta position\n"
			":since: 1.03.02\n")
        .def("setMass", &SPK::PointMass::setMass,
			":brief: Sets the mass of this PointMass\n"
			"\n"
			"The mass defines the strenght of the attraction. The more the mass, the stronger the attraction.\n"
			"A position mass will result into an attraction while a negative mass will result into a repulsion.\n"
			"Moreover a mass equal to 0 make the PointMass have no effect.\n"
			"\n"
			":param mass: the mass of this PointMass\n")
        .def("setMinDistance", &SPK::PointMass::setMinDistance,
			":brief: Sets the minimum distance of this PointMass\n"
			"\n"
			"The minimum distance of the PointMass is the minimum distance that can be considered to compute the force implied by the PointMass.\n"
			"If a distance between a Particle and a PointMass is inferior to the minimum distance of the PointMass, the distance is clamped to the minimum distance.\n"
			"This avoids forces that approaching the infinity with Particle getting very close to the PointMass.\n"
			"\n"
			":param minDistance: the minimum distance of this PointMass\n")
        .def("getPosition", &SPK::PointMass::getPosition,
			":brief: Gets the delta position\n"
			":return: the delta position\n"
			":since: 1.03.02\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getTransformedPosition", &SPK::PointMass::getTransformedPosition,
			":brief: Gets the transformed delta position\n"
			":return: the transformed delta position\n"
			":since: 1.03.02\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getMass", &SPK::PointMass::getMass,
			":brief: Gets the mass of this PointMass\n"
			":return: the mass of this PointMass\n")
        .def("getMinDistance", &SPK::PointMass::getMinDistance,
			":brief: Gets the minimum distance of this PointMass\n"
			":return: the minimum distance of this PointMass\n")
        .staticmethod("create")
        ;
}

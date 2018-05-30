// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_LinearForce.h


#include <Extensions/Modifiers/SPK_LinearForce.h>
#include <boost/python.hpp>


class LinearForceWrapper :
    public SPK::LinearForce,
    public boost::python::wrapper<SPK::LinearForce>
{
public:
    using SPK::LinearForce::LinearForce;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return LinearForce::getClassName();
        }
    }
    void innerUpdateTransform () {
        if ( auto innerUpdateTransform = this->get_override("_innerUpdateTransform") ) {
            innerUpdateTransform();
        } else {
            LinearForce::innerUpdateTransform();
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::LinearForce::create, 0, 5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(LinearForcesetFactorOverloads0, SPK::LinearForce::setFactor, 1, 2)

void init_SPK_LinearForce_h() {
    boost::python::class_<LinearForceWrapper, boost::python::bases<SPK::Modifier>, std::shared_ptr<SPK::LinearForce>>("LinearForce",
		boost::python::init<boost::python::optional<SPK::Zone *, SPK::ModifierTrigger, const SPK::Vector3D &, SPK::ForceFactor, SPK::ModelParam>>(
			":brief: Default constructor for LinearForce\n"
			":param zone: the Zone of the LinearForce or None if no Zone\n"
			":param trigger: the trigger of the LinearForce\n"
			":param force: the force of the LinearForce\n"
			":param type: the type of multiplier for the factor\n"
			":param param: the parameter used as the factor (if type != FACTOR_NONE)\n"))
        .def("getClassName", &SPK::LinearForce::getClassName)
        .def("create", &SPK::LinearForce::create, create_overload(
			":brief: Creates and registers a new LinearForce\n"
			":param zone: the Zone of the LinearForce or None if no Zone\n"
			":param trigger: the trigger of the LinearForce\n"
			":param force: the force of the LinearForce\n"
			":param type: the type of multiplier for the factor\n"
			":param param: the parameter used as the factor (if type != FACTOR_NONE)\n"
			":return: A new registered LinearForce\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .def("setForce", &SPK::LinearForce::setForce,
			":brief: Sets the force vector\n"
			":param force: the force vector\n"
			":since: 1.03.02\n")
        .def("setFactor", &SPK::LinearForce::setFactor, LinearForcesetFactorOverloads0(
			":brief: Sets the factor type to apply to the force\n"
			":param type: the type of multiplier for the factor\n"
			":param param: the parameter of the Particle to use as the factor\n"))
        .def("getForce", &SPK::LinearForce::getForce,
			":brief: Gets the force vector\n"
			":return: the force vector\n"
			":since: 1.03.02\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getTransformedForce", &SPK::LinearForce::getTransformedForce,
			":brief: Gets the transformed force vector\n"
			":return: the transformed force vector\n"
			":since: 1.03.02\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("getFactorType", &SPK::LinearForce::getFactorType,
			":brief: Gets the factor multiplier of this LinearForce\n"
			":return: the factor multiplier of this LinearForce\n")
        .def("getFactorParam", &SPK::LinearForce::getFactorParam,
			":brief: Gets the factor parameter of this LinearForce\n"
			":return: the factor parameter of this LinearForce\n")
        .staticmethod("create")
        ;
    {
        boost::python::enum_<SPK::ForceFactor>("ForceFactor", 
			":enum: ForceFactor\n"
			":brief: An enum defining the way a factor is applied to a force\n"
			":since: 1.03.00\n")
            .value("FACTOR_NONE", SPK::FACTOR_NONE)
            .value("FACTOR_LINEAR", SPK::FACTOR_LINEAR)
            .value("FACTOR_SQUARE", SPK::FACTOR_SQUARE)
            .export_values()
            ;
    }
}

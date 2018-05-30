// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_SphericEmitter.h


#include <Extensions/Emitters/SPK_SphericEmitter.h>
#include <boost/python.hpp>


class SphericEmitterWrapper :
	public SPK::SphericEmitter,
	public boost::python::wrapper<SPK::SphericEmitter>
{
public:
	using SPK::SphericEmitter::SphericEmitter;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return SphericEmitter::getClassName();
		}
	}
	void innerUpdateTransform() {
		if (auto innerUpdateTransform = this->get_override("_innerUpdateTransform")) {
			innerUpdateTransform();
		}
		else {
			SphericEmitter::innerUpdateTransform();
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::SphericEmitter::create, 0, 3)

void init_SPK_SphericEmitter_h() {
	boost::python::class_<SphericEmitterWrapper, boost::python::bases<SPK::Emitter>, std::shared_ptr<SPK::SphericEmitter>>("SphericEmitter", boost::python::init<boost::python::optional<const SPK::Vector3D &, float, float>>(
			":brief: Constructor of SphericEmitter\n"
			":param direction: the direction of the SphericEmitter\n"
			":param angleA: the first angle in radians of the SphericEmitter\n"
			":param angleB: the second angle in radians of the SphericEmitter\n"))
		.def("getClassName", &SPK::SphericEmitter::getClassName)
		.def("create", &SPK::SphericEmitter::create, create_overload(
			":brief: Creates and registers a new SphericEmitter\n"
			":param direction: the direction of the SphericEmitter\n"
			":param angleA: the first angle in radians of the SphericEmitter\n"
			":param angleB: the second angle in radians of the SphericEmitter\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setDirection", &SPK::SphericEmitter::setDirection,
			":brief: Sets the direction of this SphericEmitter\n"
			"\n"
			"Note that it is not necessary to provide a normalized Vector3D.\n"
			"This Vector3D only indicates a direction, its norm does not matter.\n"
			"\n"
			":param direction: the direction of this SphericEmitter\n")
		.def("setAngles", &SPK::SphericEmitter::setAngles,
			":brief: Sets the angles of this SphericEmitter\n"
			"\n"
			"Note that angles are clamped between 0 and 2 * PI\n"
			"AngleA does not have to be inferior to angleB, it has no importance as angles are sorted within the method.\n"
			"\n"
			":param angleA: the first angle in radians of this SphericEmitter\n"
			":param angleB: the second angle in radians of this SphericEmitter\n")
		.def("getDirection", &SPK::SphericEmitter::getDirection,
			":brief: Gets the direction of this SphericEmitter\n"
			":return: the direction of this SphericEmitter\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getTransformedDirection", &SPK::SphericEmitter::getTransformedDirection,
			":brief: Gets the direction of this SphericEmitter\n"
			":return: the direction of this SphericEmitter\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getAngleMin", &SPK::SphericEmitter::getAngleMin,
			":brief: Gets the minimum angle of this SphericEmitter\n"
			":return: the minimum angle of this SphericEmitter\n")
		.def("getAngleMax", &SPK::SphericEmitter::getAngleMax,
			":brief: Gets the maximum angle of this SphericEmitter\n"
			":return: the maximum angle of this SphericEmitter\n")
		.staticmethod("create")
		;
}

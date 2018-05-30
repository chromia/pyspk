// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Obstacle.h


#include <Extensions/Modifiers/SPK_Obstacle.h>
#include <boost/python.hpp>


class ObstacleWrapper :
	public SPK::Obstacle,
	public boost::python::wrapper<SPK::Obstacle>
{
public:
	using SPK::Obstacle::Obstacle;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Obstacle::getClassName();
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Obstacle::create, 0, 4)

void init_SPK_Obstacle_h() {
	boost::python::class_<ObstacleWrapper, boost::python::bases<SPK::Modifier>, std::shared_ptr<SPK::Obstacle>>("Obstacle", boost::python::init<boost::python::optional<SPK::Zone *, SPK::ModifierTrigger, float, float>>(
		":brief: Constructor of Obstacle\n"
		":param zone: the Zone of the Obstacle\n"
		":param trigger: the trigger of the Destructor\n"
		":param bouncingRatio: the bouncingRatio of the Obstacle\n"
		":param friction: the friction of the Obstacle\n"))
		.def("getClassName", &SPK::Obstacle::getClassName)
		.def("create", &SPK::Obstacle::create, create_overload(
			":brief: Creates and registers a new Obstacle\n"
			":param zone: the Zone of the Obstacle\n"
			":param trigger: the trigger of the Destructor\n"
			":param bouncingRatio: the bouncingRatio of the Obstacle\n"
			":param friction: the friction of the Obstacle\n"
			":return: A new registered Obstacle\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setBouncingRatio", &SPK::Obstacle::setBouncingRatio,
			":brief: Sets the bouncing ratio of this Obstacle\n"
			"\n"
			"The bouncing ratio is the multiplier applied to the normal component of the rebound.\n"
			"\n"
			":param bouncingRatio: the bouncing ratio of this Obstacle\n")
		.def("setFriction", &SPK::Obstacle::setFriction,
			":brief: Sets the friction of this Obstacle\n"
			"\n"
			"The bouncing ratio is the multiplier applied to the tangent component of the rebound.\n"
			"\n"
			":param friction: the friction of this Obstacle\n")
		.def("getBouncingRatio", &SPK::Obstacle::getBouncingRatio,
			":brief: Gets the bouncing ratio of this Obstacle\n"
			":return: the bouncing ratio of this Obstacle\n")
		.def("getFriction", &SPK::Obstacle::getFriction,
			":brief: Gets the friction of this Obstacle\n"
			":return: the friction of this Obstacle\n")
		.staticmethod("create")
		;
}

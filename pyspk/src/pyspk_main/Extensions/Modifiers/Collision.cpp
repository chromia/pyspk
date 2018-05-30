// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Collision.h


#include <Extensions/Modifiers/SPK_Collision.h>
#include <boost/python.hpp>


class CollisionWrapper :
    public SPK::Collision,
    public boost::python::wrapper<SPK::Collision>
{
public:
    using SPK::Collision::Collision;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return Collision::getClassName();
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Collision::create, 0, 2)

void init_SPK_Collision_h() {
    boost::python::class_<CollisionWrapper, boost::python::bases<SPK::Modifier>, std::shared_ptr<SPK::Collision>>("Collision",
		boost::python::init<boost::python::optional<float, float>>(
			":brief: Constructor of the Collision modifier\n"
			":param scale: the scale of the particles\n"
			":param elasticity: the elasticity of the collisions\n"))
        .def("getClassName", &SPK::Collision::getClassName)
        .def("create", &SPK::Collision::create, create_overload(
			":brief: Creates and registers a new Collision\n"
			":param scale: the scale of the particles\n"
			":param elasticity: the elasticity of the collisions\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .def("setScale", &SPK::Collision::setScale,
			":brief: Sets the scale of particles to compute their radius\n"
			"\n"
			"The radius of a particle is computed as follows :\n"
			"<i>radius = size * scale * 0.5</i>\n"
			"\n"
			":param scale: the scale of the particles\n")
        .def("setElasticity", &SPK::Collision::setElasticity,
			":brief: Sets the elasticity of the collisions\n"
			"\n"
			"The elasticity of the collisions refers to the coefficient of restitution (also called bounciness).\n"
			"See the class description for more information.\n"
			"\n"
			":param elasticity: the elasticity of the collisions\n")
        .def("getElasticity", &SPK::Collision::getElasticity,
			":brief: Gets the elasticity of the collisions\n"
			":return: the elasticity of the collisions\n")
        .def("getScale", &SPK::Collision::getScale,
			":brief: Gets the scale applied on particle radius\n"
			":return: the scale\n")
        .staticmethod("create")
        ;
}

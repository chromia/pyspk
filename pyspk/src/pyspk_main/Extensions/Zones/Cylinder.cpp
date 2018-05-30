// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Cylinder.h


#include <Extensions/Zones/SPK_Cylinder.h>
#include <Core/SPK_Particle.h>
#include <boost/python.hpp>

class CylinderWrapper :
	public SPK::Cylinder,
	public boost::python::wrapper<SPK::Cylinder>
{
public:
	using SPK::Cylinder::Cylinder;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Cylinder::getClassName();
		}
	}
	void generatePosition(SPK::Particle & particle, bool full) const {
		if (auto generatePosition = this->get_override("generatePosition")) {
			generatePosition(particle, full);
		}
		else {
			Cylinder::generatePosition(particle, full);
		}
	}
	bool contains(const SPK::Vector3D & v) const {
		if (auto contains = this->get_override("contains")) {
			return contains(v);
		}
		else {
			return Cylinder::contains(v);
		}
	}
	bool intersects(const SPK::Vector3D & v0, const SPK::Vector3D & v1, SPK::Vector3D * intersection, SPK::Vector3D * normal) const {
		if (auto intersects = this->get_override("intersects")) {
			return intersects(v0, v1, intersection, normal);
		}
		else {
			return Cylinder::intersects(v0, v1, intersection, normal);
		}
	}
	void moveAtBorder(SPK::Vector3D & v, bool inside) const {
		if (auto moveAtBorder = this->get_override("moveAtBorder")) {
			moveAtBorder(v, inside);
		}
		else {
			Cylinder::moveAtBorder(v, inside);
		}
	}
	SPK::Vector3D computeNormal(const SPK::Vector3D & point) const {
		if (auto computeNormal = this->get_override("computeNormal")) {
			return computeNormal(point);
		}
		else {
			return Cylinder::computeNormal(point);
		}
	}
	void innerUpdateTransform() {
		if (auto innerUpdateTransform = this->get_override("_innerUpdateTransform")) {
			innerUpdateTransform();
		}
		else {
			Cylinder::innerUpdateTransform();
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Cylinder::create, 0, 4)

void init_SPK_Cylinder_h() {
	boost::python::class_<CylinderWrapper, boost::python::bases<SPK::Zone>,
		std::shared_ptr<SPK::Cylinder>>("Cylinder", boost::python::init<boost::python::optional<const SPK::Vector3D &, const SPK::Vector3D &, float, float>>(
			":brief: Constructor of cylinder\n"
			":param position: the position of the cylinder\n"
			":param direction: the direction of the cylinder\n"
			":param radius: the  radius of the cylinder\n"))
		.def("getClassName", &SPK::Cylinder::getClassName)
		.def("create", &SPK::Cylinder::create, create_overload(
			":brief: Creates and registers a new cylinder\n"
			":param position: the position of the cylinder\n"
			":param direction: the direction of the cylinder\n"
			":param radius: the  radius of the cylinder\n"
			":return: a new registered cylinder\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setDirection", &SPK::Cylinder::setDirection,
			":brief: Sets the direction of the cylinder\n"
			"\n"
			"Note that the direction is normalized internally\n"
			"\n"
			":param direction: the direction of the cylinder\n")
		.def("setRadius", &SPK::Cylinder::setRadius,
			":brief: Sets the radius of this cylinder\n"
			"\n"
			"A radius cannot be negative.\n"
			"Note that negative radius are inverted internally\n"
			"\n"
			":param radius: the radius of this cylinder\n")
		.def("setLength", &SPK::Cylinder::setLength,
			":brief: Sets the length of this cylinder\n"
			"\n"
			"A length cannot be negative.\n"
			"Note that negative length are inverted internally\n"
			"\n"
			":param length: the length of this cylinder\n")
		.def("getDirection", &SPK::Cylinder::getDirection,
			":brief: Gets the direction of this cylinder\n"
			":return: the direction of this cylinder\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getTransformedDirection", &SPK::Cylinder::getTransformedDirection,
			":brief: Gets the transformed direction of this cylinder\n"
			":return: the transformed direction of this cylinder\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getRadius", &SPK::Cylinder::getRadius,
			":brief: Gets the radius of this cylinder\n"
			":return: the radius of this cylinder\n")
		.def("getLength", &SPK::Cylinder::getLength,
			":brief: Gets the length of this cylinder\n"
			":return: the length of this cylinder\n")
		.def("generatePosition", &SPK::Cylinder::generatePosition)
		.def("contains", &SPK::Cylinder::contains)
		.def("intersects", &SPK::Cylinder::intersects)
		.def("moveAtBorder", &SPK::Cylinder::moveAtBorder)
		.def("computeNormal", &SPK::Cylinder::computeNormal)
		.staticmethod("create")
		;
}

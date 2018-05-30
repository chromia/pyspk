// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Sphere.h


#include <Extensions/Zones/SPK_Sphere.h>
#include <Core/SPK_Particle.h>
#include <boost/python.hpp>


class SphereWrapper :
	public SPK::Sphere,
	public boost::python::wrapper<SPK::Sphere>
{
public:
	using SPK::Sphere::Sphere;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Sphere::getClassName();
		}
	}
	void generatePosition(SPK::Particle & particle, bool full) const {
		if (auto generatePosition = this->get_override("generatePosition")) {
			generatePosition(particle, full);
		}
		else {
			Sphere::generatePosition(particle, full);
		}
	}
	bool contains(const SPK::Vector3D & v) const {
		if (auto contains = this->get_override("contains")) {
			return contains(v);
		}
		else {
			return Sphere::contains(v);
		}
	}
	bool intersects(const SPK::Vector3D & v0, const SPK::Vector3D & v1, SPK::Vector3D * intersection, SPK::Vector3D * normal) const {
		if (auto intersects = this->get_override("intersects")) {
			return intersects(v0, v1, intersection, normal);
		}
		else {
			return Sphere::intersects(v0, v1, intersection, normal);
		}
	}
	void moveAtBorder(SPK::Vector3D & v, bool inside) const {
		if (auto moveAtBorder = this->get_override("moveAtBorder")) {
			moveAtBorder(v, inside);
		}
		else {
			Sphere::moveAtBorder(v, inside);
		}
	}
	SPK::Vector3D computeNormal(const SPK::Vector3D & point) const {
		if (auto computeNormal = this->get_override("computeNormal")) {
			return computeNormal(point);
		}
		else {
			return Sphere::computeNormal(point);
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Sphere::create, 0, 2)

void init_SPK_Sphere_h() {
	boost::python::class_<SphereWrapper, boost::python::bases<SPK::Zone>, std::shared_ptr<SPK::Sphere>>("Sphere",
		boost::python::init<boost::python::optional<const SPK::Vector3D &, float>>(
			":brief: Constructor of Sphere\n"
			":param position: position of the center of the Sphere\n"
			":param radius: radius of the Sphere\n"))
		.def("getClassName", &SPK::Sphere::getClassName)
		.def("create", &SPK::Sphere::create, create_overload(
			":brief: Creates and registers a new Sphere\n"
			":param position: position of the center of the Sphere\n"
			":param radius: radius of the Sphere\n"
			":return: A new registered Sphere\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setRadius", &SPK::Sphere::setRadius,
			":brief: Sets the radius of this Sphere\n"
			"\n"
			"A negative radius will be clamped to 0.0 and the Sphere will therefore acts as a Point.\n"
			"\n"
			":param radius: the radius of this Sphere\n")
		.def("getRadius", &SPK::Sphere::getRadius,
			":brief: Gets the radius of this Sphere\n"
			":return: the radius of this Sphere\n")
		.def("generatePosition", &SPK::Sphere::generatePosition)
		.def("contains", &SPK::Sphere::contains)
		.def("intersects", &SPK::Sphere::intersects)
		.def("moveAtBorder", &SPK::Sphere::moveAtBorder)
		.def("computeNormal", &SPK::Sphere::computeNormal)
		.staticmethod("create")
		;
}

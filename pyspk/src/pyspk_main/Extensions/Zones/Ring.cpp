// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Ring.h


#include <Extensions/Zones/SPK_Ring.h>
#include <Core/SPK_Particle.h>
#include <boost/python.hpp>


class RingWrapper :
	public SPK::Ring,
	public boost::python::wrapper<SPK::Ring>
{
public:
	using SPK::Ring::Ring;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Ring::getClassName();
		}
	}
	void generatePosition(SPK::Particle & particle, bool full) const {
		if (auto generatePosition = this->get_override("generatePosition")) {
			generatePosition(particle, full);
		}
		else {
			Ring::generatePosition(particle, full);
		}
	}
	bool contains(const SPK::Vector3D & v) const {
		if (auto contains = this->get_override("contains")) {
			return contains(v);
		}
		else {
			return Ring::contains(v);
		}
	}
	bool intersects(const SPK::Vector3D & v0, const SPK::Vector3D & v1, SPK::Vector3D * intersection, SPK::Vector3D * normal) const {
		if (auto intersects = this->get_override("intersects")) {
			return intersects(v0, v1, intersection, normal);
		}
		else {
			return Ring::intersects(v0, v1, intersection, normal);
		}
	}
	void moveAtBorder(SPK::Vector3D & v, bool inside) const {
		if (auto moveAtBorder = this->get_override("moveAtBorder")) {
			moveAtBorder(v, inside);
		}
		else {
			Ring::moveAtBorder(v, inside);
		}
	}
	SPK::Vector3D computeNormal(const SPK::Vector3D & point) const {
		if (auto computeNormal = this->get_override("computeNormal")) {
			return computeNormal(point);
		}
		else {
			return Ring::computeNormal(point);
		}
	}
	void innerUpdateTransform() {
		if (auto innerUpdateTransform = this->get_override("_innerUpdateTransform")) {
			innerUpdateTransform();
		}
		else {
			Ring::innerUpdateTransform();
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Ring::create, 0, 4)

void init_SPK_Ring_h() {
	boost::python::class_<RingWrapper, boost::python::bases<SPK::Zone>, std::shared_ptr<SPK::Ring>>("Ring",
		boost::python::init<boost::python::optional<const SPK::Vector3D &, const SPK::Vector3D &, float, float>>(
			":brief: Constructor of ring\n"
			":param position: the position of the ring\n"
			":param normal: the normal of the plane on which lies the ring\n"
			":param minRadius: the minimum radius of the ring\n"
			":param maxRadius: the maximum radius of the ring\n"))
		.def("getClassName", &SPK::Ring::getClassName)
		.def("create", &SPK::Ring::create, create_overload(
			":brief: Creates and registers a new Ring\n"
			":param position: the position of the ring\n"
			":param normal: the normal of the plane on which lies the ring\n"
			":param minRadius: the minimum radius of the ring\n"
			":param maxRadius: the maximum radius of the ring\n"
			":return: a new registered ring\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setNormal", &SPK::Ring::setNormal,
			":brief: Sets the normal of the plane on which lies this ring\n"
			"\n"
			"Note that the normal is normalized internally\n"
			"\n"
			":param normal: the normal of the plane on which lies the ring\n")
		.def("setRadius", &SPK::Ring::setRadius,
			":brief: Sets the min and max radius of this ring\n"
			"\n"
			"A radius cannot be negative.\n"
			"Note that negative radius are inverted internally\n"
			"\n"
			":param minRadius: the minimum radius of this ring\n"
			":param maxRadius: the maximum radius of this ring\n")
		.def("getNormal", &SPK::Ring::getNormal,
			":brief: Gets the normal of this ring\n"
			":return: the normal of this ring\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getTransformedNormal", &SPK::Ring::getTransformedNormal,
			":brief: Gets the transformed normal of this ring\n"
			":return: the transformed normal of this ring\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getMinRadius", &SPK::Ring::getMinRadius,
			":brief: Gets the minimum radius of this ring\n"
			":return: the minimum radius of this ring\n")
		.def("getMaxRadius", &SPK::Ring::getMaxRadius,
			":brief: Gets the maximum radius of this ring\n"
			":return: the maximum radius of this ring\n")
		.def("generatePosition", &SPK::Ring::generatePosition)
		.def("contains", &SPK::Ring::contains)
		.def("intersects", &SPK::Ring::intersects)
		.def("moveAtBorder", &SPK::Ring::moveAtBorder)
		.def("computeNormal", &SPK::Ring::computeNormal)
		.staticmethod("create")
		;
}

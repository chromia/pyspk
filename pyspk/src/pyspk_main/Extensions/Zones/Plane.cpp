// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Plane.h


#include <Extensions/Zones/SPK_Plane.h>
#include <boost/python.hpp>


class PlaneWrapper :
	public SPK::Plane,
	public boost::python::wrapper<SPK::Plane>
{
public:
	using SPK::Plane::Plane;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Plane::getClassName();
		}
	}
	void generatePosition(SPK::Particle & particle, bool full) const {
		if (auto generatePosition = this->get_override("generatePosition")) {
			generatePosition(particle, full);
		}
		else {
			Plane::generatePosition(particle, full);
		}
	}
	bool contains(const SPK::Vector3D & v) const {
		if (auto contains = this->get_override("contains")) {
			return contains(v);
		}
		else {
			return Plane::contains(v);
		}
	}
	bool intersects(const SPK::Vector3D & v0, const SPK::Vector3D & v1, SPK::Vector3D * intersection, SPK::Vector3D * normal) const {
		if (auto intersects = this->get_override("intersects")) {
			return intersects(v0, v1, intersection, normal);
		}
		else {
			return Plane::intersects(v0, v1, intersection, normal);
		}
	}
	void moveAtBorder(SPK::Vector3D & v, bool inside) const {
		if (auto moveAtBorder = this->get_override("moveAtBorder")) {
			moveAtBorder(v, inside);
		}
		else {
			Plane::moveAtBorder(v, inside);
		}
	}
	SPK::Vector3D computeNormal(const SPK::Vector3D & point) const {
		if (auto computeNormal = this->get_override("computeNormal")) {
			return computeNormal(point);
		}
		else {
			return Plane::computeNormal(point);
		}
	}
	void innerUpdateTransform() {
		if (auto innerUpdateTransform = this->get_override("_innerUpdateTransform")) {
			innerUpdateTransform();
		}
		else {
			Plane::innerUpdateTransform();
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_orverload, SPK::Plane::create, 0, 2)

void init_SPK_Plane_h() {

	boost::python::class_<PlaneWrapper, boost::python::bases<SPK::Zone>, std::shared_ptr<SPK::Plane>>("Plane",
		boost::python::init<boost::python::optional<const SPK::Vector3D &, const SPK::Vector3D &>>(
			":brief: Constructor of Plane\n"
			":param position: the position of the Plane\n"
			":param normal: the normal of the Plane\n"))
		.def("getClassName", &SPK::Plane::getClassName)
		.def("create", &SPK::Plane::create, create_orverload(
			":brief: Creates and registers a new Plane\n"
			":param position: the position of the Plane\n"
			":param normal: the normal of the Plane\n"
			":return: a new registered plane\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setNormal", &SPK::Plane::setNormal,
			":brief: Sets the normal of this Plane\n"
			"\n"
			"Note that the normal is normalized internally.\n"
			"\n"
			":param normal:  the normal of this Plane\n")
		.def("getNormal", &SPK::Plane::getNormal,
			":brief: Gets the normal of this Plane\n"
			":return: the normal of this Plane\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getTransformedNormal", &SPK::Plane::getTransformedNormal,
			":brief: Gets the transformed normal of this Plane\n"
			":return: the transformed normal of this Plane\n"
			":since: 1.05.00\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("generatePosition", &SPK::Plane::generatePosition)
		.def("contains", &SPK::Plane::contains)
		.def("intersects", &SPK::Plane::intersects)
		.def("moveAtBorder", &SPK::Plane::moveAtBorder)
		.def("computeNormal", &SPK::Plane::computeNormal)
		.staticmethod("create")
		;
}

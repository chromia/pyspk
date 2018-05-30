// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Point.h


#include <Extensions/Zones/SPK_Point.h>
#include <Core/SPK_Particle.h>
#include <boost/python.hpp>


class PointWrapper :
	public SPK::Point,
	public boost::python::wrapper<SPK::Point>
{
public:
	using SPK::Point::Point;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Point::getClassName();
		}
	}
	void generatePosition(SPK::Particle & particle, bool full) const {
		if (auto generatePosition = this->get_override("generatePosition")) {
			generatePosition(particle, full);
		}
		else {
			Point::generatePosition(particle, full);
		}
	}
	bool contains(const SPK::Vector3D & v) const {
		if (auto contains = this->get_override("contains")) {
			return contains(v);
		}
		else {
			return Point::contains(v);
		}
	}
	bool intersects(const SPK::Vector3D & v0, const SPK::Vector3D & v1, SPK::Vector3D * intersection, SPK::Vector3D * normal) const {
		if (auto intersects = this->get_override("intersects")) {
			return intersects(v0, v1, intersection, normal);
		}
		else {
			return Point::intersects(v0, v1, intersection, normal);
		}
	}
	void moveAtBorder(SPK::Vector3D & v, bool inside) const {
		if (auto moveAtBorder = this->get_override("moveAtBorder")) {
			moveAtBorder(v, inside);
		}
		else {
			Point::moveAtBorder(v, inside);
		}
	}
	SPK::Vector3D computeNormal(const SPK::Vector3D & point) const {
		if (auto computeNormal = this->get_override("computeNormal")) {
			return computeNormal(point);
		}
		else {
			return Point::computeNormal(point);
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Point::create, 0, 1)

void init_SPK_Point_h() {
	boost::python::class_<PointWrapper, boost::python::bases<SPK::Zone>, std::shared_ptr<SPK::Point>>("Point",
		boost::python::init<boost::python::optional<const SPK::Vector3D &>>(
			":brief: Constructor of Point\n"
			":param position: the position of the Point\n"))
		.def("getClassName", &SPK::Point::getClassName)
		.def("create", &SPK::Point::create, create_overload(
			":brief: Creates and registers a new Point\n"
			":param position: the position of the Point\n"
			":return: A new registered Point\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("generatePosition", &SPK::Point::generatePosition)
		.def("contains", &SPK::Point::contains)
		.def("intersects", &SPK::Point::intersects)
		.def("moveAtBorder", &SPK::Point::moveAtBorder)
		.def("computeNormal", &SPK::Point::computeNormal)
		.staticmethod("create")
		;
}

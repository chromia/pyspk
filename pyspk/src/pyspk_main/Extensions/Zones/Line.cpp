// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Line.h


#include <Extensions/Zones/SPK_Line.h>
#include <Core/SPK_Particle.h>
#include <boost/python.hpp>


class LineWrapper :
	public SPK::Line,
	public boost::python::wrapper<SPK::Line>
{
public:
	using SPK::Line::Line;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Line::getClassName();
		}
	}
	void setPosition(const SPK::Vector3D & v) {
		if (auto setPosition = this->get_override("setPosition")) {
			setPosition(v);
		}
		else {
			Line::setPosition(v);
		}
	}
	void generatePosition(SPK::Particle & particle, bool full) const {
		if (auto generatePosition = this->get_override("generatePosition")) {
			generatePosition(particle, full);
		}
		else {
			Line::generatePosition(particle, full);
		}
	}
	bool contains(const SPK::Vector3D & v) const {
		if (auto contains = this->get_override("contains")) {
			return contains(v);
		}
		else {
			return Line::contains(v);
		}
	}
	bool intersects(const SPK::Vector3D & v0, const SPK::Vector3D & v1, SPK::Vector3D * intersection, SPK::Vector3D * normal) const {
		if (auto intersects = this->get_override("intersects")) {
			return intersects(v0, v1, intersection, normal);
		}
		else {
			return Line::intersects(v0, v1, intersection, normal);
		}
	}
	void moveAtBorder(SPK::Vector3D & v, bool inside) const {
		if (auto moveAtBorder = this->get_override("moveAtBorder")) {
			moveAtBorder(v, inside);
		}
		else {
			Line::moveAtBorder(v, inside);
		}
	}
	SPK::Vector3D computeNormal(const SPK::Vector3D & point) const {
		if (auto computeNormal = this->get_override("computeNormal")) {
			return computeNormal(point);
		}
		else {
			return Line::computeNormal(point);
		}
	}
	void innerUpdateTransform() {
		if (auto innerUpdateTransform = this->get_override("_innerUpdateTransform")) {
			innerUpdateTransform();
		}
		else {
			Line::innerUpdateTransform();
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Line::create, 0, 2)

void init_SPK_Line_h() {
	boost::python::class_<LineWrapper, boost::python::bases<SPK::Zone>, std::shared_ptr<SPK::Line>>("Line",
		boost::python::init<boost::python::optional<const SPK::Vector3D &, const SPK::Vector3D &>>(
			":brief: Constructor of Line\n"
			":param p0: the first bound of this Line\n"
			":param p1: the second bound of this Line\n"))
		.def("getClassName", &SPK::Line::getClassName)
		.def("create", &SPK::Line::create, create_overload(
			":brief: Creates and registers a new Line\n"
			":param p0: the first bound of this Line\n"
			":param p1: the second bound of this Line\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setPosition", &SPK::Line::setPosition)
		.def("setBounds", &SPK::Line::setBounds,
			":brief: Sets the bounds of this Line\n"
			":param p0: the first bound of this Line\n"
			":param p1: the second bound of this Line\n")
		.def("getBound", &SPK::Line::getBound,
			":brief: Gets the bound of index of this Line\n"
			":param index: the index of the bound (0 or 1)\n"
			":return: the first bound of index of this Line\n"
			":since: 1.03.00\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getTransformedBound", &SPK::Line::getTransformedBound,
			":brief: Gets the transformed bound of index of this Line\n"
			":param index: the index of the bound (0 or 1)\n"
			":return: the transformed bound of index of this Line\n"
			":since: 1.03.00\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("pushBound", &SPK::Line::pushBound,
			":brief: Pushes a new bound to this Line\n"
			"\n"
			"This method replaces the first bound by the second bound and the second bound by the new bound.\n"
			"It allows to follow the trajectory of a moving object over time with a single Line.\n"
			"\n"
			":param bound: the new bound of this Line\n")
		.def("generatePosition", &SPK::Line::generatePosition)
		.def("contains", &SPK::Line::contains)
		.def("intersects", &SPK::Line::intersects)
		.def("moveAtBorder", &SPK::Line::moveAtBorder)
		.def("computeNormal", &SPK::Line::computeNormal)
		.staticmethod("create")
		;
}

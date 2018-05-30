// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_AABox.h


#include <Extensions/Zones/SPK_AABox.h>
#include <Core/SPK_Particle.h>
#include <boost/python.hpp>


class AABoxWrapper :
	public SPK::AABox,
	public boost::python::wrapper<SPK::AABox>
{
public:
	using SPK::AABox::AABox;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return AABox::getClassName();
		}
	}
	void generatePosition(SPK::Particle & particle, bool full) const {
		if (auto generatePosition = this->get_override("generatePosition")) {
			generatePosition(particle, full);
		}
		else {
			AABox::generatePosition(particle, full);
		}
	}
	bool contains(const SPK::Vector3D & v) const {
		if (auto contains = this->get_override("contains")) {
			return contains(v);
		}
		else {
			return AABox::contains(v);
		}
	}
	bool intersects(const SPK::Vector3D & v0, const SPK::Vector3D & v1, SPK::Vector3D * intersection, SPK::Vector3D * normal) const {
		if (auto intersects = this->get_override("intersects")) {
			return intersects(v0, v1, intersection, normal);
		}
		else {
			return AABox::intersects(v0, v1, intersection, normal);
		}
	}
	void moveAtBorder(SPK::Vector3D & v, bool inside) const {
		if (auto moveAtBorder = this->get_override("moveAtBorder")) {
			moveAtBorder(v, inside);
		}
		else {
			AABox::moveAtBorder(v, inside);
		}
	}
	SPK::Vector3D computeNormal(const SPK::Vector3D & point) const {
		if (auto computeNormal = this->get_override("computeNormal")) {
			return computeNormal(point);
		}
		else {
			return AABox::computeNormal(point);
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::AABox::create, 0, 2)

void init_SPK_AABox_h() {
	boost::python::class_<AABoxWrapper, boost::python::bases<SPK::Zone>, std::shared_ptr<SPK::AABox>>("AABox", boost::python::init<boost::python::optional<const SPK::Vector3D &, const SPK::Vector3D &>>(
		":brief: Constructor of AABox\n"
		":param position: the position of the AABox\n"
		":param dimension: the dimension of the AABox\n"))
		.def("getClassName", &SPK::AABox::getClassName)
		.def("create", &SPK::AABox::create, create_overload(
			":brief: Creates and registers a new AABox\n"
			":param position: the position of the AABox\n"
			":param dimension: the dimension of the AABox\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setDimension", &SPK::AABox::setDimension,
			":brief: Sets the dimensions of this AABox\n"
			"\n"
			"The negative dimensions are clamped to 0.\n"
			"An AABox with 0 as its 3 dimensions is equivalent to a Point\n"
			"\n"
			":param dimension: the dimensions of this AABox\n")
		.def("getDimension", &SPK::AABox::getDimension,
			":brief: Gets the dimensions of this AABox\n"
			":return: the dimensions of this AABox\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("generatePosition", &SPK::AABox::generatePosition)
		.def("contains", &SPK::AABox::contains)
		.def("intersects", &SPK::AABox::intersects)
		.def("moveAtBorder", &SPK::AABox::moveAtBorder)
		.def("computeNormal", &SPK::AABox::computeNormal)
		.staticmethod("create")
		;
}

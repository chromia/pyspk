// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Zone.h


#include <Core/SPK_Zone.h>
#include <Core/SPK_Particle.h>
#include <boost/python.hpp>


class ZoneWrapper :
	public SPK::Zone,
	public boost::python::wrapper<SPK::Zone>
{
public:
	using Zone = SPK::Zone;

	std::string getClassName() const
	{
		return this->get_override("getClassName")();
	}
	Registerable * clone(bool createBase) const
	{
		return this->get_override("clone")(createBase);
	}

	void setPosition(const SPK::Vector3D & v) {
		if (auto setPosition = this->get_override("setPosition")) {
			setPosition(v);
		}
		else {
			Zone::setPosition(v);
		}
	}
	void generatePosition(SPK::Particle & particle, bool full) const {
		this->get_override("generatePosition")(particle, full);
	}
	bool contains(const SPK::Vector3D & point) const {
		return this->get_override("contains")(point);
	}
	bool intersects(const SPK::Vector3D & v0, const SPK::Vector3D & v1, SPK::Vector3D * intersection, SPK::Vector3D * normal) const {
		return this->get_override("intersects")(v0, v1, intersection, normal);
	}
	void moveAtBorder(SPK::Vector3D & point, bool inside) const {
		this->get_override("moveAtBorder")(point, inside);
	}
	SPK::Vector3D computeNormal(const SPK::Vector3D & point) const {
		return this->get_override("computeNormal")(point);
	}
	void innerUpdateTransform() {
		if (auto innerUpdateTransform = this->get_override("_innerUpdateTransform")) {
			innerUpdateTransform();
		}
		else {
			Zone::innerUpdateTransform();
		}
	}
};

void init_SPK_Zone_h() {
	boost::python::class_<ZoneWrapper, boost::python::bases<SPK::Registerable, SPK::Transformable>, boost::noncopyable>("Zone")
		.def("setPosition", &SPK::Zone::setPosition,
			":brief: Sets the position of this Zone\n"
			":param v: the position of this Zone\n")
		.def("getPosition", &SPK::Zone::getPosition,
			":brief: Gets the position of this Zone\n"
			":return: the position of this Zone\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getTransformedPosition", &SPK::Zone::getTransformedPosition,
			":brief: Gets the transformed position of this Zone\n"
			":return: the transformed position of this Zone\n"
			":since: 1.03.00\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("generatePosition", boost::python::pure_virtual(&SPK::Zone::generatePosition),
			":brief: Randomly generates a position inside this Zone for a given Particle\n"
			":param particle: the Particle whose position will be generated\n"
			":param full: True to generate a position in the whole volume of this Zone, False to generate a position only at borders\n")
		.def("contains", boost::python::pure_virtual(&SPK::Zone::contains),
			":brief: Checks whether a point is within the Zone\n"
			":param point: the point to check\n"
			":return: True if the point is within the Zone, False otherwise\n")
		.def("intersects", boost::python::pure_virtual(&SPK::Zone::intersects),
			":brief: Checks whether a line intersects the Zone\n"
			"\n"
			"The intersection is computed only if the Vector3D intersection is not None.\n"
			"The normal is computed if the Vector3D* normal AND intersection are not None.\n"
			"\n"
			":param v0: start of the line\n"
			":param v1: end of the line\n"
			":param intersection: the Vector3D where the intersection will be stored, None not to compute the intersection\n"
			":param normal: the Vector3D where the normal will be stored, None not to compute the normal\n"
			":return: True if the line intersects with the Zone, False otherwise\n")
		.def("moveAtBorder", boost::python::pure_virtual(&SPK::Zone::moveAtBorder),
			":brief: Moves a point at the border of the Zone\n"
			":param point: the point that will be moved to the border of the Zone\n"
			":param inside: True to move the point inside the Zone of APPROXIMATION_VALUE, False to move it outside of APPROXIMATION_VALUE\n")
		.def("computeNormal", boost::python::pure_virtual(&SPK::Zone::computeNormal),
			":brief: Computes the normal for the point\n"
			":param point: the point from where the normal is computed\n"
			":return: the normal vector\n"
			":since: 1.02.00\n")
		.def("getClassName", boost::python::pure_virtual(&SPK::Zone::getClassName))
		;
}

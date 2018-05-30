// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Vector3D.h


#include <Core/SPK_Vector3D.h>
#include <boost/python.hpp>


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Vector3DsetOverloads0, SPK::Vector3D::set, 2, 3)

void init_SPK_Vector3D_h() {
    boost::python::class_<SPK::Vector3D, std::shared_ptr<SPK::Vector3D>>("Vector3D", boost::python::init<boost::python::optional<float, float, float>>(
			":brief: Constructor for the Vector3D\n"
			":param x: x coordinate\n"
			":param y: y coordinate\n"
			":param z: z coordinate\n"))
        .def("__iadd__", static_cast<SPK::Vector3D &(SPK::Vector3D::*)(const SPK::Vector3D &)>(&SPK::Vector3D::operator+=), boost::python::return_value_policy<boost::python::copy_non_const_reference>())
        .def("__iadd__", static_cast<SPK::Vector3D &(SPK::Vector3D::*)(float)>(&SPK::Vector3D::operator+=), boost::python::return_value_policy<boost::python::copy_non_const_reference>())
        .def("__isub__", static_cast<SPK::Vector3D &(SPK::Vector3D::*)(const SPK::Vector3D &)>(&SPK::Vector3D::operator-=), boost::python::return_value_policy<boost::python::copy_non_const_reference>())
        .def("__isub__", static_cast<SPK::Vector3D &(SPK::Vector3D::*)(float)>(&SPK::Vector3D::operator-=), boost::python::return_value_policy<boost::python::copy_non_const_reference>())
        .def("__imul__", &SPK::Vector3D::operator*=, boost::python::return_value_policy<boost::python::copy_non_const_reference>())
        .def("__ifloordiv__", &SPK::Vector3D::operator/=, boost::python::return_value_policy<boost::python::copy_non_const_reference>())
        .def("__neg__", &SPK::Vector3D::operator-)

		//Indexcing is not supported
        //.def("operator[]", static_cast<float &(SPK::Vector3D::*)(int)>(&SPK::Vector3D::operator[]), boost::python::return_value_policy<boost::python::copy_non_const_reference>())
        //.def("operator[]", static_cast<const float &(SPK::Vector3D::*)(int) const>(&SPK::Vector3D::operator[]), boost::python::return_value_policy<boost::python::copy_const_reference>())

        .def("set", &SPK::Vector3D::set, Vector3DsetOverloads0(
			":brief: Sets the values of the Vector3D\n"
			":param x: x coordinate\n"
			":param y: y coordinate\n"
			":param z: z coordinate\n"))
        .def("getSqrNorm", &SPK::Vector3D::getSqrNorm,
			":brief: Gets the square norm of the Vector3D\n"
			"\n"
			"the square norm is defined as <i>x * x + y * y + z * z</i>.\n"
			"This method is faster than getNorm() and should be used when possible.\n"
			"\n"
			":return: the square norm of the Vector3D\n")
        .def("getNorm", &SPK::Vector3D::getNorm,
			":brief: Gets the norm of the Vector3D\n"
			"\n"
			"the norm is defined as <i>math.sqrt(x * x + y * y + z * z)</i>.\n"
			"\n"
			":return: the norm of the Vector3D\n")
        .def("normalize", &SPK::Vector3D::normalize,
			":brief: Normalizes the Vector3D\n"
			"\n"
			"This method performs these operations :<i>\n"
			"x /= |v|\n"
			"y /= |v|\n"
			"z /= |v|</i>\n"
			"Note that if the norm is equal to 0, nothing happens and False is returned.\n"
			"\n"
			":return: True if this Vector3D can be normalized, False otherwise\n")
        .def("revert", &SPK::Vector3D::revert,
			":brief: Reverts the Vector3D\n"
			"\n"
			"This method performs these operations :<i>\n"
			"x = -x\n"
			"y = -y\n"
			"z = -z</i>\n")
        .def("abs", &SPK::Vector3D::abs,
			":brief: Sets this Vector3D to its absolute values\n"
			"\n"
			"This method performs these operations :<i>\n"
			"x = abs(x)\n"
			"y = abs(y)\n"
			"z = abs(z)</i>\n"
			"\n"
			":since: 1.02.00\n")
        .def("crossProduct", &SPK::Vector3D::crossProduct,
			":brief: Computes the cross product between v and the vector3D and store the result in the vector3D\n"
			":param v: the vector3D used to compute the cross product (*this x v)\n")
        .def_readwrite("x", &SPK::Vector3D::x, "x coordinate of the vector")
        .def_readwrite("y", &SPK::Vector3D::y, "y coordinate of the vector")
        .def_readwrite("z", &SPK::Vector3D::z, "z coordinate of the vector")
		.def(boost::python::self_ns::repr(boost::python::self))

		//operator +
		.def(boost::python::self + float())
		.def(float() + boost::python::self)
		.def(boost::python::self + boost::python::self)
		//operator -
		.def(boost::python::self - float())
		.def(float() - boost::python::self)
		.def(boost::python::self - boost::python::self)
		//operator *
		.def(boost::python::self * float())
		.def(float() * boost::python::self)
		//operator /
		.def(boost::python::self / float())
		.def(float() / boost::python::self)
		//operator ==
		.def(boost::python::self == boost::python::self)
		//operator !=
		.def(boost::python::self != boost::python::self)
        ;
    
    boost::python::def("dotProduct", static_cast<float(*)(const SPK::Vector3D &, const SPK::Vector3D &)>(&SPK::dotProduct),
		":brief: Returns the dot product between two Vector3D\n"
		":param v0: the first Vector3D\n"
		":param v1: the second Vector3D\n"
		":return: the dot product (v0 . v1)\n"
	);

	boost::python::def("getSqrDist", &SPK::getSqrDist);
}

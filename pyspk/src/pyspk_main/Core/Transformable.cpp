// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Transformable.h


#include <Core/SPK_Transformable.h>
#include <Core/SPK_Zone.h>
#include <boost/python.hpp>


class TransformableWrapper :
	public SPK::Transformable,
	public boost::python::wrapper<SPK::Transformable>
{
public:
	using SPK::Transformable::Transformable;
	void innerUpdateTransform() {
		if (auto innerUpdateTransform = this->get_override("_innerUpdateTransform")) {
			innerUpdateTransform();
		}
		else {
			Transformable::innerUpdateTransform();
		}
	}
	void propagateUpdateTransform() {
		if (auto propagateUpdateTransform = this->get_override("_propagateUpdateTransform")) {
			propagateUpdateTransform();
		}
		else {
			Transformable::propagateUpdateTransform();
		}
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(updateTransform_overload, SPK::Transformable::updateTransform, 0, 1)

void init_SPK_Transformable_h() {
	boost::python::class_<TransformableWrapper, boost::noncopyable>("Transformable")
		.def("setTransform", &SPK::Transformable::setTransform,
			":brief: Sets the local transform of this Transformable\n"
			"\n"
			"Note that the matrix must contain the following value in that order :\n"
			"<i>[side.x side.y side.z side.w up.x up.y up.z up.w look.x look.y look.z look.w pos.x pos.y pos.z pos.w]</i>\n"
			"(look being -look in right-handed coordinate systems)\n"
			"\n"
			"The matrix being row or column major is just a convention which is not important.\n"
			"The only thing that matters is that vectors coordinates are contiguous in memory.\n"
			"If not, see setTransformNC(float)\n"
			"\n"
			":param transform: the transform to copy its content from\n")
		.def("setTransformNC", &SPK::Transformable::setTransformNC,
			":brief: Sets the local transfom of this Transformable from a \"non contiguous vector coordinates\" matrix\n"
			"\n"
			"Note that the matrix must contain the following value in that order :\n"
			"<i>[side.x up.x look.x pos.x side.x up.x look.x pos.x side.x up.x look.x pos.x side.w up.w look.w pos.w]</i>\n"
			"(look being -look in right-handed coordinate systems)\n"
			"\n"
			"Note the inner transform is stored with vector coordinates being contiguous in memory.\n"
			"See setTransform(const float*)\n"
			"\n"
			":param transform: the transform to copy its content from\n")
		.def("setTransformPosition", &SPK::Transformable::setTransformPosition,
			":brief: Sets the position of the local transform\n"
			"\n"
			"The orientation is left untouched.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param pos: the position of the local transform\n"
			":since: 1.05.00\n")
		.def("setTransformOrientationRH", &SPK::Transformable::setTransformOrientationRH,
			":brief: Sets the orientation of the local transform in a right-handed system\n"
			"\n"
			"The position is left untouched.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param look: the look vector of the transformable\n"
			":param up: the up vector of the transformable\n"
			":since: 1.05.00\n")
		.def("setTransformOrientationLH", &SPK::Transformable::setTransformOrientationLH,
			":brief: Sets the orientation of the local transform in a left-handed system\n"
			"\n"
			"The position is left untouched.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param look: the look vector of the transformable\n"
			":param up: the up vector of the transformable\n"
			":since: 1.05.00\n")
		.def("setTransformOrientation", &SPK::Transformable::setTransformOrientation,
			":brief: Sets the orientation of the local transform\n"
			"\n"
			"This method allows to set the orientation around an arbitrary axis.\n"
			"The position is left untouched.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param axis: the axis of rotation\n"
			":param angle: the angle of rotation around the axis\n"
			":since: 1.05.00\n")
		.def("setTransformOrientationX", &SPK::Transformable::setTransformOrientationX,
			":brief: Sets the orientation of the local transform\n"
			"\n"
			"This method allows to set the orientation around an the x axis.\n"
			"The position is left untouched.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param angle: the angle of rotation around the x axis\n"
			":since: 1.05.00\n")
		.def("setTransformOrientationY", &SPK::Transformable::setTransformOrientationY,
			":brief: Sets the orientation of the local transform\n"
			"\n"
			"This method allows to set the orientation around an the y axis.\n"
			"The position is left untouched.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param angle: the angle of rotation around the y axis\n"
			":since: 1.05.00\n")
		.def("setTransformOrientationZ", &SPK::Transformable::setTransformOrientationZ,
			":brief: Sets the orientation of the local transform\n"
			"\n"
			"This method allows to set the orientation around an the z axis.\n"
			"The position is left untouched.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param angle: the angle of rotation around the z axis\n"
			":since: 1.05.00\n")
		.def("getLocalTransform", &SPK::Transformable::getLocalTransform,
			":brief: Gets the local transform of this Transformable\n"
			":return: a pointer to the local transform of this Transformable\n",
			boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		.def("getWorldTransform", &SPK::Transformable::getWorldTransform,
			":brief: Gets the world transform of this Transformable\n"
			":return: a pointer to the world transform of this Transformable\n",
			boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		.def("isLocalIdentity", &SPK::Transformable::isLocalIdentity,
			":brief: Tells whether the local transform is the identity or not\n"
			":return: True if the local transform is identity, False if not\n")
		.def("getLocalTransformPos", &SPK::Transformable::getLocalTransformPos,
			":brief: Gets the position of the local transform\n"
			":return: the position of the local transform\n"
			":since: 1.05.00\n")
		.def("getLocalTransformSide", &SPK::Transformable::getLocalTransformSide,
			":brief: Gets the side vector of the local transform\n"
			":return: the side vector of the local transform\n"
			":since: 1.05.00\n")
		.def("getLocalTransformUp", &SPK::Transformable::getLocalTransformUp,
			":brief: Gets the up vector of the local transform\n"
			":return: the up vector of the local transform\n"
			":since: 1.05.00\n")
		.def("getLocalTransformLookRH", &SPK::Transformable::getLocalTransformLookRH,
			":brief: Gets the look vector of the local transform in a right-handed system\n"
			":return: the look vector of the local transform\n"
			":since: 1.05.00\n")
		.def("getLocalTransformLookLH", &SPK::Transformable::getLocalTransformLookLH,
			":brief: Gets the look vector of the local transform in a left-handed system\n"
			":return: the look vector of the local transform\n"
			":since: 1.05.00\n")
		.def("getWorldTransformPos", &SPK::Transformable::getWorldTransformPos,
			":brief: Gets the position of the world transform\n"
			":return: the position of the world transform\n"
			":since: 1.05.00\n")
		.def("getWorldTransformSide", &SPK::Transformable::getWorldTransformSide,
			":brief: Gets the side vector of the world transform\n"
			":return: the side vector of the world transform\n"
			":since: 1.05.00\n")
		.def("getWorldTransformUp", &SPK::Transformable::getWorldTransformUp,
			":brief: Gets the up vector of the world transform\n"
			":return: the up vector of the world transform\n"
			":since: 1.05.00\n")
		.def("getWorldTransformLookRH", &SPK::Transformable::getWorldTransformLookRH,
			":brief: Gets the look vector of the world transform  in a right-handed system\n"
			":return: the look vector of the world transform\n"
			":since: 1.05.00\n")
		.def("getWorldTransformLookLH", &SPK::Transformable::getWorldTransformLookLH,
			":brief: Gets the look vector of the world transform  in a left-handed system\n"
			":return: the look vector of the world transform\n"
			":since: 1.05.00\n")
		.def("lookAtRH", &SPK::Transformable::lookAtRH,
			":brief: lookAt method for a right-handed system\n"
			"\n"
			"The vectors are normalized internally.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param target: the point the transformable is looking at\n"
			":param up: the up vector of the transformable\n"
			":param pos: the position of the transformable\n"
			":since: 1.05.00\n")
		.def("lookAtLH", &SPK::Transformable::lookAtLH,
			":brief: lookAt method for a left-handed system\n"
			"\n"
			"The vectors are normalized internally.\n"
			"\n"
			"Note that this methods sets the local transform.\n"
			"To compute the world transform and propagate it, updateTransform(Transformable) must be called afterwards.\n"
			"\n"
			":param target: the point the transformable is looking at\n"
			":param up: the up vector of the transformable\n"
			":param pos: the position of the transformable\n"
			":since: 1.05.00\n")
		.def("updateTransform", &SPK::Transformable::updateTransform, updateTransform_overload(
			":brief: Updates the world transform of this Transformable\n"
			"\n"
			"The parent transform and the local transform is used to derive the world transform.\n"
			"If parent is None, the local transform is simply copied to the world transform.\n"
			"\n"
			"Note that this method only updates the transform if needed\n"
			"\n"
			":param parent: the parent node of this Transformable or None\n"))
		.def("resetTransform", &SPK::Transformable::resetTransform,
			":brief: Resets the transform to identity")
		;
}

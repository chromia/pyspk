// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_System.h


#include <Core/SPK_Factory.h>
#include <Core/SPK_System.h>
#include <Core/SPK_Group.h>
#include <Core/SPK_Vector3D.h>
#include <boost/python.hpp>


class SystemWrapper :
	public SPK::System,
	public boost::python::wrapper<SPK::System>
{
public:
	using SPK::System::System;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return System::getClassName();
		}
	}
	bool update(float deltaTime) {
		if (auto update = this->get_override("update")) {
			return update(deltaTime);
		}
		else {
			return System::update(deltaTime);
		}
	}
	void render() const {
		if (auto render = this->get_override("render")) {
			render();
		}
		else {
			System::render();
		}
	}
	SPK::Registerable * findByName(const std::string & name) {
		if (auto findByName = this->get_override("findByName")) {
			return findByName(name);
		}
		else {
			return System::findByName(name);
		}
	}
	void registerChildren(bool registerAll) {
		if (auto registerChildren = this->get_override("_registerChildren")) {
			registerChildren(registerAll);
		}
		else {
			System::registerChildren(registerAll);
		}
	}
	void copyChildren(const SPK::Registerable & object, bool keepChildren) {
		if (auto copyChildren = this->get_override("_copyChildren")) {
			copyChildren(object, keepChildren);
		}
		else {
			System::copyChildren(object, keepChildren);
		}
	}
	void destroyChildren(bool createBase) {
		if (auto destroyChildren = this->get_override("_destroyChildren")) {
			destroyChildren(createBase);
		}
		else {
			System::destroyChildren(createBase);
		}
	}
	void propagateUpdateTransform() {
		if (auto propagateUpdateTransform = this->get_override("_propagateUpdateTransform")) {
			propagateUpdateTransform();
		}
		else {
			System::propagateUpdateTransform();
		}
	}
};

SPK::System* copySystem(SPK::System *src)
{
	return SPK_Copy(SPK::System, src->getID());
}
void destroySystem(SPK::System *src)
{
	SPK_Destroy(src);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(SystemsetClampStepOverloads0, SPK::System::setClampStep, 1, 2)

void init_SPK_System_h() {

	boost::python::def("copySystem", &copySystem,
		"Creates a new System from an existing one",
		boost::python::return_value_policy<boost::python::reference_existing_object>());
	boost::python::def("destroySystem", &destroySystem,
		"Destroy a System");

	boost::python::class_<SystemWrapper, boost::python::bases<SPK::Registerable, SPK::Transformable>, std::shared_ptr<SPK::System>>("System")
		.def("getClassName", &SPK::System::getClassName)
		.def("create", &SPK::System::create,
			":brief: Creates and registers a new System\n"
			":return: A new registered System\n"
			":since: 1.04.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("setCameraPosition", &SPK::System::setCameraPosition,
			":brief: Sets the camera position\n"
			"\n"
			"Note that the camera position is only useful if a group has to be sorted.\n"
			"In that case this vector will be used as the camera position to derive the distance between the particle and the camera position.\n"
			"The camera position has to be updated before an update of the sorted group.\n"
			"\n"
			":param: cameraPosition the camera position\n")
		.def("setClampStep", &SPK::System::setClampStep,
			SystemsetClampStepOverloads0(
				":brief: Enables or not the clamping on the deltaTime when updating systems\n"
				"\n"
				"This allows to limit too big deltaTime which may spoil your particle systems.\n"
				"Basically if the deltaTime is higher than the clamp value, the clamp calue is used as the deltaTime.\n"
				"\n"
				"It allows in real step mode to avoid having too big deltaTimes and in the other 2 modes to avoid having too\n"
				"many updates that may slow down the application.\n"
				"\n"
				"Note that setting the clamp value too low may slow down your systems\n"
				"\n"
				":param useClampStep: True to use a clamp value on the step, False not to\n"
				":param clamp: the clamp value\n"
				":since: 1.05.00\n"))
		.def("useConstantStep", &SPK::System::useConstantStep,
			":brief: Uses a constant step to update the systems\n"
			"\n"
			"This tells the system to be updated with a constant time.\n"
			"Depending on the deltaTime passed for the update, 0 to many updates can occur.\n"
			"For example if the delta time is 1.0 and the constant step is 0.1 then 10 updates of time 0.1 will occur.\n"
			"\n"
			"This mode is useful when the update must be constant (accurate collisions...) but be aware it can be very computationnaly intensive.\n"
			"\n"
			":param constantStep: the value of the step\n"
			":since: 1.05.00\n")
		.def("useAdaptiveStep", &SPK::System::useAdaptiveStep,
			":brief: Uses an adaptive step to update the systems\n"
			"\n"
			"This tells the system to be updated with a time between min and max.\n"
			"If the deltaTime passed is higher than maxStep or lower than minStep then this mode operates like the constant step mode with\n"
			"either constant time being maxStep or minStep (respectivally).\n"
			"If the deltaTime lies between minStep and maxStep then this mode performs like the real step mode.\n"
			"\n"
			"This mode is a good alternative between the other two.\n"
			"Combined with the clamp step, it allows to correctly handle the step time without being to much frame rate dependant.\n"
			"\n"
			":param minStep: the minimal time step\n"
			":param maxStep: the maximal time step\n"
			":since: 1.05.00\n")
		.def("useRealStep", &SPK::System::useRealStep,
			":brief: Uses the real step to update the systems\n"
			"\n"
			"This is the basic mode (and the mode per default) to update the systems.\n"
			"One call to update means one update of time deltaTime.\n"
			"\n"
			"This mode is the simpler and the one that allows best performance on low end systems.\n"
			"However the update may be inaccurate (due to too big deltaTime) and it performs badly with frame rate variation.\n"
			"\n"
			":since: 1.05.00\n")
		.def("enableAABBComputing", &SPK::System::enableAABBComputing,
			":brief: Enables or disables the computation of the axis aligned Vector for this System\n"
			"\n"
			"Enabling the computation of the AABB for the System only takes the AABB of all AABB of the Groups within the System where AABB computation is enabled.\n"
			"see Group::enableAABBComputing(bool) for more details.\n"
			"\n"
			":param AABB: True to enable the computing of the AABB of this System, False to disable it\n")
		.def("getCameraPosition", &SPK::System::getCameraPosition,
			":brief: Gets the camera position\n"
			"\n"
			"Note that the camera position vector is only read by SPARK. Only the user modifies it.\n"
			"\n"
			":return: the camera position\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getStepMode", &SPK::System::getStepMode,
			":brief: Gets the current step mode\n"
			":return: the current step mode\n")
		.def("getNbParticles", &SPK::System::getNbParticles,
			":brief: Gets the number of active particles in this system\n"
			"\n"
			"The number of active particles in the system is the addition of the number of active particles in each group of the system.\n"
			"Note that the number of active particle of the system is updated after each update of the system.\n"
			"This means if the user changes manually the number of particles in a group and call this method before an update, the number returned will not be up to date.\n"
			"To compute and get the real number of active particles in the System, see computeNbParticles().\n"
			"\n"
			":return: the number of active particle in the system\n")
		.def("computeNbParticles", &SPK::System::computeNbParticles,
			":brief: Computes the number of active particles in this System and returns it\n"
			"\n"
			"Unlike getNbParticles() which returns the last number of particles computed (after a call to update(float) or empty()),\n"
			"this method recomputes the current number of active particles by parsing all the groups of this System.\n"
			"In that way, this method must not be used as an accessor but call once when necesseray between 2 updates.\n"
			"\n"
			"Note that this method updates the inner number of particles of this System, which means a call to getNbParticles() will\n"
			"then return the computed number.\n"
			"\n"
			":return: the number of active particle in the system\n"
			":since: 1.02.01\n")
		.def("getNbGroups", &SPK::System::getNbGroups,
			":brief: Gets the number of groups in the System\n"
			":return: the number of groups in the System\n")
		.def("getGroups", &SPK::System::getGroups,
			":brief: Gets the vector of the groups (pointers) in this System\n"
			"\n"
			"This method allows to modify Group parameters within the System.\n"
			"Note that for addition and removal, methods <i>addGroup(Group)</i> and <i>removeGroup(Group)</i> must exist.\n"
			"\n"
			":return: a STL vector containing the groups in the System\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getGroup", &SPK::System::getGroup,
			":brief: Gets the Group at index\n"
			"\n"
			"Note that no bound check is performed.\n"
			"\n"
			":param index: the index of the Group to get\n"
			":return: the Group at index\n"
			":since: 1.03.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("isAABBComputingEnabled", &SPK::System::isAABBComputingEnabled,
			":brief: Tells whether the computation of the axis aligned bouding box is enabled\n"
			"\n"
			"For a description of the computation of the AABB, see enableAABBComputing(bool).\n"
			"\n"
			":return: True if the computation of the AABB is enabled, False if it is disabled\n")
		.def("getAABBMin", &SPK::System::getAABBMin,
			":brief: Gets a Vector3D holding the minimum coordinates of the AABB of this System.\n"
			"\n"
			"Note that this method is only useful when the AABB computation is enabled (see enableAABBComputing(bool)).\n"
			"\n"
			":return: a Vector3D holding the minimum coordinates of the AABB of this System\n"
			":since: 1.01.00\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getAABBMax", &SPK::System::getAABBMax,
			":brief: Gets a Vector3D holding the maximum coordinates of the AABB of this System.\n"
			"\n"
			"Note that this method is only useful when the AABB computation is enabled (see enableAABBComputing(bool)).\n"
			"\n"
			":return: a Vector3D holding the maximum coordinates of the AABB of this System\n"
			":since: 1.01.00\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("addGroup", &SPK::System::addGroup,
			":brief: Adds a Group to the System\n"
			":param group: a pointer on the Group to add to the System\n")
		.def("removeGroup", &SPK::System::removeGroup,
			":brief: Removes a Group from the System\n"
			"\n"
			"If the Group cannot be found, nothing happens.\n"
			"\n"
			":param group: a pointer on the Group to remove from the System\n")
		.def("update", &SPK::System::update,
			":brief: Updates the System of the current time step\n"
			"\n"
			"Note that this method updates all groups in the System from first to last.\n"
			"\n"
			":param deltaTime: the time step\n"
			":return: True if the System is still active (has active groups)\n")
		.def("render", &SPK::System::render,
			":brief: Renders particles in the System\n"
			"\n"
			"Note that this method renders all groups in the System from first to last.\n")
		.def("grow", &SPK::System::grow,
			":brief: Makes this System grow to the given time\n"
			"\n"
			"This method is useful to get a newwly created System to a mature state.\n"
			"This method only calls update(float) with the step until the total update time reaches the time.\n"
			"\n"
			":param time: the total time of which to update this System\n"
			":param step: the time the System is updated at each call to update(float)\n"
			"\n"
			":since: 1.01.00\n")
		.def("empty", &SPK::System::empty,
			":brief: Empties the System\n"
			"\n"
			"This method will make all particles in the System inactive.\n"
			"However all connections are kept which means groups are still in theSystem.\n")
		.def("sortParticles", &SPK::System::sortParticles,
			":brief: Sorts the particles in all the group of this System where the sorting is enabled\n"
			"\n"
			"Note that the sorting is also performed during the update.\n"
			"This method is therefore only useful when the camera position changes several times between 2 updates.\n"
			"\n"
			"This methods calls the Group::sortParticles() of each Group in this System.\n"
			"\n"
			":since: 1.01.00\n")
		.def("computeDistances", &SPK::System::computeDistances,
			":brief: Computes the distances between each Particle in each Group of this System\n"
			"\n"
			"Note that the distances computation is also performed during the update.\n"
			"This method is therefore only useful when the camera position changes several times between 2 updates.\n"
			"\n"
			"This methods calls the Group::computeDistances() of each Group in this System.\n"
			"\n"
			":since: 1.01.00\n")
		.def("computeAABB", &SPK::System::computeAABB,
			":brief: Computes the bounding box of this System and of all groups in the System\n"
			"\n"
			"The bounding box of the System is only computed if the System has its bounding box computing enabled.\n"
			"In the same way, the bounding box of a Group within the System is only computed if the Group has its bounding box computing enabled.\n"
			"\n"
			"Note that the computation of bounding boxes is also performed during the update.\n"
			"This method is therefore only useful when the bounding boxes have to be recomputed between 2 updates.\n"
			"\n"
			"This methods calls the Group::computeAABB() of each Group in this System.\n"
			"\n"
			":since: 1.01.00\n")
		.def("findByName", &SPK::System::findByName, boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		.staticmethod("create")
		.staticmethod("setCameraPosition")
		.staticmethod("setClampStep")
		.staticmethod("useConstantStep")
		.staticmethod("useAdaptiveStep")
		.staticmethod("useRealStep")
		.staticmethod("getCameraPosition")
		.staticmethod("getStepMode")
		;
	{
		boost::python::enum_<SPK::StepMode>("StepMode",
			":enum: StepMode\n"
			":brief: Enumeration defining how to handle the step time of particle systems\n"
			":since: 1.05.00\n")
			.value("STEP_REAL", SPK::STEP_REAL)
			.value("STEP_CONSTANT", SPK::STEP_CONSTANT)
			.value("STEP_ADAPTIVE", SPK::STEP_ADAPTIVE)
			.export_values()
			;
	}
}

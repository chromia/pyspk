// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Group.h


#include <Core/SPK_Group.h>
#include <Core/SPK_Renderer.h>
#include <Core/SPK_Emitter.h>
#include <Core/SPK_Modifier.h>
#include <Core/SPK_Zone.h>
#include <Core/SPK_Model.h>
#include <Core/SPK_Buffer.h>
#include <boost/python.hpp>
#include <boost/type_traits.hpp>


class GroupWrapper :
	public SPK::Group,
	public boost::python::wrapper<SPK::Group>
{
public:
	GroupWrapper(SPK::Model *model, size_t capacity = 1000)
		: SPK::Group(model, capacity) {}
	GroupWrapper(const GroupWrapper& group)
		: SPK::Group(group) {}

	using Base = SPK::Group;
	//using Base = GroupSub;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Base::getClassName();
		}
	}
	SPK::Registerable * findByName(const std::string & name) {
		if (auto findByName = this->get_override("findByName")) {
			return findByName(name);
		}
		else {
			return Base::findByName(name);
		}
	}
	void registerChildren(bool registerAll) {
		if (auto registerChildren = this->get_override("_registerChildren")) {
			registerChildren(registerAll);
		}
		else {
			Base::registerChildren(registerAll);
		}
	}
	void copyChildren(const SPK::Registerable & object, bool createBase) {
		if (auto copyChildren = this->get_override("_copyChildren")) {
			copyChildren(object, createBase);
		}
		else {
			Base::copyChildren(object, createBase);
		}
	}
	void destroyChildren(bool keepChildren) {
		if (auto destroyChildren = this->get_override("_destroyChildren")) {
			destroyChildren(keepChildren);
		}
		else {
			Base::destroyChildren(keepChildren);
		}
	}
	void propagateUpdateTransform() {
		if (auto propagateUpdateTransform = this->get_override("_propagateUpdateTransform")) {
			propagateUpdateTransform();
		}
		else {
			Group::propagateUpdateTransform();
		}
	}
};

class GroupSub : public SPK::Group
{
public:
	GroupSub(SPK::Model *model = NULL, size_t capacity = SPK::Pool<SPK::Particle>::DEFAULT_CAPACITY)
		: SPK::Group(model, capacity) {}
	GroupSub(const GroupSub& group)
		: SPK::Group(group) {}

	static GroupSub* create(SPK::Model* model = NULL, size_t capacity = SPK::Pool<SPK::Particle>::DEFAULT_CAPACITY)
	{
		GroupSub* obj = new GroupSub(model, capacity);
		registerObject(obj);
		return obj;
	}

	void setCustomUpdate_Proxy(boost::python::object callback)
	{
		auto f = [=](SPK::Particle& particle, float deltaTime) -> bool
		{
			boost::python::object ret = callback(boost::ref(particle), deltaTime);
			boost::python::extract<bool> ext(ret);
			return ext.check() ? ext() : false;
		};
		setCustomUpdate(f);
	}
	void setCustomBirth_Proxy(boost::python::object callback)
	{
		auto f = [=](SPK::Particle& particle)
		{
			callback(boost::ref(particle));
		};
		setCustomBirth(f);
	}
	void setCustomDeath_Proxy(boost::python::object callback)
	{
		auto f = [=](SPK::Particle& particle)
		{
			callback(boost::ref(particle));
		};
		setCustomDeath(f);
	}
};


BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Group::create, 0, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GroupaddParticlesOverloads1, SPK::Group::addParticles, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GroupaddParticlesOverloads2, SPK::Group::addParticles, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GroupaddParticlesOverloads5, SPK::Group::addParticles, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GroupaddParticlesOverloads8, SPK::Group::addParticles, 4, 5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GroupaddParticlesOverloads9, SPK::Group::addParticles, 4, 5)
//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(GroupcreateBufferOverloads0, SPK::Group::createBuffer, 2, 4)
BOOST_PYTHON_FUNCTION_OVERLOADS(sub_create_overload, GroupSub::create, 0, 2)

void init_SPK_Group_h() {

	SPK::Particle& (SPK::Group::*nonconst_getParticle)(size_t) = &SPK::Group::getParticle;
	//const SPK::Particle& (SPK::Group::*const_getParticle)(size_t) const = &SPK::Group::getParticle;
	void (SPK::Group::*public_sortParticles)() = &SPK::Group::sortParticles;

	boost::python::class_<GroupWrapper, boost::python::bases<SPK::Registerable, SPK::Transformable>, std::shared_ptr<SPK::Group>>("_Group",
			":brief: Constructor for a Group\n"
			"\n"
			"A Group is constructed with a Model (if None, the default Model will be used to generate particles).\n"
			"This Model will be used to handle Particle's generation and evolution within the Group.\n"
			"\n"
			"A Group also needs a maximum capacity which is the maximum number of particles the Group can handle. This is necessary to reserve some memory space.\n"
			"Note that the capacity can be changed by calling reallocate(unsigned int).\n"
			"By default, the capacity is set to Pool.DEFAULT_CAPACITY.\n"
			"\n"
			":param model: the Model of this Group\n"
			":param capacity: The maximum number of particles of this Group\n",
			boost::python::init<boost::python::optional<SPK::Model *, size_t>>())
		.def(boost::python::init<const GroupWrapper&>(
			":brief: Copy Constructor of Group\n"
			":param group: the Group to construct the new Group from\n"
			))
		.def("getClassName", &SPK::Group::getClassName)
		.def("create", &SPK::Group::create, create_overload(
			":brief: Creates and registers a new Group\n"
			":param model: the Model of this Group\n"
			":param capacity: The maximum number of particles of this Group\n"
			":return: A new registered Group\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setModel", &SPK::Group::setModel,
			":brief: change the Model of this Group\n"
			"\n"
			"If the model parameter is set to None, the default model will be used. No changes are done if the model parameter is equal\n"
			"to the Model of this Group. Changing the Model of this Group will empty it.\n"
			"\n"
			":param model: the Model of this Group\n")
		.def("setRenderer", &SPK::Group::setRenderer,
			":brief: Sets the Renderer of this Group\n"
			"\n"
			"If the Renderer is set to None, the particles of the Group will not be renderered with a call to render().\n"
			"\n"
			"Note that if the bufferManagement is on (see enableBuffersManagement(bool)), setting the Renderer will first\n"
			"destroys the buffers needed for the previous Renderer held by this Group and create buffers needed for the new Renderer.\n"
			"\n"
			":param renderer: the Renderer of this Group\n")
		.def("setFriction", &SPK::Group::setFriction,
			":brief: Sets the friction of this Group\n"
			"\n"
			"The friction defines the way particles are accelerated or decelerated in their environment.\n"
			"<ul>\n"
			"<li>If the friction is 0.0, particles in the Group are neither accelerated nor decelerated (it is the default setting).</li>\n"
			"<li>If the friction is positive, particles will be decelerated function of their speed.</li>\n"
			"<li>If the friction is negative, particles will be accelerated function of their speed.</li>\n"
			"</ul>\n"
			"The friction is applied on each Particle as followed :\n"
			"<i>velocity *= 1 - min(1,friction * deltaTime / weight)</i>\n"
			"\n"
			"Note that the lighter the Particle, the more effect has the friction on it.\n"
			"\n"
			":param: friction the friction of the Group\n")
		.def("setGravity", &SPK::Group::setGravity,
			":brief: Sets the gravity of this Group\n"
			"\n"
			"The gravity is a vector which defines an attractive force that will be applied to each Particle in the Group during the update.\n"
			"By default the gravity is the null vector (i.e. a Vector3D equal to (0,0,0)) which means no gravity is applied.\n"
			"\n"
			"The gravity is applied on each Particle as followed :\n"
			"<i>velocity += gravity * deltaTime</i>\n"
			"\n"
			":param gravity: the Vector3D that will be used as the gravity for this Group\n")
		.def("enableSorting", &SPK::Group::enableSorting,
			":brief: Enables or disables the sorting of particles\n"
			"\n"
			"The sorting is performed from particles further to the camera to particles closer to the camera.\n"
			"Sorting particles allows to well draw particles with alpha.\n"
			"\n"
			"If the sorting is enabled/disabled, the distance computation is enabled/disabled as well.\n"
			"\n"
			"Note that sorting a Group is a computationnaly expensive operation that should be avoided when possible.\n"
			"\n"
			":param sort: True to enable the sorting of particles, False otherwise\n")
		.def("enableDistanceComputation", &SPK::Group::enableDistanceComputation,
			":brief: Enables or disables the computation of the distance of a Particle from the camera\n"
			"\n"
			"The distance computation happens at each call to update().\n"
			"The distance of a Particle from the camera can be gotten with a call to Particle.getDistanceFromCamera() or Particle.getSqrDistanceFromCamera()\n"
			"\n"
			"Note that the distance is defined by the difference vector between the Particle and the the camera set with System.setCameraPosition(Vector3D).\n"
			"\n"
			"If the distance computation is disabled, then the sorting of particles is disabled as well.\n"
			"\n"
			":param distanceComputation: True to enable the computation of the camera distance, False not to\n"
			":since: 1.01.00\n")
		.def("enableAABBComputing", &SPK::Group::enableAABBComputing,
			":brief: Enables or disables the computation of the axis aligned bouding box of the Group\n"
			"\n"
			"if the computing of the AABB is enabled, after each call to update(), 2 Vector3D are updated with the coordinates information of the AABB.\n"
			"Those Vector3D can be gotten with getAABBMin() and getAABBMax() which give respectively the minimum and maximum coordinates of the bounding box in each axis.\n"
			"\n"
			"Knowing the AABB of a Group of particles can be useful in some case like frustum culling for instance.\n"
			"\n"
			"Note that the bounding box does not use the size information of the particles which means when computing the bounding box, particles are considered to be points in the space.\n"
			"\n"
			":param AABB: True to enable the computing of the AABB of the Group, False to disable it\n")
		.def("enableBuffersManagement", &SPK::Group::enableBuffersManagement,
			":brief: Enables or not Renderer buffers management in a statix way\n"
			"\n"
			"If the buffer management is enabled, then a call to setRenderer(Renderer) will destroy the existing buffers of the previous Renderer\n"
			"held by this Group, and create the needed buffer for the new Renderer.\n"
			"\n"
			"By default, the renderer buffers management is enabled.\n"
			"\n"
			":param manage: True to enable buffers management, False to disable it\n"
			":since: 1.03.00\n")
		.def("getParticles", &SPK::Group::getParticles,
			":brief: Gets the Pool of particles of the Group\n"
			"\n"
			"Note that the Pool returned is constant as the user is not allowed to modify the internal structure of particles.\n"
			"\n"
			":return: the Pool of the Group\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getParticle", nonconst_getParticle,
			":brief: Gets a single Particle in the Group by its index\n"
			"\n"
			"Note that a given Particle in a Group is not ensured to keep the same index all over its life.\n"
			"Particle index can be changed when inactivating particles.\n"
			"\n"
			":param index: the position in the Group's Pool of the Particle to get\n"
			":return: the Particle at index\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		//.def("getParticle", const_getParticle, boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getNbParticles", &SPK::Group::getNbParticles,
			":brief: Gets the number of particles in the Group\n"
			":return: the number of particles in the Group\n")
		.def("getEmitters", &SPK::Group::getEmitters,
			":brief: Gets the emitters of the Group\n"
			":return: the vector of emitters of the Group\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getEmitter", &SPK::Group::getEmitter,
			":brief: Gets an Emitter of the Group by its index\n"
			":param index: the position in the vector of emitters of the Emitter to get\n"
			":return: the Emitter at index\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getNbEmitters", &SPK::Group::getNbEmitters,
			":brief: Gets the number of emitters in this Group\n"
			":return: the number of emitters in this Group\n")
		.def("getModifiers", &SPK::Group::getModifiers,
			":brief: Gets the modifiers of the Group\n"
			":return: the vector of modifiers of the Group\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getModifier", &SPK::Group::getModifier,
			":brief: Gets an Modifier of the Group by its index\n"
			":param index: the position in the vector of modifiers of the Modifier to get\n"
			":return: the Modifier at index\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getNbModifiers", &SPK::Group::getNbModifiers,
			":brief: Gets the number of modifiers in this Group\n"
			":return: the number of modifiers in this Group\n")
		.def("getModel", &SPK::Group::getModel,
			":brief: Gets the Model of this Group\n"
			":return: the Model of this Group\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getRenderer", &SPK::Group::getRenderer,
			":brief: Gets the Renderer of this Group\n"
			":return: the Renderer of this Group\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getFriction", &SPK::Group::getFriction,
			":brief: Gets the friction coefficient of this Group\n"
			"\n"
			"For a description of the friction see setFriction(float).\n"
			"\n"
			":return: the friction coefficient of this Group\n")
		.def("getGravity", &SPK::Group::getGravity,
			":brief: Gets the gravity Vector3D of this Group\n"
			"\n"
			"For a description of the gravity see setGravity(Vector3D).\n"
			"\n"
			":return: the gravity Vector3D of this Group\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("isSortingEnabled", &SPK::Group::isSortingEnabled,
			":brief: Tells whether the sorting of particles from back to front is enabled\n"
			"\n"
			"For a description of the sorting of particles, see enableSorting(bool).\n"
			"\n"
			":return: True if the sorting is enabled, False otherwise\n")
		.def("isDistanceComputationEnabled", &SPK::Group::isDistanceComputationEnabled,
			":brief: Tells whether the distance computation between particles and camera is enabled\n"
			":return: True is the distance computation is enabled, False if not\n"
			":since: 1.01.00\n")
		.def("isAABBComputingEnabled", &SPK::Group::isAABBComputingEnabled,
			":brief: Tells whether the computation of the axis aligned bouding box is enabled\n"
			"\n"
			"For a description of the computation of the AABB, see enableAABBComputing(bool).\n"
			"\n"
			":return: True if the computation of the AABB is enabled, False if it is disabled\n")
		.def("getAABBMin", &SPK::Group::getAABBMin,
			":brief: Gets a Vector3D holding the minimum coordinates of the AABB of the Group.\n"
			"\n"
			"Note that this method is only useful when the AABB computation is enabled (see enableAABBComputing(bool)).\n"
			"\n"
			":return: a Vector3D holding the minimum coordinates of the AABB of the Group\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getAABBMax", &SPK::Group::getAABBMax,
			":brief: Gets a Vector3D holding the maximum coordinates of the AABB of the Group.\n"
			"\n"
			"Note that this method is only useful when the AABB computation is enabled (see enableAABBComputing(bool)).\n"
			"\n"
			":return: a Vector3D holding the maximum coordinates of the AABB of the Group\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
		.def("getParamAddress", &SPK::Group::getParamAddress,
			":brief: Gets the start address of the given param\n"
			"\n"
			"This method is used by a Renderer to define the start position of an array to pass to the GPU.\n"
			"You will not generally need it unless you re designing your own Renderer.\n"
			"\n"
			"Note that if the parameter is not enabled, the return value will point to an enabled parameter starting address.\n"
			"\n"
			":param param: the parameter whose start address is gotten\n"
			":since: 1.03.00\n",
			boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		.def("getPositionAddress", &SPK::Group::getPositionAddress,
			":brief: Gets the start address of the position\n"
			"\n"
			"This method is used by a Renderer to define the start position of an array to pass to the GPU.\n"
			"You will not generally need it unless you re designing your own Renderer.\n"
			"\n"
			":since: 1.03.00\n",
			boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		.def("getParamStride", &SPK::Group::getParamStride,
			":brief: Gets the stride for parameters\n"
			"\n"
			"This method is used by a Renderer to know the stride of an array to pass to the GPU.\n"
			"You will not generally need it unless you re designing your own Renderer.\n"
			"\n"
			":since: 1.03.00\n")
		.def("getPositionStride", &SPK::Group::getPositionStride,
			":brief: Gets the stride for positions\n"
			"\n"
			"This method is used by a Renderer to know the stride of an array to pass to the GPU.\n"
			"You will not generally need it unless you re designing your own Renderer.\n"
			"\n"
			":since: 1.03.00\n")
		.def("isBuffersManagementEnabled", &SPK::Group::isBuffersManagementEnabled,
			":brief: Tells whether renderers buffer management is enabled or not\n"
			"\n"
			"see enableBuffersManagement(bool) for more information.\n"
			"\n"
			":return: True if renderers buffer management is enabled, False if it is disable\n"
			":since: 1.03.00\n")
		// addParticles is implemented as wrapper function in __init__.py
		.def("addParticles_raw", static_cast<void(SPK::Group::*)(unsigned int, const SPK::Vector3D &, const SPK::Vector3D &)>(&SPK::Group::addParticles))
		.def("addParticles_raw", static_cast<void(SPK::Group::*)(unsigned int, const SPK::Zone *, SPK::Emitter *, bool)>(&SPK::Group::addParticles), GroupaddParticlesOverloads1())
		.def("addParticles_raw", static_cast<void(SPK::Group::*)(unsigned int, const SPK::Zone *, const SPK::Vector3D &, bool)>(&SPK::Group::addParticles), GroupaddParticlesOverloads2())
		.def("addParticles_raw", static_cast<void(SPK::Group::*)(unsigned int, const SPK::Vector3D &, SPK::Emitter *)>(&SPK::Group::addParticles))
		.def("addParticles_raw", static_cast<void(SPK::Group::*)(unsigned int, SPK::Emitter *)>(&SPK::Group::addParticles))
		.def("addParticles_raw", static_cast<void(SPK::Group::*)(const SPK::Zone *, SPK::Emitter *, float, bool)>(&SPK::Group::addParticles), GroupaddParticlesOverloads5())
		.def("addParticles_raw", static_cast<void(SPK::Group::*)(const SPK::Vector3D &, SPK::Emitter *, float)>(&SPK::Group::addParticles))
		.def("addParticles_raw", static_cast<void(SPK::Group::*)(SPK::Emitter *, float)>(&SPK::Group::addParticles))
		.def("addParticles_raw", static_cast<float(SPK::Group::*)(const SPK::Vector3D &, const SPK::Vector3D &, SPK::Emitter *, float, float)>(&SPK::Group::addParticles), GroupaddParticlesOverloads8())
		.def("addParticles_raw", static_cast<float(SPK::Group::*)(const SPK::Vector3D &, const SPK::Vector3D &, const SPK::Vector3D &, float, float)>(&SPK::Group::addParticles), GroupaddParticlesOverloads9())
		.def("removeParticle", &SPK::Group::removeParticle,
			":brief: Removes a Particle from this Group\n"
			"\n"
			"Note that the Particle object is not destroyed but only inactivated in the Pool.\n"
			"\n"
			":param index: the position of the Particle in this Group\n")
		.def("addEmitter", &SPK::Group::addEmitter,
			":brief: Adds an Emitter in this Group\n"
			"\n"
			"Note that if the emitter is already in the group, it will not be inserted again.\n"
			"\n"
			":param emitter: the Emitter to add\n")
		.def("removeEmitter", &SPK::Group::removeEmitter,
			":brief: Removes an Emitter from this Group\n"
			":param emitter: the Emitter to remove\n")
		.def("addModifier", &SPK::Group::addModifier,
			":brief: Adds an Modifier in this Group\n"
			":param modifier: the Modifier to add\n")
		.def("removeModifier", &SPK::Group::removeModifier,
			":brief: Removes an Modifier from this Group\n"
			":param modifier: the Modifier to remove\n")
		.def("update", &SPK::Group::update,
			":brief: Updates this Group by a step time\n"
			"\n"
			"The update process performs those operations :\n"
			"<ul>\n"
			"<li>Updates the mutable parameters of each Particle.</li>\n"
			"<li>Updates the velocity of each Particle function of the friction and the gravity of the Group.</li>\n"
			"<li>Applies each Modifier of the Group to each Particle.</li>\n"
			"<li>Removes all dead particles</li>\n"
			"<li>Adds particles generated by the emitters of the Group.</li>\n"
			"<li>Adds particles added manually by the user (with calls to addParticles).</li>\n"
			"</ul>\n"
			"Those operations are optimized to limit the swaps and shifts of particles in the Pool to its minimum.\n"
			"This method tells whether the Group still has some Particles, or if some Particles will still be generated by the Emitters at the next updates by returning a boolean.\n"
			"\n"
			":param deltaTime: the time step used to update the Group\n"
			":return: True if the Group has still some current or future Particles to update, False otherwise\n")
		.def("render", &SPK::Group::render,
			":brief: Renders this Group\n"
			"\n"
			"Note that if no Renderer is attached to the Group, nothing will happen.\n")
		.def("empty", &SPK::Group::empty,
			":brief: Empties this Group\n"
			"\n"
			"Not that this method does not release resource of this Group.\n"
			"Particles are only inactivated, not destroyed.\n")
		.def("flushAddedParticles", &SPK::Group::flushAddedParticles,
			":brief: Adds all manually added particles to the Group\n"
			"\n"
			"This method adds all particles added with the addParticles methods immediatly to the Group without waiting the next call to update().\n"
			"The Particles are added in FIFO order until there is no more or the capacity limit of the Group is reached.\n")
		.def("sortParticles", public_sortParticles,
			":brief: Sorts the particles within this Group\n"
			"\n"
			"The particles are sorted only if the sorting of this Group is enabled. See enableSorting(bool).\n"
			"Note that update() also sorts the particles.\n"
			"This method is therefore only useful when the camera position changes several times between 2 updates.\n"
			"\n"
			"This method also makes a call to computeDistances().\n"
			"\n"
			":since: 1.01.00\n")
		.def("computeDistances", &SPK::Group::computeDistances,
			":brief: Computes the distance between each Particle within this Group and the camera\n"
			"\n"
			"The distances are computed only if the computation of distances of this Group is enabled. See enableDistanceComputation(bool).\n"
			"Note that update() also computes the distances between particles and camera.\n"
			"This method is therefore only useful when the camera position changes several times between 2 updates.\n"
			"\n"
			":since: 1.01.00\n")
		.def("computeAABB", &SPK::Group::computeAABB,
			":brief: Computes the bounding box of this Group\n"
			"\n"
			"The bounding box is computed only if the computation of the bounding box of this Group is enabled. See enableAABBComputing(bool).\n"
			"Note that update() also computes the bounding box.\n"
			"This method is therefore only useful when the bounding boxe has to be recomputed between 2 updates.\n"
			"\n"
			":since: 1.01.00\n")
		.def("reallocate", &SPK::Group::reallocate,
			":brief: Increases the maximum number of particles this Group can hold\n"
			"\n"
			"Note that decreasing the capacity will have no effect.\n"
			"A reallocation of the group capacity will destroy all its current buffers.\n"
			"\n"
			":param: capacity The maximum number of particles of this Group\n"
			":since: 1.02.00\n")
		//.def("createBuffer", &SPK::Group::createBuffer, GroupcreateBufferOverloads0()[boost::python::return_value_policy<boost::python::return_opaque_pointer>()])
		//.def("destroyBuffer", &SPK::Group::destroyBuffer)
		//.def("destroyAllBuffers", &SPK::Group::destroyAllBuffers)
		//.def("getBuffer", static_cast<SPK::Buffer *(SPK::Group::*)(const std::string &, unsigned int) const>(&SPK::Group::getBuffer), boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		//.def("getBuffer", static_cast<SPK::Buffer *(SPK::Group::*)(const std::string &) const>(&SPK::Group::getBuffer), boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		.def("findByName", &SPK::Group::findByName, boost::python::return_value_policy<boost::python::return_opaque_pointer>())
		.staticmethod("create")
		.staticmethod("enableBuffersManagement")
		.staticmethod("isBuffersManagementEnabled")
		;


		auto group = boost::python::class_<GroupSub, boost::python::bases<SPK::Group>, std::shared_ptr<GroupSub>>("Group",
			boost::python::init<boost::python::optional<SPK::Model *, size_t>>())
			.def(boost::python::init<const GroupSub&>())
			.def("create", &GroupSub::create, sub_create_overload()
				[boost::python::return_value_policy<boost::python::reference_existing_object>()]);

		//check if customized version of SPARK is used
		typedef void (SPK::Group::*calltype)(std::function<void(SPK::Particle&)>);
		bool customized = boost::is_same<decltype(&SPK::Group::setCustomBirth), calltype>::value;
		if (customized) {
			group
				.def("setCustomUpdate", &GroupSub::setCustomUpdate_Proxy,
					":brief: Assigns a callback for the custom update\n"
					"\n"
					"The user has the possibility to assign a callback function for update that will be called for each Particle right after the standard update.\n"
					"The signature of the function must be of the form :\n"
					"<i>def customUpdate(Particle, float) -> bool:</i>\n"
					"with :\n"
					"<ul>\n"
					"<li>Particle  being the Particle which is currently updated</li>\n"
					"<li>float  being the time step</li>\n"
					"<li> the return bool being True if the Particle has to die at the end of the update, False otherwise</li>\n"
					"</ul>\n"
					"\n"
					":param fupdate: the callback function that will perform custom update for this Group\n")
				.def("setCustomBirth", &GroupSub::setCustomBirth_Proxy,
					":brief: Assigns a callback for custom birth\n"
					"\n"
					"This method allows to assign a callback function that will be called each time a Particle borns.\n"
					"The signature of the function must be of the form :\n"
					"<i>def customUpdate(Particle):</i>\n"
					"with Particle  being the Particle which is just born.\n"
					"\n"
					":param fbirth: the callback function that will perform custom birth for this Group\n")
				.def("setCustomDeath", &GroupSub::setCustomDeath_Proxy,
					":brief: Assigns a callback for custom death\n"
					"\n"
					"This method allows to assign a callback function that will be called each time a Particle dies.\n"
					"The signature of the function must be of the form :\n"
					"<i>def customUpdate(Particle):</i>\n"
					"with Particle  being the Particle which has just died.\n"
					"\n"
					":param fdeath: the callback function that will perform custom death for this Group\n");
		}
}

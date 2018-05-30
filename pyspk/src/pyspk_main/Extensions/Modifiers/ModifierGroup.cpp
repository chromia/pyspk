// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_ModifierGroup.h


#include <Extensions/Modifiers/SPK_ModifierGroup.h>
#include <Core/SPK_Group.h>
#include <boost/python.hpp>


class ModifierGroupWrapper :
    public SPK::ModifierGroup,
    public boost::python::wrapper<SPK::ModifierGroup>
{
public:
    using SPK::ModifierGroup::ModifierGroup;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return ModifierGroup::getClassName();
        }
    }
    void createBuffers (const SPK::Group & group) {
        if ( auto createBuffers = this->get_override("createBuffers") ) {
            createBuffers(group);
        } else {
            ModifierGroup::createBuffers(group);
        }
    }
    void destroyBuffers (const SPK::Group & group) {
        if ( auto destroyBuffers = this->get_override("destroyBuffers") ) {
            destroyBuffers(group);
        } else {
            ModifierGroup::destroyBuffers(group);
        }
    }
    SPK::Registerable * findByName (const std::string & name) {
        if ( auto findByName = this->get_override("findByName") ) {
            return findByName(name);
        } else {
            return ModifierGroup::findByName(name);
        }
    }
    void registerChildren (bool registerAll) {
        if ( auto registerChildren = this->get_override("_registerChildren") ) {
            registerChildren(registerAll);
        } else {
            ModifierGroup::registerChildren(registerAll);
        }
    }
    void copyChildren (const SPK::Registerable & object, bool createBase) {
        if ( auto copyChildren = this->get_override("_copyChildren") ) {
            copyChildren(object, createBase);
        } else {
            ModifierGroup::copyChildren(object, createBase);
        }
    }
    void destroyChildren (bool keepChildren) {
        if ( auto destroyChildren = this->get_override("_destroyChildren") ) {
            destroyChildren(keepChildren);
        } else {
            ModifierGroup::destroyChildren(keepChildren);
        }
    }
    bool checkBuffers (const SPK::Group & group) {
        if ( auto checkBuffers = this->get_override("_checkBuffers") ) {
            return checkBuffers(group);
        } else {
            return ModifierGroup::checkBuffers(group);
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::ModifierGroup::create, 0, 2)

void init_SPK_ModifierGroup_h() {
    boost::python::class_<ModifierGroupWrapper, boost::python::bases<SPK::Modifier>, std::shared_ptr<SPK::ModifierGroup>>("ModifierGroup",
		boost::python::init<boost::python::optional<SPK::Zone *, SPK::ModifierTrigger>>(
			":brief: Constructor of ModifierGroup\n"
			":param zone: the Zone of this ModifierGroup\n"
			":param trigger: the trigger of the ModifierGroup\n"))
        .def("getClassName", &SPK::ModifierGroup::getClassName)
        .def("create", &SPK::ModifierGroup::create, create_overload(
			":brief: Creates and registers a new ModifierGroup\n"
			":param zone: the Zone of this ModifierGroup\n"
			":param trigger: the trigger of the ModifierGroup\n"
			":return: A new registered ModifierGroup\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .def("useGlobalGroup", &SPK::ModifierGroup::useGlobalGroup,
			":brief: Uses this ModifierGroup as a global group\n"
			"\n"
			"A global group allows to use only one trigger for many modifiers. It works as follow :<ul>\n"
			"<li>If the trigger of the partition group is activated, then the all children modifiers are activated no matter their Zone.</li>\n"
			"<li>The same happens for the wrong side.</li>\n"
			"</ul>\n"
			"Note that if a child Modifier needs intersection or normal computation (the Modifier Obstacle for instance), the variables have to be set.\n"
			"\n"
			":param useIntersection: True to enable intersection computation in this ModifierGroup\n"
			":param useNormal: True to enable normal computation in this ModifierGroup\n")
        .def("usePartitionGroup", &SPK::ModifierGroup::usePartitionGroup,
			":brief: Uses this ModifierGroup as a partition group\n"
			"\n"
			"A partition group allows to partition the space in order to have faster tests. It works as follow :<ul>\n"
			"<li>If the trigger of the partition group is activated, then modifiers within are tested to check if they are triggered.</li>\n"
			"<li>If yes, the Modifier is activated, if no, nothing happens.</li>\n"
			"<li>If handleWrongSize if set, the isWrongSide() method of the partition group calls the isWrongSide() of its children if it has to.</li>\n"
			"</ul>\n"
			"\n"
			":param handleWrongSide: True to enable intersection computation in this ModifierGroup\n")
        .def("getNb", &SPK::ModifierGroup::getNb,
			":brief: Gets the number of modifiers in this ModifierGroup\n"
			":return: the number of modifiers in this ModifierGroup\n")
        .def("getModifiers", &SPK::ModifierGroup::getModifiers,
			":brief: Gets the vector containing all the children modifiers\n"
			":return: the vector containing all the children modifiers\n",
			boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("isGlobalGroup", &SPK::ModifierGroup::isGlobalGroup,
			":brief: Tells whether this ModifierGroup is a global group\n"
			"\n"
			"For more information about global groups, see useGlobalGroup(bool,bool).\n"
			"\n"
			":return: True if this ModifierGroup is a global group, False if not\n")
        .def("isPartitionGroup", &SPK::ModifierGroup::isPartitionGroup,
			":brief: Tells whether this ModifierGroup is a partition group\n"
			"\n"
			"For more information about partition groups, see usePartitionGroup(bool).\n"
			"\n"
			":return: True if this ModifierGroup is a partition group, False if not\n")
        .def("handlesWrongSide", &SPK::ModifierGroup::handlesWrongSide,
			":brief: Tells whether this partition group handles wrong side\n"
			"\n"
			"If this ModifierGroup is a global group, the return value is not used.\n"
			"For more information about partition groups, see usePartitionGroup(bool).\n"
			"\n"
			"Note that the wrong side can only be used with the following triggers :<ul>\n"
			"<li>ModifierTrigger.INSIDE_ZONE</li>\n"
			"<li>ModifierTrigger.OUTSIDE_ZONE</li>\n"
			"<li>ModifierTrigger.ENTER_ZONE</li>\n"
			"<li>ModifierTrigger.EXIT_ZONE</li>\n"
			"</ul>\n"
			"\n"
			":return: True if this ModifierGroup handles wrong size, False if not\n")
        .def("usesIntersection", &SPK::ModifierGroup::usesIntersection,
			":brief: Tells whether this global group computes the intersection\n"
			"\n"
			"If this ModifierGroup is a partition group, the return value is not used.\n"
			"For more information about global groups, see useGlobalGroup(bool,bool).\n"
			"\n"
			"Note that the intersection can only be used with the following triggers :<ul>\n"
			"<li>ModifierTrigger.INTERSECT_ZONE</li>\n"
			"<li>ModifierTrigger.ENTER_ZONE</li>\n"
			"<li>ModifierTrigger.EXIT_ZONE</li>\n"
			"</ul>\n"
			"\n"
			":return: True if this ModifierGroup computes the intersection, False if not\n")
        .def("usesNormal", &SPK::ModifierGroup::usesNormal,
			":brief: Tells whether this global group computes the normal\n"
			"\n"
			"If this ModifierGroup is a partition group, the return value is not used.\n"
			"For more information about global groups, see useGlobalGroup(bool,bool).\n"
			"\n"
			"Note that the normal can only be used with the following triggers :<ul>\n"
			"<li>ModifierTrigger.INTERSECT_ZONE</li>\n"
			"<li>ModifierTrigger.ENTER_ZONE</li>\n"
			"<li>ModifierTrigger.EXIT_ZONE</li>\n"
			"</ul>\n"
			"\n"
			":return: True if this ModifierGroup computes the normal, False if not\n")
        .def("addModifier", &SPK::ModifierGroup::addModifier,
			":brief: Adds a Modifier to this ModifierGroup\n"
			":param modifier: the Modifier to add to this ModifierGroup\n")
        .def("removeModifier", &SPK::ModifierGroup::removeModifier,
			":brief: Removes a Modifier from this ModifierGroup\n"
			":param modifier: the Modifier to remove from this ModifierGroup\n"
			":return: True if the Modifier has been found and removed, False if not\n")
        .def("clear", &SPK::ModifierGroup::clear,
			":brief: Removes all Modifier children from this ModifierGroup\n")
        .def("createBuffers", &SPK::ModifierGroup::createBuffers)
        .def("destroyBuffers", &SPK::ModifierGroup::destroyBuffers)
        .def("findByName", &SPK::ModifierGroup::findByName, boost::python::return_value_policy<boost::python::return_opaque_pointer>())
        .staticmethod("create")
        ;
}

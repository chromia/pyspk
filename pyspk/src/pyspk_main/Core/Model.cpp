// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Model.h


#include "Core/SPK_Model.h"
#include <boost/python.hpp>


class ModelWrapper :
	public SPK::Model,
	public boost::python::wrapper<SPK::Model>
{
public:
	using SPK::Model::Model;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return Model::getClassName();
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Model::create, 0, 4)

void init_SPK_Model_h() {
	boost::python::class_<ModelWrapper, boost::python::bases<SPK::Registerable>, std::shared_ptr<SPK::Model>>("Model",
		boost::python::init<boost::python::optional<int, int, int, int>>(
			":brief: The constructor for Model\n"
			"\n"
			"The user have to pass the Model flags that cannot be changed afterwards.\n"
			"To set up flags enumerators from ModelParamFlag can be used in a OR way.\n"
			"For instance, <i>Model(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA,FLAG_NONE,FLAG_NONE,FLAG_NONE)</i>\n"
			"constructs a Model that will generate Particles with a defined color that will not change over time.\n"
			"\n"
			"Since 1.03.00, PARAM_RED, PARAM_GREEN and PARAM_BLUE are enabled by default.\n"
			"This is needed to pass the values to the GPU in an optimized way.\n"
			"Note that even the enable flag passed by the user does not containt those params, they will be enabled anyway.\n"
			"\n"
			":param enableFlag: the enable flag\n"
			":param mutableFlag: the mutable flag\n"
			":param randomFlag: the random flag\n"
			":param interpolatedFlag: the interpolated flag\n"))
		.def(boost::python::init<SPK::Model&>(
			":brief: The copy constructor for Model\n"
			":param model: The Model to construct the new Model from\n"))
		.def("getClassName", &SPK::Model::getClassName)
		.def("create", &SPK::Model::create, create_overload(
			":brief: Creates and registers a new Model\n"
			":param enableFlag: the enable flag\n"
			":param mutableFlag: the mutable flag\n"
			":param randomFlag: the random flag\n"
			":param interpolatedFlag: the interpolated flag\n"
			":return: A new registered Model\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setLifeTime", &SPK::Model::setLifeTime,
			":brief: Sets the life time\n"
			"\n"
			"All particles generated under this Model will be given a life time randomly generated within [lifeTimeMin,lifeTimeMax[.\n"
			"To generate particles with a non random life time, simply use <i>setLifeTime(a,a)</i>.\n"
			"\n"
			":param lifeTimeMin: the minimum life time\n"
			":param lifeTimeMax: the maximum life time\n")
		.def("setImmortal", &SPK::Model::setImmortal,
			":brief: Defines whether to generate immortal particles or not\n"
			"\n"
			"Immortal particles will still be given a life time. However the age of immortal particles will not increase so that they cannot die.\n"
			"An immortal Particle whose Model immortality is unset will grow older again.\n"
			"The immortality is therefore dynamic and can be granted or removed by impacting all particles generated under this Model.\n"
			"\n"
			":param immortal: whether the Model will generate immortal particles or not\n")
		.def("setParam", static_cast<bool(SPK::Model::*)(SPK::ModelParam, float, float, float, float)>(&SPK::Model::setParam),
			":brief: Sets a given parameter with 4 values\n"
			"\n"
			"This method will only set parameters that are enabled, mutable and random.\n"
			"If not nothing will happen and the method will return False.\n"
			"\n"
			":param type: the parameter to set\n"
			":param startMin: the minimum value at the Particle birth\n"
			":param startMax: the maximum value at the Particle birth\n"
			":param endMin: the minimum value at the Particle death\n"
			":param endMax: the maximum value at the Particle death\n"
			":return: True if the parameter can be set (right flags), False otherwise\n")
		.def("setParam", static_cast<bool(SPK::Model::*)(SPK::ModelParam, float, float)>(&SPK::Model::setParam),
			":brief: Sets a given parameter with 2 values\n"
			"\n"
			"This method will only set parameters that are enabled and <b>either</b> mutable or random.\n"
			"If not nothing will happen and the method will return False.\n"
			"\n"
			"In case of a mutable parameter : value0 defines the birth value and value0 defines the death value.\n"
			"In case of a random parameter : value0 defines the minimum value and value1 defines the maximum value.\n"
			"\n"
			":param type: the parameter to set\n"
			":param value0: the first value (the meaning depends on the flag)\n"
			":param value1: the second value (the meaning depends on the flag)\n"
			":return: True if the parameter can be set (right flags), False otherwise\n")
		.def("setParam", static_cast<bool(SPK::Model::*)(SPK::ModelParam, float)>(&SPK::Model::setParam),
			":brief: Sets a given parameter with 1 value\n"
			"\n"
			"This method will only set parameters that are enabled and <b>neither</b> mutable or random.\n"
			"If not nothing will happen and the method will return False.\n"
			"\n"
			"Note that to set the value of an interpolated parameter cannot be set with this function,\n"
			"you must used the interpolator object associated to the parameter to control its value.\n"
			"\n"
			":param type: the parameter to set\n"
			":param value: the value of the parameter\n"
			":return: True if the parameter can be set (right flags), False otherwise\n")
		.def("getLifeTimeMin", &SPK::Model::getLifeTimeMin,
			":brief: Gets the minimum life time\n"
			":return: the minimum life time\n")
		.def("getLifeTimeMax", &SPK::Model::getLifeTimeMax,
			":brief: Gets the maximum life time\n"
			":return: the minimum life time\n")
		.def("isImmortal", &SPK::Model::isImmortal,
			":brief: Returns whether immortility is set or not\n"
			":return: whether immortility is set or not\n")
		.def("isEnabled", &SPK::Model::isEnabled,
			":brief: Checks whether a parameter is enabled or not\n"
			":param type: the parameter\n"
			":return: 0 is the parameter is not enabled, a flag with the parameter bit set otherwise\n")
		.def("isMutable", &SPK::Model::isMutable,
			":brief: Checks whether a parameter is mutable or not\n"
			":param type: the parameter\n"
			":return: 0 is the parameter is not mutable, a flag with the parameter bit set otherwise\n")
		.def("isRandom", &SPK::Model::isRandom,
			":brief: Checks whether a parameter is random or not\n"
			":param type: the parameter\n"
			":return: 0 is the parameter is not random, a flag with the parameter bit set otherwise\n")
		.def("isInterpolated", &SPK::Model::isInterpolated,
			":brief: Checks whether a parameter is interpolated or not\n"
			":param type: the parameter\n"
			":return: 0 is the parameter is not interpolated, a flag with the parameter bit set otherwise\n"
			":since: 1.05.00\n")
		.def("getParamValue", &SPK::Model::getParamValue,
			":brief: Gets a parameter value\n"
			"\n"
			"If index is superior or equal to the number of values for the parameter, the default value of the parameter is returned.\n"
			"Otherwise the value returned depending on the index is :\n"
			"<ul>\n"
			"<li>Enabled parameters :\n"
			"  <ul>\n"
			"  <li>0 : the value of the parameter</li>\n"
			"  </ul>\n"
			"</li>\n"
			"<li>Mutable parameters :\n"
			"  <ul>\n"
			"  <li>0 : the birth value of the parameter</li>\n"
			"  <li>1 : the death value of the parameter</li>\n"
			"  </ul>\n"
			"</li>\n"
			"<li>Random parameters :\n"
			"  <ul>\n"
			"  <li>0 : the minimum value of the parameter</li>\n"
			"  <li>1 : the maximum value of the parameter</li>\n"
			"  </ul>\n"
			"</li>\n"
			"<li>Mutable and random parameters :\n"
			"  <ul>\n"
			"  <li>0 : the minimum birth value of the parameter</li>\n"
			"  <li>1 : the maximum birth value of the parameter</li>\n"
			"  <li>2 : the minimum death value of the parameter</li>\n"
			"  <li>3 : the maximum death value of the parameter</li>\n"
			"  </ul>\n"
			"</li>\n"
			"</ul>\n"
			"\n"
			"Note that in case of an interpolated parameter, the default value is always returned.\n"
			"The user has to use the interpolator object associated to the parameter to get its values.\n"
			"\n"
			":param type: the parameter\n"
			":param index: the index of the value to get\n"
			":return: the value\n")
		.def("getNbValues", &SPK::Model::getNbValues,
			":brief: Gets the number of values for a parameter\n"
			"\n"
			"The number of values depends on the Model flags :\n"
			"<ul>\n"
			"<li>Enabled parameter : 1</li>\n"
			"<li>Mutable <b>or</b> random parameter : 2</li>\n"
			"<li>Mutable <b>and</b> random parameter : 4</li>\n"
			"<li>Interpolated parameter : 0</li>\n"
			"<li>Not Enabled : 0</li>\n"
			"</ul>\n"
			":param type: the parameter\n"
			":return: the number of values stored in the Model for the parameter\n")
		.def("getNbEnabled", &SPK::Model::getNbEnabled,
			":brief: Gets the number of enabled parameters\n"
			":return: the number of enabled parameters in the model\n"
			":since: 1.05.00\n")
		.def("getNbMutable", &SPK::Model::getNbMutable,
			":brief: Gets the number of mutable parameters\n"
			":return: the number of mutable parameters in the model\n"
			":since: 1.05.00\n")
		.def("getNRandom", &SPK::Model::getNRandom,
			":brief: Gets the number of random parameters\n"
			":return: the number of random parameters in the model\n"
			":since: 1.05.00\n")
		.def("getInterpolator", &SPK::Model::getInterpolator,
			":brief: Gets the interpolator for the given parameter\n"
			":param param: the parameter whose intepolator must be retrieved\n"
			":return: a pointer to the interpolator of the given parameter or NULL if the parameter is not interpolated\n"
			":since: 1.05.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getNbInterpolated", &SPK::Model::getNbInterpolated,
			":brief: Gets the number of interpolated parameters\n"
			":return: the number of interpolated parameters in the model\n"
			":since: 1.05.00\n")
		.def("getSizeOfParticleCurrentArray", &SPK::Model::getSizeOfParticleCurrentArray,
			":brief: Gets the number of float values in the particle current array\n"
			"\n"
			"The particle current array holds the current values of enabled parameters for a particle.\n"
			"This is used internally and should not be needed by the user.\n"
			"\n"
			":return: the number of float values in the particle current array\n"
			":since: 1.02.00\n")
		.def("getSizeOfParticleExtendedArray", &SPK::Model::getSizeOfParticleExtendedArray,
			":brief: Gets the number of float values in the particle extended array\n"
			"\n"
			"The particle extended array holds the extended values needed for parameters interpolation for a particle.\n"
			"This is used internally and should not be needed by the user.\n"
			"\n"
			":return: the number of float values in the particle extended array\n"
			":since: 1.03.00\n")
		.def("getSizeOfModelArray", &SPK::Model::getSizeOfModelArray,
			":brief: Gets the number of float values in the model array\n"
			"\n"
			"This is used internally and should not be needed by the user.\n"
			"\n"
			":return: the number of float values in the model array\n"
			":since: 1.02.00\n")
		.def("getParameterOffset", &SPK::Model::getParameterOffset,
			":brief: Gets the offset of the given parameter in the current array\n"
			"\n"
			"This methods is used internally by the engine\n"
			"\n"
			":param param: the parameter\n"
			":return: the offset of the given parameter in the current array\n"
			":since: 1.03.00\n")
		.def("getDefaultValue", &SPK::Model::getDefaultValue,
			":brief: Gets the default value of the parameter\n"
			":param param: the parameter to get the default value from\n"
			":return: the default value of the parameter\n"
			":since: 1.05.00\n")
		.staticmethod("create")
		.staticmethod("getDefaultValue")
		;
	{
		boost::python::enum_<SPK::ModelParamFlag>("ModelParamFlag",
			":enum: ModelParamFlag\n"
			":brief: Constants used to set bits in Model flags\n"
			"\n"
			"These constant values are simply <i>1 << ModelParam</i> constants\n"
			)
			.value("FLAG_NONE", SPK::FLAG_NONE)
			.value("FLAG_RED", SPK::FLAG_RED)
			.value("FLAG_GREEN", SPK::FLAG_GREEN)
			.value("FLAG_BLUE", SPK::FLAG_BLUE)
			.value("FLAG_ALPHA", SPK::FLAG_ALPHA)
			.value("FLAG_SIZE", SPK::FLAG_SIZE)
			.value("FLAG_MASS", SPK::FLAG_MASS)
			.value("FLAG_ANGLE", SPK::FLAG_ANGLE)
			.value("FLAG_TEXTURE_INDEX", SPK::FLAG_TEXTURE_INDEX)
			.value("FLAG_ROTATION_SPEED", SPK::FLAG_ROTATION_SPEED)
			.value("FLAG_CUSTOM_0", SPK::FLAG_CUSTOM_0)
			.value("FLAG_CUSTOM_1", SPK::FLAG_CUSTOM_1)
			.value("FLAG_CUSTOM_2", SPK::FLAG_CUSTOM_2)
			.export_values()
			;
	}
}

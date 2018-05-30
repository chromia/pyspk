// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_DEF.h


#include <Core/SPK_DEF.h>
#include <boost/python.hpp>

void setRandomSeed(unsigned int seed)
{
	SPK::randomSeed = seed;
}
unsigned int randint(unsigned int min, unsigned max)
{
	return SPK::random(min, max);
}
float random(float min, float max)
{
	return SPK::random(min, max);
}

void init_SPK_DEF_h() {
	{
		boost::python::def("setRandomSeed", &setRandomSeed,
			":brief: set the random seed for the pseudo random numbers generation (1 by default)");
		boost::python::def("randint", static_cast<unsigned int(*)(unsigned int, unsigned int)>(&randint),
			":brief: Returns a random number in the range [min,max[\n"
			"\n"
			"Note that the sequence of pseudo random number generated depends on the initial seed which can be set by setting randomSeed.\n"
			"\n"
			":param min: the minimum value\n"
			":param max: the maximum value\n"
			"\n"
			":return: a random number within [min,max[\n"
		);
		boost::python::def("random", static_cast<float (*)(float, float)>(&random),
			":brief: Returns a random number in the range [min,max[\n"
			"\n"
			"Note that the sequence of pseudo random number generated depends on the initial seed which can be set by setting randomSeed.\n"
			"\n"
			":param min: the minimum value\n"
			":param max: the maximum value\n"
			"\n"
			":return: a random number within [min,max[\n"
		);

		boost::python::enum_<SPK::ModelParam>("ModelParam",
			":enum: ModelParam\n"
			":brief: Constants for the Model parameters\n"
			)
			.value("PARAM_RED", SPK::PARAM_RED)
			.value("PARAM_GREEN", SPK::PARAM_GREEN)
			.value("PARAM_BLUE", SPK::PARAM_BLUE)
			.value("PARAM_ALPHA", SPK::PARAM_ALPHA)
			.value("PARAM_SIZE", SPK::PARAM_SIZE)
			.value("PARAM_MASS", SPK::PARAM_MASS)
			.value("PARAM_ANGLE", SPK::PARAM_ANGLE)
			.value("PARAM_TEXTURE_INDEX", SPK::PARAM_TEXTURE_INDEX)
			.value("PARAM_ROTATION_SPEED", SPK::PARAM_ROTATION_SPEED)
			.value("PARAM_CUSTOM_0", SPK::PARAM_CUSTOM_0)
			.value("PARAM_CUSTOM_1", SPK::PARAM_CUSTOM_1)
			.value("PARAM_CUSTOM_2", SPK::PARAM_CUSTOM_2)
			.export_values()
			;
	}
}

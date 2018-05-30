// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Interpolator.h


#include <Core/SPK_Interpolator.h>
#include <Core/SPK_Particle.h>
#include <boost/python.hpp>


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(InterpolatorsetTypeOverloads0, SPK::Interpolator::setType, 1, 2)

void init_SPK_Interpolator_h() {
    boost::python::class_<SPK::InterpolatorEntry, std::shared_ptr<SPK::InterpolatorEntry>>("InterpolatorEntry",
		":brief: Default constructor of interpolator entry. All values are set to 0")
        .def(boost::python::init<float, float>(
			":brief: Constructs an interpolator entry with y0 and y1 having the same value\n"
			":param x: the x value\n"
			":param y: the y value (value of y0 and y1)\n"))
        .def(boost::python::init<float, float, float>(
			":brief: Constructs and interpolator entry\n"
			":param x: the x value\n"
			":param y0: the y0 value\n"
			":param y1: the y1 value\n"))
        .def(boost::python::init<float>(
			"used internally"))
        .def_readwrite("x", &SPK::InterpolatorEntry::x, "x value of this entry")
        .def_readwrite("y0", &SPK::InterpolatorEntry::y0, "y first value of this entry")
        .def_readwrite("y1", &SPK::InterpolatorEntry::y1, "y second value of this entry")
		.def(boost::python::self < boost::python::self)
		.def(boost::python::self == boost::python::self)
        ;

    boost::python::class_<SPK::Interpolator, boost::noncopyable>("Interpolator", boost::python::no_init)
        .def("setType", &SPK::Interpolator::setType, InterpolatorsetTypeOverloads0(
			":brief: Sets the value used to interpolate\n"
			"\n"
			"See the class description for more information.\n"
			"Note that the argument param is only used when the type is INTERPOLATOR_PARAM.\n"
			"\n"
			":param type: the type of value used to interpolate\n"
			":param param: the parameter used to interpolate when the type is INTERPOLATOR_PARAM.\n"))
        .def("enableLooping", &SPK::Interpolator::enableLooping,
			":brief: Enables or disables the looping of the graph\n"
			"\n"
			"The range of the graph is defined between the entry with the minimum x and the entry with the maximum y.\n"
			"If the looping is disabled, the x are clamped to the range.\n"
			"If the looping is enabled, the value of x is reported in the range. It is better that the xmin and xmax have\n"
			"the same y values so that the graph tiles perfectly.\n"
			"\n"
			":param loop: true to enabled the looping, false to disable it\n")
        .def("setScaleXVariation", &SPK::Interpolator::setScaleXVariation,
			":brief: Sets the scale variation in x\n"
			"\n"
			"See the class description for more information\n"
			"\n"
			":param scaleXVariation: the scale variation in x\n")
        .def("setOffsetXVariation", &SPK::Interpolator::setOffsetXVariation,
			":brief: Sets the offset variation in x\n"
			"\n"
			"See the class description for more information\n"
			"\n"
			":param offsetXVariation: the offset variation in x\n")
        .def("getType", &SPK::Interpolator::getType,
			":brief: Gets the type of value used to interpolate\n"
			":return: the type of value used to interpolate\n")
        .def("getInterpolatorParam", &SPK::Interpolator::getInterpolatorParam,
			":brief: Gets the parameter used to interpolate\n"
			"\n"
			"Note that the parameter is only used if the type is INTERPOLATOR_PARAM\n"
			"\n"
			":return: the parameter used to interpolate\n")
        .def("isLoopingEnabled", &SPK::Interpolator::isLoopingEnabled,
			":brief: Tells whether the looping is enabled or not\n"
			":return: True if the looping is enabled, False if not\n")
        .def("getScaleXVariation", &SPK::Interpolator::getScaleXVariation,
			":brief: Gets the scale variation along x\n"
			":return: the scale variation along x\n")
        .def("getOffsetXVariation", &SPK::Interpolator::getOffsetXVariation,
			":brief: Gets the offset variation along x\n"
			":return: the offset variation along x\n")
        .def("getGraph", static_cast<std::set<SPK::InterpolatorEntry> &(SPK::Interpolator::*)()>(&SPK::Interpolator::getGraph),
			":brief: Gets the graph of the interpolator\n"
			":return: the graph of the interpolator\n",
			boost::python::return_value_policy<boost::python::copy_non_const_reference>())
        //.def("getGraph", static_cast<const std::set<SPK::InterpolatorEntry> &(SPK::Interpolator::*)() const>(&SPK::Interpolator::getGraph),
		//	boost::python::return_value_policy<boost::python::copy_const_reference>())
        .def("addEntry", static_cast<bool(SPK::Interpolator::*)(const SPK::InterpolatorEntry &)>(&SPK::Interpolator::addEntry),
			":brief: Adds an entry to the graph\n"
			":param entry: the entry to add to the graph\n"
			":return: True if the entry has been added to the graph, False if not (the graph already contains an entry with the same x)\n")
        .def("addEntry", static_cast<bool(SPK::Interpolator::*)(float, float)>(&SPK::Interpolator::addEntry),
			":brief: Adds an entry to the graph\n"
			":param x: the x of the entry to add\n"
			":param y: the y of the entry to add (y0 and y1 are set to y)\n"
			":return: True if the entry has been added to the graph, False if not (the graph already contains an entry with the same x)\n")
        .def("addEntry", static_cast<bool(SPK::Interpolator::*)(float, float, float)>(&SPK::Interpolator::addEntry),
			":brief: Adds an entry to the graph\n"
			":param x: the x of the entry to add\n"
			":param y0: the y0 of the entry to add\n"
			":param y1: the y1 of the entry to add\n"
			":return: True if the entry has been added to the graph, False if not (the graph already contains an entry with the same x)\n")
        .def("clearGraph", &SPK::Interpolator::clearGraph,
			":brief: Clears the graph (removes all the entries)")
        .def("generateSinCurve", &SPK::Interpolator::generateSinCurve,
			":brief: Generates a sinusoidal curve\n"
			"\n"
			"Note that the graph is previously cleared from all its entries\n")
        .def("generatePolyCurve", &SPK::Interpolator::generatePolyCurve,
			":brief: Generates a polynomial curve\n"
			"\n"
			"Note that the graph is previously cleared from all its entries\n")
        ;

	{
        boost::python::enum_<SPK::InterpolationType>("InterpolationType",
			":enum: InterpolationType\n"
			":brief: Constants defining which type of value is used for interpolation\n"
			":since: 1.05.00\n")
            .value("INTERPOLATOR_LIFETIME", SPK::INTERPOLATOR_LIFETIME)
            .value("INTERPOLATOR_AGE", SPK::INTERPOLATOR_AGE)
            .value("INTERPOLATOR_PARAM", SPK::INTERPOLATOR_PARAM)
            .value("INTERPOLATOR_VELOCITY", SPK::INTERPOLATOR_VELOCITY)
            .export_values()
            ;
    }
}

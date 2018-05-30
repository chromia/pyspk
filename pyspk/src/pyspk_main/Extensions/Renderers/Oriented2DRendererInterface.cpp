// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Oriented2DRendererInterface.h


#include <Extensions/Renderers/SPK_Oriented2DRendererInterface.h>
#include <boost/python.hpp>


void init_SPK_Oriented2DRendererInterface_h() {
    boost::python::class_<SPK::Oriented2DRendererInterface, std::shared_ptr<SPK::Oriented2DRendererInterface>>("Oriented2DRendererInterface",
		":brief: Constructor of Oriented2DRendererInterface")
        .def("setOrientation", &SPK::Oriented2DRendererInterface::setOrientation,
			":brief: Sets the way quads are oriented in the universe\n"
			":param orientation: the orientation of the quad\n")
        .def("getOrientation", &SPK::Oriented2DRendererInterface::getOrientation,
			":brief: Gets the orientation of the quads\n"
			":return: the orientation of the quads\n")
        .def_readwrite("orientationVector", &SPK::Oriented2DRendererInterface::orientationVector,
			":brief: The orientation vector\n"
			"\n"
			"It is used in 2 orientation modes :\n"
			"<ul>\n"
			"<li>ORIENTATION2D_AXIS : The orientation vector is used as the axis</li>\n"
			"<li>ORIENTATION2D_POINT : The orientation vector is the point particle look to</li>\n"
			"</ul>\n"
			"In other modes the orientation vector is not used\n")
        ;
    {
        boost::python::enum_<SPK::Orientation2D>("Orientation2D", 
			":brief: Defines the orientation of a particle oriented in 2D\n"
			":since: 1.04.00\n")
            .value("ORIENTATION2D_UP", SPK::ORIENTATION2D_UP)
            .value("ORIENTATION2D_DIRECTION", SPK::ORIENTATION2D_DIRECTION)
            .value("ORIENTATION2D_POINT", SPK::ORIENTATION2D_POINT)
            .value("ORIENTATION2D_AXIS", SPK::ORIENTATION2D_AXIS)
            .export_values()
            ;
    }
}

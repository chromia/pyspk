// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_LineRendererInterface.h


#include <Extensions/Renderers/SPK_LineRendererInterface.h>
#include <boost/python.hpp>


class LineRendererInterfaceWrapper :
    public SPK::LineRendererInterface,
    public boost::python::wrapper<SPK::LineRendererInterface>
{
public:
    using SPK::LineRendererInterface::LineRendererInterface;
    void setWidth (float width) {
        if ( auto setWidth = this->get_override("setWidth") ) {
            setWidth(width);
        } else {
            LineRendererInterface::setWidth(width);
        }
    }
};

void init_SPK_LineRendererInterface_h() {
    boost::python::class_<LineRendererInterfaceWrapper, std::shared_ptr<SPK::LineRendererInterface>>("LineRendererInterface",
		boost::python::init<boost::python::optional<float, float>>(
			":brief: Constructor of LineRendererInterface\n"
			":param length: the length multiplier of this LineRendererInterface\n"
			":param width: the width of this GLLineRenderer\n"))
        .def("setLength", &SPK::LineRendererInterface::setLength,
			":brief: Sets the length multiplier of this LineRendererInterface\n"
			"\n"
			"The length multiplier is the value which will be multiplied by the Particle velocity to get the line length in the universe.\n"
			"A positive length means the line will be drawn in advance to the Particle, as opposed to a negative length.\n"
			"\n"
			":param length: the length multiplier of this GLLineRenderer\n")
        .def("setWidth", &SPK::LineRendererInterface::setWidth,
			":brief: Sets the width of this LineRendererInterface\n"
			":param width: the width of this LineRendererInterface\n")
        .def("getLength", &SPK::LineRendererInterface::getLength,
			":brief: Gets the length multiplier of this LineRendererInterface\n"
			":return: the length multiplier of this LineRendererInterface\n")
        .def("getWidth", &SPK::LineRendererInterface::getWidth,
			":brief: Gets the width of this LineRendererInterface\n"
			":return: the width of this LineRendererInterface\n")
        ;
}

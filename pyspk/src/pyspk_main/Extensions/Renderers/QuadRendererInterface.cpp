// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_QuadRendererInterface.h


#include <Extensions/Renderers/SPK_QuadRendererInterface.h>
#include <boost/python.hpp>


class QuadRendererInterfaceWrapper :
	public SPK::QuadRendererInterface,
	public boost::python::wrapper<SPK::QuadRendererInterface>
{
public:
	using SPK::QuadRendererInterface::QuadRendererInterface;
	bool setTexturingMode(SPK::TexturingMode mode) {
		if (auto setTexturingMode = this->get_override("setTexturingMode")) {
			return setTexturingMode(mode);
		}
		else {
			return QuadRendererInterface::setTexturingMode(mode);
		}
	}
};

void init_SPK_QuadRendererInterface_h() {
	boost::python::class_<QuadRendererInterfaceWrapper, std::shared_ptr<SPK::QuadRendererInterface>>("QuadRendererInterface", boost::python::init<boost::python::optional<float, float>>())
		.def("setTexturingMode", &SPK::QuadRendererInterface::setTexturingMode)
		.def("setAtlasDimensions", &SPK::QuadRendererInterface::setAtlasDimensions)
		.def("setScale", &SPK::QuadRendererInterface::setScale)
		.def("getTexturingMode", &SPK::QuadRendererInterface::getTexturingMode)
		.def("getAtlasDimensionX", &SPK::QuadRendererInterface::getAtlasDimensionX)
		.def("getAtlasDimensionY", &SPK::QuadRendererInterface::getAtlasDimensionY)
		.def("getScaleX", &SPK::QuadRendererInterface::getScaleX)
		.def("getScaleY", &SPK::QuadRendererInterface::getScaleY)
		;
	{
		boost::python::enum_<SPK::TexturingMode>("TexturingMode")
			.value("TEXTURE_NONE", SPK::TEXTURE_NONE)
			.value("TEXTURE_2D", SPK::TEXTURE_2D)
			.value("TEXTURE_3D", SPK::TEXTURE_3D)
			.export_values()
			;
	}
}

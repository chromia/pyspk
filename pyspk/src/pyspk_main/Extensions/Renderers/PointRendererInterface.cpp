// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_PointRendererInterface.h


#include <Extensions/Renderers/SPK_PointRendererInterface.h>
#include <boost/python.hpp>


class PointRendererInterfaceWrapper :
	public SPK::PointRendererInterface,
	public boost::python::wrapper<SPK::PointRendererInterface>
{
public:
	using SPK::PointRendererInterface::PointRendererInterface;
	bool setType(SPK::PointType type) {
		if (auto setType = this->get_override("setType")) {
			return setType(type);
		}
		else {
			return PointRendererInterface::setType(type);
		}
	}
	void setSize(float size) {
		if (auto setSize = this->get_override("setSize")) {
			setSize(size);
		}
		else {
			PointRendererInterface::setSize(size);
		}
	}
};

void init_SPK_PointRendererInterface_h() {
	boost::python::class_<PointRendererInterfaceWrapper, std::shared_ptr<SPK::PointRendererInterface>>("PointRendererInterface", boost::python::init<boost::python::optional<SPK::PointType, float>>())
		.def("setType", &SPK::PointRendererInterface::setType)
		.def("setSize", &SPK::PointRendererInterface::setSize)
		.def("getType", &SPK::PointRendererInterface::getType)
		.def("getSize", &SPK::PointRendererInterface::getSize)
		;
	{
		boost::python::enum_<SPK::PointType>("PointType")
			.value("POINT_SQUARE", SPK::POINT_SQUARE)
			.value("POINT_CIRCLE", SPK::POINT_CIRCLE)
			.value("POINT_SPRITE", SPK::POINT_SPRITE)
			.export_values()
			;
	}
}

// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Oriented3DRendererInterface.h


#include <Extensions/Renderers/SPK_Oriented3DRendererInterface.h>
#include <boost/python.hpp>


void init_SPK_Oriented3DRendererInterface_h() {
    boost::python::class_<SPK::Oriented3DRendererInterface, std::shared_ptr<SPK::Oriented3DRendererInterface>>("Oriented3DRendererInterface")
        .def("setOrientation", static_cast<void(SPK::Oriented3DRendererInterface::*)(SPK::LookOrientation, SPK::UpOrientation, SPK::LockedAxis)>(&SPK::Oriented3DRendererInterface::setOrientation))
        .def("setOrientation", static_cast<void(SPK::Oriented3DRendererInterface::*)(SPK::OrientationPreset)>(&SPK::Oriented3DRendererInterface::setOrientation))
        .def("getLookOrientation", &SPK::Oriented3DRendererInterface::getLookOrientation)
        .def("getUpOrientation", &SPK::Oriented3DRendererInterface::getUpOrientation)
        .def("getLockedAxis", &SPK::Oriented3DRendererInterface::getLockedAxis)
        .def_readwrite("lookVector", &SPK::Oriented3DRendererInterface::lookVector)
        .def_readwrite("upVector", &SPK::Oriented3DRendererInterface::upVector)
        ;
    {
        boost::python::enum_<SPK::LookOrientation>("LookOrientation")
            .value("LOOK_CAMERA_PLANE", SPK::LOOK_CAMERA_PLANE)
            .value("LOOK_CAMERA_POINT", SPK::LOOK_CAMERA_POINT)
            .value("LOOK_AXIS", SPK::LOOK_AXIS)
            .value("LOOK_POINT", SPK::LOOK_POINT)
            .export_values()
            ;
    }
    {
        boost::python::enum_<SPK::UpOrientation>("UpOrientation")
            .value("UP_CAMERA", SPK::UP_CAMERA)
            .value("UP_DIRECTION", SPK::UP_DIRECTION)
            .value("UP_AXIS", SPK::UP_AXIS)
            .value("UP_POINT", SPK::UP_POINT)
            .export_values()
            ;
    }
    {
        boost::python::enum_<SPK::LockedAxis>("LockedAxis")
            .value("LOCK_LOOK", SPK::LOCK_LOOK)
            .value("LOCK_UP", SPK::LOCK_UP)
            .export_values()
            ;
    }
    {
        boost::python::enum_<SPK::OrientationPreset>("OrientationPreset")
            .value("CAMERA_PLANE_ALIGNED", SPK::CAMERA_PLANE_ALIGNED)
            .value("CAMERA_POINT_ALIGNED", SPK::CAMERA_POINT_ALIGNED)
            .value("DIRECTION_ALIGNED", SPK::DIRECTION_ALIGNED)
            .value("AROUND_AXIS", SPK::AROUND_AXIS)
            .value("TOWARDS_POINT", SPK::TOWARDS_POINT)
            .value("FIXED_ORIENTATION", SPK::FIXED_ORIENTATION)
            .export_values()
            ;
    }
}

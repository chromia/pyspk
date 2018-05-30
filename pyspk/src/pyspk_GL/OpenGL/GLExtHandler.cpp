// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_GLExtHandler.h


#include <RenderingAPIs/OpenGL/SPK_GLExtHandler.h>
#include <boost/python.hpp>


void init_SPK_GLExtHandler_h() {
    auto boost_python_SPK_GL_GLExtHandler = boost::python::class_<SPK::GL::GLExtHandler, boost::noncopyable>("GLExtHandler", boost::python::no_init)
        .def("loadGLExtPointSprite", &SPK::GL::GLExtHandler::loadGLExtPointSprite)
        .def("loadGLExtPointParameter", &SPK::GL::GLExtHandler::loadGLExtPointParameter)
        .def("setPixelPerUnit", &SPK::GL::GLExtHandler::setPixelPerUnit)
        .def("loadGLExtTexture3D", &SPK::GL::GLExtHandler::loadGLExtTexture3D)
        .def("glTexImage3D", &SPK::GL::GLExtHandler::glTexImage3D)
        .def("loadGLExtShader", &SPK::GL::GLExtHandler::loadGLExtShader)
        .staticmethod("loadGLExtPointSprite")
        .staticmethod("loadGLExtPointParameter")
        .staticmethod("setPixelPerUnit")
        .staticmethod("loadGLExtTexture3D")
        .staticmethod("glTexImage3D")
        .staticmethod("loadGLExtShader")
        ;
    {
        /*boost::python::enum_<SPK::GL::GLExtHandler::GLExtension>("GLExtension")
            .value("UNCHECKED", SPK::GL::GLExtHandler::UNCHECKED)
            .value("SUPPORTED", SPK::GL::GLExtHandler::SUPPORTED)
            .value("UNSUPPORTED", SPK::GL::GLExtHandler::UNSUPPORTED)
            .export_values()
            ;*/
    }
}

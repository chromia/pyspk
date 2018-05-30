// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_GLLineRenderer.h


#include <RenderingAPIs/OpenGL/SPK_GLLineRenderer.h>
#include <Core/SPK_Group.h>
#include <boost/python.hpp>


class GLLineRendererWrapper :
    public SPK::GL::GLLineRenderer,
    public boost::python::wrapper<SPK::GL::GLLineRenderer>
{
public:
    using SPK::GL::GLLineRenderer::GLLineRenderer;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return GLLineRenderer::getClassName();
        }
    }
    void createBuffers (const SPK::Group & group) {
        if ( auto createBuffers = this->get_override("createBuffers") ) {
            createBuffers(group);
        } else {
            GLLineRenderer::createBuffers(group);
        }
    }
    void destroyBuffers (const SPK::Group & group) {
        if ( auto destroyBuffers = this->get_override("destroyBuffers") ) {
            destroyBuffers(group);
        } else {
            GLLineRenderer::destroyBuffers(group);
        }
    }
    void render (const SPK::Group & group) {
        if ( auto render = this->get_override("render") ) {
            render(group);
        } else {
            GLLineRenderer::render(group);
        }
    }
    bool checkBuffers (const SPK::Group & group) {
        if ( auto checkBuffers = this->get_override("_checkBuffers") ) {
            return checkBuffers(group);
        } else {
            return GLLineRenderer::checkBuffers(group);
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::GL::GLLineRenderer::create, 0, 2)

void init_SPK_GLLineRenderer_h() {
    boost::python::class_<GLLineRendererWrapper, boost::python::bases<SPK::GL::GLRenderer, SPK::LineRendererInterface>, std::shared_ptr<SPK::GL::GLLineRenderer>>("GLLineRenderer",
		boost::python::init<boost::python::optional<float, float>>(
			":brief: Constructor of GLLineRenderer\n"
			":param length: the length multiplier of this GLLineRenderer\n"
			":param width: the width of this GLLineRenderer in pixels\n"))
        .def("getClassName", &SPK::GL::GLLineRenderer::getClassName)
        .def("create", &SPK::GL::GLLineRenderer::create, create_overload(
			":brief: Creates and registers a new GLLineRenderer\n"
			":param length: the length multiplier of this GLLineRenderer\n"
			":param width: the width of this GLLineRenderer in pixels\n"
			":return: A new registered GLLineRenderer\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .def("createBuffers", &SPK::GL::GLLineRenderer::createBuffers)
        .def("destroyBuffers", &SPK::GL::GLLineRenderer::destroyBuffers)
        .def("render", &SPK::GL::GLLineRenderer::render)
        .staticmethod("create")
        ;
}

// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_GLLineTrailRenderer.h


#include <RenderingAPIs/OpenGL/SPK_GLLineTrailRenderer.h>
#include <Core/SPK_Group.h>
#include <boost/python.hpp>


class GLLineTrailRendererWrapper :
    public SPK::GL::GLLineTrailRenderer,
    public boost::python::wrapper<SPK::GL::GLLineTrailRenderer>
{
public:
    using SPK::GL::GLLineTrailRenderer::GLLineTrailRenderer;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return GLLineTrailRenderer::getClassName();
        }
    }
    void enableBlending (bool blendingEnabled) {
        if ( auto enableBlending = this->get_override("enableBlending") ) {
            enableBlending(blendingEnabled);
        } else {
            GLLineTrailRenderer::enableBlending(blendingEnabled);
        }
    }
    void createBuffers (const SPK::Group & group) {
        if ( auto createBuffers = this->get_override("createBuffers") ) {
            createBuffers(group);
        } else {
            GLLineTrailRenderer::createBuffers(group);
        }
    }
    void destroyBuffers (const SPK::Group & group) {
        if ( auto destroyBuffers = this->get_override("destroyBuffers") ) {
            destroyBuffers(group);
        } else {
            GLLineTrailRenderer::destroyBuffers(group);
        }
    }
    void render (const SPK::Group & group) {
        if ( auto render = this->get_override("render") ) {
            render(group);
        } else {
            GLLineTrailRenderer::render(group);
        }
    }
    bool checkBuffers (const SPK::Group & group) {
        if ( auto checkBuffers = this->get_override("_checkBuffers") ) {
            return checkBuffers(group);
        } else {
            return GLLineTrailRenderer::checkBuffers(group);
        }
    }
};

void init_SPK_GLLineTrailRenderer_h() {

	void (SPK::GL::GLLineTrailRenderer::*init0)(const SPK::Group&) = &SPK::GL::GLLineTrailRenderer::init;

    boost::python::class_<GLLineTrailRendererWrapper, boost::python::bases<SPK::GL::GLRenderer>, std::shared_ptr<SPK::GL::GLLineTrailRenderer>>("GLLineTrailRenderer",
		":brief: Default constructor of GLLineTrailRenderer")
        .def("getClassName", &SPK::GL::GLLineTrailRenderer::getClassName)
        .def("create", &SPK::GL::GLLineTrailRenderer::create,
			":brief: Creates and registers a new GLLineTrailRenderer\n"
			":return: A new registered GLLineTrailRenderer\n"
			":since: 1.04.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
        .def("setNbSamples", &SPK::GL::GLLineTrailRenderer::setNbSamples,
			":brief: Sets the number of samples in a trail\n"
			"\n"
			"The number of samples defines the number of points used to construct the trail.\n"
			"The bigger the number of samples, the smoother the trail but the bigger the compution time and the memory consumption.\n"
			"\n"
			":param nbSamples: the number of samples to construct the trails\n")
        .def("setWidth", &SPK::GL::GLLineTrailRenderer::setWidth,
			":brief: Sets the width of a trail\n"
			"\n"
			"Like for GLLineRenderer, the width is defined in pixels and is not dependant of the distance of the trail from the camera\n"
			"\n"
			":param width: the width of trails in pixels\n")
        .def("setDuration", &SPK::GL::GLLineTrailRenderer::setDuration,
			":brief: Sets the duration of a sample\n"
			"\n"
			"The duration of a sample is defined by its life time from its creation to its destruction.\n"
			"Note that the alpha of a sample will decrease linearly from its initial alpha to 0.\n"
			"\n"
			":param duration: the duration of a sample\n")
        .def("setDegeneratedLines", &SPK::GL::GLLineTrailRenderer::setDegeneratedLines,
			":brief: Sets the color components of degenerated lines\n"
			":param r: the red component\n"
			":param g: the green component\n"
			":param b: the blue component\n"
			":param a: the alpha component\n")
        .def("enableBlending", &SPK::GL::GLLineTrailRenderer::enableBlending)
        .def("getNbSamples", &SPK::GL::GLLineTrailRenderer::getNbSamples,
			":brief: Gets the number of samples per trail\n"
			":return: the number of samples per trail\n")
        .def("getWidth", &SPK::GL::GLLineTrailRenderer::getWidth,
			":brief: Gets the width of a trail\n"
			":return: the width of a trail (in pixels)\n")
        .def("getDuration", &SPK::GL::GLLineTrailRenderer::getDuration,
			":brief: Gets the duration of a sample\n"
			":return: the duration of a sample\n")
        .def("createBuffers", &SPK::GL::GLLineTrailRenderer::createBuffers)
        .def("destroyBuffers", &SPK::GL::GLLineTrailRenderer::destroyBuffers)
        .def("init", init0,
			":brief: Inits all the trails of the particle of the group\n"
			"\n"
			"All the samples are set to the current position of the particle.\n"
			"The trail of each particle has therefore a length of 0 and is ready for update.\n"
			"This function allows to clear the buffers for GLLineTrailRenderer of the given group.\n"
			"\n"
			":param group: the Group whose buffers need to be initialized\n")
        .def("render", &SPK::GL::GLLineTrailRenderer::render)
        .staticmethod("create")
        ;
}

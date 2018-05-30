// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_GLQuadRenderer.h


#include <RenderingAPIs/OpenGL/SPK_GLQuadRenderer.h>
#include <boost/python.hpp>


class GLQuadRendererWrapper :
	public SPK::GL::GLQuadRenderer,
	public boost::python::wrapper<SPK::GL::GLQuadRenderer>
{
public:
	using SPK::GL::GLQuadRenderer::GLQuadRenderer;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return GLQuadRenderer::getClassName();
		}
	}
	bool setTexturingMode(SPK::TexturingMode mode) {
		if (auto setTexturingMode = this->get_override("setTexturingMode")) {
			return setTexturingMode(mode);
		}
		else {
			return GLQuadRenderer::setTexturingMode(mode);
		}
	}
	void createBuffers(const SPK::Group & group) {
		if (auto createBuffers = this->get_override("createBuffers")) {
			createBuffers(group);
		}
		else {
			GLQuadRenderer::createBuffers(group);
		}
	}
	void destroyBuffers(const SPK::Group & group) {
		if (auto destroyBuffers = this->get_override("destroyBuffers")) {
			destroyBuffers(group);
		}
		else {
			GLQuadRenderer::destroyBuffers(group);
		}
	}
	void render(const SPK::Group & group) {
		if (auto render = this->get_override("render")) {
			render(group);
		}
		else {
			GLQuadRenderer::render(group);
		}
	}
	bool checkBuffers(const SPK::Group & group) {
		if (auto checkBuffers = this->get_override("_checkBuffers")) {
			return checkBuffers(group);
		}
		else {
			return GLQuadRenderer::checkBuffers(group);
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::GL::GLQuadRenderer::create, 0, 2)

void init_SPK_GLQuadRenderer_h() {
	boost::python::class_<GLQuadRendererWrapper, boost::python::bases<SPK::GL::GLRenderer, SPK::QuadRendererInterface, SPK::Oriented3DRendererInterface, SPK::GL::GLExtHandler>,
		std::shared_ptr<SPK::GL::GLQuadRenderer>>("GLQuadRenderer",
			boost::python::init<boost::python::optional<float, float>>(
				":brief: Constructor of GLQuadRenderer\n"
				":param: scaleX the scale of the width of the quad\n"
				":param: scaleY the scale of the height of the quad\n"))
		.def("getClassName", &SPK::GL::GLQuadRenderer::getClassName)
		.def("create", &SPK::GL::GLQuadRenderer::create, create_overload(
			":brief: Creates and registers a new GLQuadRenderer\n"
			":param: scaleX the scale of the width of the quad\n"
			":param: scaleY the scale of the height of the quad\n"
			":return: A new registered GLQuadRenderer\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setTexturingMode", &SPK::GL::GLQuadRenderer::setTexturingMode)
		.def("setTexture", &SPK::GL::GLQuadRenderer::setTexture)
		.def("getTexture", &SPK::GL::GLQuadRenderer::getTexture,
			":brief: Gets the texture of this GLQuadRenderer\n"
			":return: the texture of this GLQuadRenderer\n")
		.def("createBuffers", &SPK::GL::GLQuadRenderer::createBuffers)
		.def("destroyBuffers", &SPK::GL::GLQuadRenderer::destroyBuffers)
		.def("render", &SPK::GL::GLQuadRenderer::render)
		.staticmethod("create")
		;
}

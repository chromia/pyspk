// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_GLRenderer.h


#include <RenderingAPIs/OpenGL/SPK_GLRenderer.h>
#include <Core/SPK_Group.h>
#include <boost/python.hpp>


class GLRendererWrapper :
	public SPK::GL::GLRenderer,
	public boost::python::wrapper<SPK::GL::GLRenderer>
{
public:
	using SPK::GL::GLRenderer::GLRenderer;

	std::string getClassName() const
	{
		return this->get_override("getClassName")();
	}
	Registerable * clone(bool createBase) const
	{
		return this->get_override("clone")(createBase);
	}

	void enableBlending(bool blendingEnabled) {
		if (auto enableBlending = this->get_override("enableBlending")) {
			enableBlending(blendingEnabled);
		}
		else {
			GLRenderer::enableBlending(blendingEnabled);
		}
	}
	void setBlending(SPK::BlendingMode blendMode) {
		if (auto setBlending = this->get_override("setBlending")) {
			setBlending(blendMode);
		}
		else {
			GLRenderer::setBlending(blendMode);
		}
	}

	void render(const SPK::Group& group)
	{
		this->get_override("render")(group);
	}
};

void init_SPK_GLRenderer_h() {
	boost::python::class_<GLRendererWrapper, boost::python::bases<SPK::Renderer>, boost::noncopyable>("GLRenderer")
		.def("enableBlending", &SPK::GL::GLRenderer::enableBlending)
		.def("setBlendingFunctions", &SPK::GL::GLRenderer::setBlendingFunctions)
		.def("setBlending", &SPK::GL::GLRenderer::setBlending)
		.def("setTextureBlending", &SPK::GL::GLRenderer::setTextureBlending)
		.def("isBlendingEnabled", &SPK::GL::GLRenderer::isBlendingEnabled)
		.def("getSrcBlendingFunction", &SPK::GL::GLRenderer::getSrcBlendingFunction)
		.def("getDestBlendingFunction", &SPK::GL::GLRenderer::getDestBlendingFunction)
		.def("getTextureBlending", &SPK::GL::GLRenderer::getTextureBlending)
		.def("saveGLStates", &SPK::GL::GLRenderer::saveGLStates)
		.def("restoreGLStates", &SPK::GL::GLRenderer::restoreGLStates)
		.staticmethod("saveGLStates")
		.staticmethod("restoreGLStates")
		;
}

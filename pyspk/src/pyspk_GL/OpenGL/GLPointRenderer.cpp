// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_GLPointRenderer.h


#include <RenderingAPIs/OpenGL/SPK_GLPointRenderer.h>
#include <Core/SPK_Group.h>
#include <boost/python.hpp>


class GLPointRendererWrapper :
	public SPK::GL::GLPointRenderer,
	public boost::python::wrapper<SPK::GL::GLPointRenderer>
{
public:
	using SPK::GL::GLPointRenderer::GLPointRenderer;
	std::string getClassName() const {
		if (auto getClassName = this->get_override("getClassName")) {
			return getClassName();
		}
		else {
			return GLPointRenderer::getClassName();
		}
	}
	bool setType(SPK::PointType type) {
		if (auto setType = this->get_override("setType")) {
			return setType(type);
		}
		else {
			return GLPointRenderer::setType(type);
		}
	}
	void render(const SPK::Group & group) {
		if (auto render = this->get_override("render")) {
			render(group);
		}
		else {
			GLPointRenderer::render(group);
		}
	}
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::GL::GLPointRenderer::create , 0, 1)

void init_SPK_GLPointRenderer_h() {
	boost::python::class_<GLPointRendererWrapper, boost::python::bases<SPK::GL::GLRenderer, SPK::PointRendererInterface, SPK::GL::GLExtHandler>, 
		std::shared_ptr<SPK::GL::GLPointRenderer>>( "GLPointRenderer",
			boost::python::init<boost::python::optional<float>>(
				":brief: Constructor of GLPointRenderer\n"
				":param size: the size of the points\n"
				))
		.def("getClassName", &SPK::GL::GLPointRenderer::getClassName)
		.def("create", &SPK::GL::GLPointRenderer::create, create_overload(
			":brief: Creates and registers a new GLPointRenderer\n"
			":param size: the size of the points\n"
			":return: A new registered GLPointRenderer\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
		.def("setType", &SPK::GL::GLPointRenderer::setType)
		.def("setTexture", &SPK::GL::GLPointRenderer::setTexture,
			":brief: Sets the texture of this GLPointRenderer\n"
			"\n"
			"Note that the texture is only used if point sprites are used (type is set to POINT_SPRITE)\n"
			"\n"
			":param textureIndex: the index of the openGL texture of this GLPointRenderer\n")
		.def("enableWorldSize", &SPK::GL::GLPointRenderer::enableWorldSize,
			":brief: Sets the way size of points is computed in this GLPointRenderer\n"
			"\n"
			"if universe size is used (True), the extension is checked.\n"
			"if universe size is not supported by the hardware, False is returned and nothing happens.\n"
			"\n"
			"If world size is enabled, the static method setPixelPetUnit(float,int) must be called to set the conversion between pixels and world units.\n"
			"\n"
			":param worldSizeEnabled: True to enable universe size, False to use screen size\n"
			":return: True the type of size can be set, False otherwise\n")
		.def("getTexture", &SPK::GL::GLPointRenderer::getTexture,
			":brief: Gets the texture of this GLPointRenderer\n"
			":return: the texture of this GLPointRenderer\n")
		.def("isWorldSizeEnabled", &SPK::GL::GLPointRenderer::isWorldSizeEnabled,
			":brief: Tells whether world size is enabled or not in this GLPointRenderer\n"
			":return: True if world size is enabled, False if not\n")
		.def("render", &SPK::GL::GLPointRenderer::render)
		.staticmethod("create")
		;
}

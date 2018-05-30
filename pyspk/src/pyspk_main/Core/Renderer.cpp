// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Renderer.h


#include "Core/SPK_Renderer.h"
#include "Core/SPK_Group.h"
#include "Core/SPK_Particle.h"
#include <boost/python.hpp>


class RendererWrapper :
	public SPK::Renderer,
	public boost::python::wrapper<SPK::Renderer>
{
public:
	using Renderer = SPK::Renderer;

	std::string getClassName() const
	{
		return this->get_override("getClassName")();
	}
	Registerable * clone(bool createBase) const
	{
		return this->get_override("clone")(createBase);
	}

	void setBlending(SPK::BlendingMode blendMode) {
		this->get_override("setBlending")(blendMode);
	}
	void enableRenderingHint(SPK::RenderingHint renderingHint, bool enable) {
		if (auto enableRenderingHint = this->get_override("enableRenderingHint")) {
			enableRenderingHint(renderingHint, enable);
		}
		else {
			Renderer::enableRenderingHint(renderingHint, enable);
		}
	}
	void setAlphaTestThreshold(float alphaThreshold) {
		if (auto setAlphaTestThreshold = this->get_override("setAlphaTestThreshold")) {
			setAlphaTestThreshold(alphaThreshold);
		}
		else {
			Renderer::setAlphaTestThreshold(alphaThreshold);
		}
	}
	bool isRenderingHintEnabled(SPK::RenderingHint renderingHint) const {
		if (auto isRenderingHintEnabled = this->get_override("isRenderingHintEnabled")) {
			return isRenderingHintEnabled(renderingHint);
		}
		else {
			return Renderer::isRenderingHintEnabled(renderingHint);
		}
	}
	void render(const SPK::Group & group) {
		this->get_override("render")(group);
	}
};

void init_SPK_Renderer_h() {
	boost::python::class_<RendererWrapper, boost::python::bases<SPK::Registerable, SPK::BufferHandler>, boost::noncopyable>("Renderer")
		.def("setActive", &SPK::Renderer::setActive,
			":brief: Sets this Renderer active or not.\n"
			"\n"
			"An inactive Renderer will render its parent Group when a call to Group.render() is made.\n"
			"However it can still be used manually by the user with render(Group).\n"
			"\n"
			":param active: True to activate this Renderer, False to deactivate it\n"
			":since: 1.03.00\n")
		.def("setBlending", boost::python::pure_virtual(&SPK::Renderer::setBlending),
			":brief: Sets the blending mode of this renderer\n"
			"\n"
			"This is a generic method that allows to set most common blending modes in a generic way.\n"
			"However renderers can implement method to give more control over the blending mode used.\n"
			"\n"
			":param blendMode: the blending mode to use\n"
			":since: 1.04.00\n")
		.def("enableRenderingHint", &SPK::Renderer::enableRenderingHint,
			":brief: Enables or disables a rendering hint\n"
			"\n"
			"Note that as stated, these are only hints that may not be taken into account in all rendering APIs\n"
			"\n"
			":param renderingHint: the renderingHint to enable or disable\n"
			":param enable: True to enable it, False to disable it\n"
			":since: 1.04.00\n")
		.def("setAlphaTestThreshold", &SPK::Renderer::setAlphaTestThreshold,
			":brief: Tells the alpha threshold to use when the ALPHA_TEST is enabled\n"
			"\n"
			"The operation performs by the alpha test is <i>greater or equal to threshold</i>\n"
			"\n"
			":param alphaThreshold: the alpha threshold to use for the alpha test\n"
			":since: 1.04.00\n")
		.def("isActive", &SPK::Renderer::isActive,
			":brief: Tells whether this Renderer is active or not\n"
			":return: True if this Renderer is active, False if is is inactive\n"
			":since: 1.03.00\n")
		.def("isRenderingHintEnabled", &SPK::Renderer::isRenderingHintEnabled,
			":brief: Tells whether a rendering hint is enabled or not\n"
			":param renderingHint: the rendering hint\n"
			":since: 1.04.00\n")
		.def("getAlphaTestThreshold", &SPK::Renderer::getAlphaTestThreshold,
			":brief: Gets the alpha threhold used by the alpha test\n"
			":return: the alpha threhold used by the alpha test\n"
			":since: 1.04.00\n")
		.def("render", boost::python::pure_virtual(&SPK::Renderer::render),
			":brief: Renders a Group of particles\n"
			":param group: the Group to render\n")
		.def("getClassName", boost::python::pure_virtual(&SPK::Renderer::getClassName))
		;
	{
		boost::python::enum_<SPK::BlendingMode>("BlendingMode",
			":enum: BlendingMode\n"
			":brief: Constants defining the available blending modes\n"
			":since: 1.04.00\n")
			.value("BLENDING_NONE", SPK::BLENDING_NONE)
			.value("BLENDING_ADD", SPK::BLENDING_ADD)
			.value("BLENDING_ALPHA", SPK::BLENDING_ALPHA)
			.export_values()
			;
	}
	{
		boost::python::enum_<SPK::RenderingHint>("RenderingHint",
			":enum: RenderingHint\n"
			":brief: Constants defining the available rendering hints\n"
			":since: 1.04.00\n")
			.value("ALPHA_TEST", SPK::ALPHA_TEST)
			.value("DEPTH_TEST", SPK::DEPTH_TEST)
			.value("DEPTH_WRITE", SPK::DEPTH_WRITE)
			.export_values()
			;
	}
}

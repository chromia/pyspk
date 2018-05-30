// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_BufferHandler.h


#include <Core/SPK_BufferHandler.h>
#include <Core/SPK_Group.h>
#include <boost/python.hpp>


class BufferHandlerWrapper :
	public SPK::BufferHandler,
	public boost::python::wrapper<SPK::BufferHandler>
{
public:
	using SPK::BufferHandler::BufferHandler;
	void createBuffers(const SPK::Group & group) {
		if (auto createBuffers = this->get_override("createBuffers")) {
			createBuffers(group);
		}
		else {
			BufferHandler::createBuffers(group);
		}
	}
	void destroyBuffers(const SPK::Group & group) {
		if (auto destroyBuffers = this->get_override("destroyBuffers")) {
			destroyBuffers(group);
		}
		else {
			BufferHandler::destroyBuffers(group);
		}
	}
	/*bool checkBuffers(const SPK::Group & group) {
		if (auto checkBuffers = this->get_override("_checkBuffers")) {
			return checkBuffers(group);
		}
		else {
			return BufferHandler::checkBuffers(group);
		}
	}*/
};

void init_SPK_BufferHandler_h() {
	boost::python::class_<BufferHandlerWrapper, boost::noncopyable>("BufferHandler",
		":class: BufferHandler\n"
		":brief: A base interface that allows manipulation on Buffer\n"
		":since: 1.04.00\n"
		)
		.def("enableBuffersCreation", &SPK::BufferHandler::enableBuffersCreation,
			":brief: Enables or disables the automatic buffers creation in a static way\n"
			"\n"
			"By default, the buffers creation is enabled.\n"
			"\n"
			":param creation: true to enable the buffers creation, false to disable it\n")
		.def("isBuffersCreationEnabled", &SPK::BufferHandler::isBuffersCreationEnabled,
			":brief: Tells whether the automatic buffers creation is enabled or not\n"
			":return: True if the buffers creation is enabled, False if it is disabled\n")
		.def("createBuffers", &SPK::BufferHandler::createBuffers, &BufferHandlerWrapper::createBuffers,
			":brief: Creates the buffers for this buffer handler in the given group\n"
			"\n"
			"If the buffers for this type of buffer handler already exists within the Group, they are priorly destroyed.\n"
			"The type of buffers created depends on the state of the buffer handler at the time this method is called.\n"
			"\n"
			"This method has to be overridden in derived classes that use buffers\n"
			"\n"
			":param group: the Group in which to create the buffers for this buffer handler\n")
		.def("destroyBuffers", &SPK::BufferHandler::destroyBuffers, &BufferHandlerWrapper::destroyBuffers,
			":brief: Destroys the buffers for this buffer handler in the given group\n"
			"\n"
			"if the buffers dont exist, nothing happens.\n"
			"\n"
			"This method has to be overridden in derived classes that use buffers\n"
			"\n"
			":param group: the Group in which to destroy the buffers for this buffer handler\n")
		.staticmethod("enableBuffersCreation")
		.staticmethod("isBuffersCreationEnabled")
		;
}

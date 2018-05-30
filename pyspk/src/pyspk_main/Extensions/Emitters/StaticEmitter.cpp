// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_StaticEmitter.h


#include <Extensions/Emitters/SPK_StaticEmitter.h>
#include <boost/python.hpp>


class StaticEmitterWrapper :
    public SPK::StaticEmitter,
    public boost::python::wrapper<SPK::StaticEmitter>
{
public:
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return StaticEmitter::getClassName();
        }
    }
};

void init_SPK_StaticEmitter_h() {
    boost::python::class_<StaticEmitterWrapper, boost::python::bases<SPK::Emitter>, std::shared_ptr<SPK::StaticEmitter>>("StaticEmitter")
        .def("getClassName", &SPK::StaticEmitter::getClassName)
        .def("create", &SPK::StaticEmitter::create,
			":brief: Creates and registers a new StaticEmitter\n"
			":return: A new registered StaticEmitter\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
        .staticmethod("create")
        ;
}

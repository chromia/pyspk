// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_RandomEmitter.h


#include <Extensions/Emitters/SPK_RandomEmitter.h>
#include <boost/python.hpp>


class RandomEmitterWrapper :
    public SPK::RandomEmitter,
    public boost::python::wrapper<SPK::RandomEmitter>
{
public:
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return RandomEmitter::getClassName();
        }
    }
};

void init_SPK_RandomEmitter_h() {
    boost::python::class_<RandomEmitterWrapper, boost::python::bases<SPK::Emitter>, std::shared_ptr<SPK::RandomEmitter>>("RandomEmitter")
        .def("getClassName", &SPK::RandomEmitter::getClassName)
        .def("create", &SPK::RandomEmitter::create,
			":brief: Creates and registers a new RandomEmitter\n"
			":return: A new registered RandomEmitter\n"
			":since: 1.04.00\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
        .staticmethod("create")
        ;
}

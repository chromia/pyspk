// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Rotator.h


#include <Extensions/Modifiers/SPK_Rotator.h>
#include <boost/python.hpp>


class RotatorWrapper :
    public SPK::Rotator,
    public boost::python::wrapper<SPK::Rotator>
{
public:
    using SPK::Rotator::Rotator;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return Rotator::getClassName();
        }
    }
};

void init_SPK_Rotator_h() {
    boost::python::class_<RotatorWrapper, boost::python::bases<SPK::Modifier>, std::shared_ptr<SPK::Rotator>>("Rotator")
        .def("getClassName", &SPK::Rotator::getClassName)
        .def("create", &SPK::Rotator::create,
			":brief: Creates and registers a new Rotator\n"
			":return: A new registered Rotator\n",
			boost::python::return_value_policy<boost::python::reference_existing_object>())
        .staticmethod("create")
        ;
}

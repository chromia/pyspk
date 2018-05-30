// generate by pypp <https://github.com/mugwort-rc/pypp>
// original source code: SPK_Destroyer.h


#include <Extensions/Modifiers/SPK_Destroyer.h>
#include <boost/python.hpp>


class DestroyerWrapper :
    public SPK::Destroyer,
    public boost::python::wrapper<SPK::Destroyer>
{
public:
    using SPK::Destroyer::Destroyer;
    std::string getClassName () const {
        if ( auto getClassName = this->get_override("getClassName") ) {
            return getClassName();
        } else {
            return Destroyer::getClassName();
        }
    }
};

BOOST_PYTHON_FUNCTION_OVERLOADS(create_overload, SPK::Destroyer::create, 0, 2)

void init_SPK_Destroyer_h() {
    boost::python::class_<DestroyerWrapper, boost::python::bases<SPK::Modifier>, std::shared_ptr<SPK::Destroyer>>("Destroyer",
		boost::python::init<boost::python::optional<SPK::Zone *, SPK::ModifierTrigger>>(
			":brief: Constructor of Destroyer\n"
			":param zone: the Zone of the Destroyer\n"
			":param trigger: the trigger of the Destroyer\n"))
        .def("getClassName", &SPK::Destroyer::getClassName)
        .def("create", &SPK::Destroyer::create, create_overload(
			":brief: Creates and registers a new Destroyer\n"
			":param zone: the Zone of the Destroyer\n"
			":param trigger: the trigger of the Destroyer\n"
			":return: A new registered Destroyer\n"
			":since: 1.04.00\n")
			[boost::python::return_value_policy<boost::python::reference_existing_object>()])
        .staticmethod("create")
        ;
}

#include <nrhi/command_list_base.hpp>



namespace nrhi {

    A_command_list::A_command_list(TK_valid<A_device> device_p, const F_command_list_desc& desc) :
        device_p_(device_p),
        desc_(desc)
    {

    }
    A_command_list::~A_command_list(){

    }

}
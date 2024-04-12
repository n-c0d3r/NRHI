#include <nrhi/command_queue_base.hpp>



namespace nrhi {

    A_command_queue::A_command_queue(TK_valid<A_device> device_p, const F_command_queue_desc& desc) :
        device_p_(device_p),
        desc_(desc)
    {

    }
    A_command_queue::~A_command_queue(){

    }

}
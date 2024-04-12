#include <nrhi/directx11/command_queue.hpp>



namespace nrhi {

    F_directx11_command_queue::F_directx11_command_queue(TK_valid<A_device> device_p, const F_command_queue_desc& desc) :
        A_command_queue(device_p, desc)
    {

    }
    F_directx11_command_queue::~F_directx11_command_queue(){

    }



    TU<A_command_queue> HD_directx11_command_queue::create(TK_valid<A_device> device_p, const F_command_queue_desc& desc){

        return TU<F_directx11_command_queue>()(device_p, desc);
    }

    void HD_directx11_command_queue::execute_command_lists(
        TK_valid<A_command_queue> command_queue_p,
        TG_span<TK_valid<A_command_list>> command_list_p_span
    ) {

    }

}
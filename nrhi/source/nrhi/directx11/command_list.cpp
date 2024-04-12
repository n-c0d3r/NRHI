#include <nrhi/directx11/command_list.hpp>



namespace nrhi {

    F_directx11_command_list::F_directx11_command_list(TK_valid<A_device> device_p, const F_command_list_desc& desc) :
        A_command_list(device_p, desc)
    {

    }
    F_directx11_command_list::~F_directx11_command_list(){

    }



    TU<A_command_list> HD_directx11_command_list::create(TK_valid<A_device> device_p, const F_command_list_desc& desc){

        return TU<F_directx11_command_list>()(device_p, desc);
    }

}
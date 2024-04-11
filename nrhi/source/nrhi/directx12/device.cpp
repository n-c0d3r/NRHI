#include <nrhi/directx12/device.hpp>



namespace nrhi {

    F_directx12_device::F_directx12_device(TK_valid<A_adapter> adapter_p) :
        A_device(adapter_p)
    {

    }
    F_directx12_device::~F_directx12_device(){

    }



    TU_valid<A_device> HD_directx12_device::create(TK_valid<A_adapter> adapter_p){

        return TU_valid<F_directx12_device>::T_make(adapter_p);
    }

}
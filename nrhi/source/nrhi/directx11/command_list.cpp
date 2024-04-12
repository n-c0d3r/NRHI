#include <nrhi/directx11/command_list.hpp>
#include <nrhi/directx11/device.hpp>



namespace nrhi {

    F_directx11_command_list::F_directx11_command_list(TK_valid<A_device> device_p, const F_command_list_desc& desc) :
        A_command_list(device_p, desc)
    {

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

        HRESULT hr = d3d11_device_p->CreateDeferredContext(
            0,
            &d3d11_device_context_p_
        );

        NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 deferred context";

    }
    F_directx11_command_list::~F_directx11_command_list(){

    }



    TU<A_command_list> HD_directx11_command_list::create(TK_valid<A_device> device_p, const F_command_list_desc& desc){

        return TU<F_directx11_command_list>()(device_p, desc);
    }

}
#include <nrhi/directx12/command_list.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/command_allocator.hpp>



namespace nrhi {

    F_directx12_command_list::F_directx12_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc) :
        A_command_list(device_p, desc)
    {
		// setup command allocator
		if(desc.manual_command_allocator_p)
		{
			command_allocator_p_ = desc.manual_command_allocator_p;
		}
		else
		{
			owned_command_allocator_p_ = H_command_allocator::create(
				device_p,
				{ desc.type }
			);
			command_allocator_p_ = owned_command_allocator_p_.keyed();
		}

		// create d3d12 command list
		device_p.T_cast<F_directx12_device>()->d3d12_device_p()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE(desc.type),
			command_allocator_p_.T_cast<F_directx12_command_allocator>()->d3d12_command_allocator_p(),
			0,
			IID_PPV_ARGS(&d3d12_command_list_p_)
		);
		NCPP_ASSERT(d3d12_command_list_p_) << "create d3d12 command list failed";

		HRESULT hr = d3d12_command_list_p_->Close();
    	NCPP_ASSERT(!FAILED(hr)) << "can't close command list";
	}
    F_directx12_command_list::~F_directx12_command_list(){

		if(d3d12_command_list_p_)
			d3d12_command_list_p_->Release();
    }



    TU<A_command_list> HD_directx12_command_list::create(TKPA_valid<A_device> device_p, const F_command_list_desc& desc){

        return TU<F_directx12_command_list>()(device_p, desc);
    }

}
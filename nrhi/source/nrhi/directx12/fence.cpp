#include <nrhi/directx12/fence.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi {

	F_directx12_fence::F_directx12_fence(
		TKPA_valid<A_device> device_p,
		const F_fence_desc& desc
	) :
		A_fence(
			device_p,
			desc
		)
	{
		device_p.T_cast<F_directx12_device>()->d3d12_device_p()->CreateFence(
			desc.initial_value,
			D3D12_FENCE_FLAGS(desc.flags),
			IID_PPV_ARGS(&d3d12_fence_p_)
		);

		NCPP_ASSERT(d3d12_fence_p_) << "can't create d3d12 fence";
	}
	F_directx12_fence::~F_directx12_fence(){

		if(d3d12_fence_p_)
			d3d12_fence_p_->Release();
	}



	TU<A_fence> HD_directx12_fence::create(
		TKPA_valid<A_device> device_p,
		const F_fence_desc& desc
	){
		return TU<F_directx12_fence>()(
			device_p,
			desc
		);
	}

	u64 HD_directx12_fence::value(TKPA_valid<A_fence> fence_p) {

		auto dx12_fence_p = fence_p.T_cast<F_directx12_fence>();

		return dx12_fence_p->d3d12_fence_p_->GetCompletedValue();
	}
	void HD_directx12_fence::wait(TKPA_valid<A_fence> fence_p, u64 target_value){

		while(value(fence_p) != target_value);
	}
	b8 HD_directx12_fence::is_completed(TKPA_valid<A_fence> fence_p, u64 target_value){

		return (value(fence_p) == target_value);
	}

}
#include <nrhi/directx12/command_list.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/resource.hpp>
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

	void HD_directx12_command_list::async_resource_barrier(
		TKPA_valid<A_command_list> command_list_p,
		const F_resource_barrier& resource_barrier
	) {
		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		D3D12_RESOURCE_BARRIER d3d12_resource_barrier;
		d3d12_resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE(resource_barrier.type);
		d3d12_resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS(resource_barrier.flags);

		NRHI_ENUM_SWITCH(
			resource_barrier.type,
			NRHI_ENUM_CASE(
				ED_resource_barrier_type::TRANSITION,
				const auto& resource_transition_barrier = resource_barrier.transition;
				d3d12_resource_barrier.Transition.pResource = resource_transition_barrier.resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
				d3d12_resource_barrier.Transition.Subresource = resource_transition_barrier.subresource_index;
				d3d12_resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES(resource_transition_barrier.state_before);
				d3d12_resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES(resource_transition_barrier.state_after);
			)
			NRHI_ENUM_CASE(
				ED_resource_barrier_type::ALIASING,
				const auto& resource_aliasing_barrier = resource_barrier.aliasing;
				d3d12_resource_barrier.Aliasing.pResourceBefore = resource_aliasing_barrier.resource_before_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
				d3d12_resource_barrier.Aliasing.pResourceAfter = resource_aliasing_barrier.resource_after_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
			)
			NRHI_ENUM_CASE(
				ED_resource_barrier_type::UAV,
				const auto& resource_uav_barrier = resource_barrier.uav;
				d3d12_resource_barrier.UAV.pResource = resource_uav_barrier.resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
			)
		);

		dx12_command_list_p->d3d12_command_list_p()->ResourceBarrier(
			1,
			&d3d12_resource_barrier
		);
	}
	void HD_directx12_command_list::async_resource_barriers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<F_resource_barrier>& resource_barriers
	) {
		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		u32 barrier_count = resource_barriers.size();

		using F_resource_barrier_batch = TG_array<D3D12_RESOURCE_BARRIER, NRHI_MAX_RESOURCE_BARRIER_COUNT_PER_BATCH>;
		F_resource_barrier_batch batch;

		for(u32 i = 0; i < barrier_count; ++i)
		{
			const auto& resource_barrier = resource_barriers[i];

			D3D12_RESOURCE_BARRIER d3d12_resource_barrier;
			d3d12_resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE(resource_barrier.type);
			d3d12_resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS(resource_barrier.flags);

			NRHI_ENUM_SWITCH(
				resource_barrier.type,
				NRHI_ENUM_CASE(
					ED_resource_barrier_type::TRANSITION,
					const auto& resource_transition_barrier = resource_barrier.transition;
					d3d12_resource_barrier.Transition.pResource = resource_transition_barrier.resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
					d3d12_resource_barrier.Transition.Subresource = resource_transition_barrier.subresource_index;
					d3d12_resource_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES(resource_transition_barrier.state_before);
					d3d12_resource_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES(resource_transition_barrier.state_after);
				)
				NRHI_ENUM_CASE(
					ED_resource_barrier_type::ALIASING,
					const auto& resource_aliasing_barrier = resource_barrier.aliasing;
					d3d12_resource_barrier.Aliasing.pResourceBefore = resource_aliasing_barrier.resource_before_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
					d3d12_resource_barrier.Aliasing.pResourceAfter = resource_aliasing_barrier.resource_after_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
				)
				NRHI_ENUM_CASE(
					ED_resource_barrier_type::UAV,
					const auto& resource_uav_barrier = resource_barrier.uav;
					d3d12_resource_barrier.UAV.pResource = resource_uav_barrier.resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
				)
			);
		}

		dx12_command_list_p->d3d12_command_list_p()->ResourceBarrier(
			resource_barriers.size(),
			batch.data()
		);
	}

}
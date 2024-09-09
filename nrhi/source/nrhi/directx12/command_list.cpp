#include <nrhi/directx12/command_list.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/resource_view.hpp>
#include <nrhi/directx12/frame_buffer.hpp>
#include <nrhi/directx12/descriptor_heap.hpp>
#include <nrhi/directx12/root_signature.hpp>
#include <nrhi/directx12/pipeline_state.hpp>
#include <nrhi/directx12/command_signature.hpp>
#include <nrhi/directx12/pix_runtime.hpp>
#include <nrhi/command_allocator.hpp>
#include <nrhi/command_list.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx12_command_list::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		auto wvalue = G_to_wstring(value.c_str());
		d3d12_command_list_p_->SetName(wvalue.data());
	}
#endif



    F_directx12_command_list::F_directx12_command_list(
		TKPA_valid<A_device> device_p,
		const F_command_list_desc& desc,
		F_directx12_command_list_create_with_command_allocator_flag
	) :
        A_command_list(device_p, desc)
    {
		command_allocator_p_ = desc.command_allocator_p;

		// create d3d12 command list
		device_p.T_cast<F_directx12_device>()->d3d12_device_p()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE(desc.type),
			command_allocator_p_.T_cast<F_directx12_command_allocator>()->d3d12_command_allocator_p(),
			0,
			IID_PPV_ARGS(&d3d12_command_list_p_)
		);
		NCPP_ASSERT(d3d12_command_list_p_) << "create d3d12 command list failed";
	}
    F_directx12_command_list::~F_directx12_command_list(){

		if(d3d12_command_list_p_)
			d3d12_command_list_p_->Release();
    }



#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void HD_directx12_command_list::begin_event(
		TKPA_valid<A_command_list> command_list_p,
		PA_vector3_f32 color,
		const F_debug_name& name
	)
	{
		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();
		HD_directx12_pix_runtime::begin_event_on_command_list()(
			dx12_command_list_p->d3d12_command_list_p(),
			HD_directx12_pix_runtime::to_pix_color(color),
			name.c_str()
		);
	}
	void HD_directx12_command_list::end_event(TKPA_valid<A_command_list> command_list_p)
	{
		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();
		HD_directx12_pix_runtime::end_event_on_command_list()(
			dx12_command_list_p->d3d12_command_list_p()
		);
	}
#endif

    TU<A_command_list> HD_directx12_command_list::create_with_command_allocator(TKPA_valid<A_device> device_p, const F_command_list_desc& desc){

        return TU<F_directx12_command_list>()(device_p, desc, F_directx12_command_list_create_with_command_allocator_flag {});
    }

	void HD_directx12_command_list::async_begin(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_command_allocator> command_allocator_p
	) {
		NCPP_ASSERT(!(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_)) << "already in record";
		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			command_list_p.T_cast<F_directx12_command_list>()->is_in_record_ = true;
		);
		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->Reset(
			command_allocator_p.T_cast<F_directx12_command_allocator>()->d3d12_command_allocator_p(),
			0
		);
	}
	void HD_directx12_command_list::async_end(TKPA_valid<A_command_list> command_list_p) {

		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			command_list_p.T_cast<F_directx12_command_list>()->is_in_record_ = false;
		);
		HRESULT hr = command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->Close();
		NCPP_ASSERT(!FAILED(hr)) << "invalid record";
	}

	void HD_directx12_command_list::async_resource_barrier(
		TKPA_valid<A_command_list> command_list_p,
		const F_resource_barrier& resource_barrier
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

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
				ED_resource_barrier_type::UNORDERED_ACCESS,
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
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		u32 barrier_count = resource_barriers.size();

		D3D12_RESOURCE_BARRIER batch[NRHI_MAX_RESOURCE_BARRIER_COUNT_PER_BATCH];

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
					ED_resource_barrier_type::UNORDERED_ACCESS,
					const auto& resource_uav_barrier = resource_barrier.uav;
					d3d12_resource_barrier.UAV.pResource = resource_uav_barrier.resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
				)
			);

			batch[i] = d3d12_resource_barrier;
		}

		dx12_command_list_p->d3d12_command_list_p()->ResourceBarrier(
			resource_barriers.size(),
			batch
		);
	}

	void HD_directx12_command_list::clear_state(
		TKPA_valid<A_command_list> command_list_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		dx12_command_list_p->d3d12_command_list_p()->ClearState(0);
	}
	void HD_directx12_command_list::async_clear_rtv(
		TKPA_valid<A_command_list> command_list_p,
		K_valid_rtv_handle rtv_p,
		PA_vector4_f32 color
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		async_clear_rtv_with_descriptor(
			command_list_p,
			rtv_p->descriptor_handle().cpu_address,
			color
		);
	}
	void HD_directx12_command_list::async_clear_dsv(
		TKPA_valid<A_command_list> command_list_p,
		K_valid_dsv_handle dsv_p,
		ED_clear_flag flag,
		f32 depth,
		u8 stencil
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		async_clear_dsv_with_descriptor(
			command_list_p,
			dsv_p->descriptor_handle().cpu_address,
			flag,
			depth,
			stencil
		);
	}
	void HD_directx12_command_list::async_clear_rtv_with_descriptor(
		TKPA_valid<A_command_list> command_list_p,
		F_descriptor_cpu_address rtv_cpu_address,
		PA_vector4_f32 color
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		dx12_command_list_p->d3d12_command_list_p()->ClearRenderTargetView(
			{ rtv_cpu_address },
			(const float*)&color,
			0,
			0
		);
	}
	void HD_directx12_command_list::async_clear_dsv_with_descriptor(
		TKPA_valid<A_command_list> command_list_p,
		F_descriptor_cpu_address dsv_cpu_address,
		ED_clear_flag flag,
		f32 depth,
		u8 stencil
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		dx12_command_list_p->d3d12_command_list_p()->ClearDepthStencilView(
			{ dsv_cpu_address },
			D3D12_CLEAR_FLAGS(flag),
			depth,
			stencil,
			0,
			0
		);
	}

	void HD_directx12_command_list::bind_descriptor_heaps(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<TK_valid<A_descriptor_heap>>& descriptor_heap_p_span
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics() || command_list_p->supports_compute()) << "command list does not support graphics or compute";

		ID3D12DescriptorHeap* d3d12_descriptor_heap_p_array[2];

		u8 descriptor_heap_count = descriptor_heap_p_span.size();
		for(u8 i = 0; i < descriptor_heap_count; ++i) {

			d3d12_descriptor_heap_p_array[i] = descriptor_heap_p_span[i].T_cast<F_directx12_descriptor_heap>()->d3d12_descriptor_heap_p();
		}

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetDescriptorHeaps(
			descriptor_heap_count,
			d3d12_descriptor_heap_p_array
		);
	}

	void HD_directx12_command_list::bind_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_pipeline_state> pipeline_state_p
	) {
		NCPP_ASSERT(pipeline_state_p->type() != ED_pipeline_state_type::NONE);

		NRHI_ENUM_SWITCH(
			pipeline_state_p->type(),
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::GRAPHICS,
				ZG_bind_pipeline_state(command_list_p, { pipeline_state_p });
			)
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::COMPUTE,
				ZC_bind_pipeline_state(command_list_p, { pipeline_state_p });
			)
		);
	}

	void HD_directx12_command_list::ZC_bind_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_compute_pipeline_state_handle pipeline_state_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		dx12_command_list_p->d3d12_command_list_p()->SetPipelineState(
			pipeline_state_p.T_cast<F_directx12_pipeline_state>()->d3d12_pipeline_state_p()
		);
	}
	void HD_directx12_command_list::ZC_bind_root_signature(
		TKPA_valid <nrhi::A_command_list> command_list_p,
		TKPA_valid <nrhi::A_root_signature> root_signature_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetComputeRootSignature(
			root_signature_p.T_cast<F_directx12_root_signature>()->d3d12_root_signature_p()
		);
	}
	void HD_directx12_command_list::ZC_bind_root_descriptor_table(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		F_descriptor_gpu_address gpu_address
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetComputeRootDescriptorTable(
			root_param_index,
			{ gpu_address }
		);
	}
	void HD_directx12_command_list::ZC_bind_root_constants(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		const TG_span<u32>& constant_span,
		u32 offset_in_constants
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetComputeRoot32BitConstants(
			root_param_index,
			constant_span.size(),
			(void*)(constant_span.data()),
			offset_in_constants
		);
	}
	void HD_directx12_command_list::ZC_bind_root_constant(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		u32 root_constant,
		u32 offset_in_constants
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetComputeRoot32BitConstant(
			root_param_index,
			root_constant,
			offset_in_constants
		);
	}
	void HD_directx12_command_list::ZC_bind_root_srv(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		KPA_valid_srv_handle srv_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		return ZC_bind_root_srv_with_resource(
			command_list_p,
			root_param_index,
			NCPP_FOH_VALID(srv_p->desc().resource_p)
		);
	}
	void HD_directx12_command_list::ZC_bind_root_uav(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		KPA_valid_uav_handle uav_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		return ZC_bind_root_uav_with_resource(
			command_list_p,
			root_param_index,
			NCPP_FOH_VALID(uav_p->desc().resource_p)
		);
	}
	void HD_directx12_command_list::ZC_bind_root_cbv_with_resource(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		ZC_bind_root_cbv_with_gpu_virtual_address(
			command_list_p,
			root_param_index,
			HD_directx12_resource::gpu_virtual_address(resource_p)
		);
	}
	void HD_directx12_command_list::ZC_bind_root_srv_with_resource(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		ZC_bind_root_srv_with_gpu_virtual_address(
			command_list_p,
			root_param_index,
			HD_directx12_resource::gpu_virtual_address(resource_p)
		);
	}
	void HD_directx12_command_list::ZC_bind_root_uav_with_resource(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		ZC_bind_root_uav_with_gpu_virtual_address(
			command_list_p,
			root_param_index,
			HD_directx12_resource::gpu_virtual_address(resource_p)
		);
	}
	void HD_directx12_command_list::ZC_bind_root_cbv_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetComputeRootConstantBufferView(
			root_param_index,
			gpu_virtual_address
		);
	}
	void HD_directx12_command_list::ZC_bind_root_srv_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetComputeRootShaderResourceView(
			root_param_index,
			gpu_virtual_address
		);
	}
	void HD_directx12_command_list::ZC_bind_root_uav_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetComputeRootUnorderedAccessView(
			root_param_index,
			gpu_virtual_address
		);
	}

	void HD_directx12_command_list::ZG_bind_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_graphics_pipeline_state_handle pipeline_state_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		const auto& options = HD_directx12_pipeline_state::graphics_options(pipeline_state_p);

		auto d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		d3d12_command_list_p->SetPipelineState(
			pipeline_state_p.T_cast<F_directx12_pipeline_state>()->d3d12_pipeline_state_p()
		);
		d3d12_command_list_p->IASetPrimitiveTopology(
			D3D12_PRIMITIVE_TOPOLOGY(options.primitive_topology)
		);
	}
	void HD_directx12_command_list::ZG_bind_root_signature(
		TKPA_valid <nrhi::A_command_list> command_list_p,
		TKPA_valid <nrhi::A_root_signature> root_signature_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetGraphicsRootSignature(
			root_signature_p.T_cast<F_directx12_root_signature>()->d3d12_root_signature_p()
		);
	}
	void HD_directx12_command_list::ZG_bind_root_descriptor_table(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		F_descriptor_gpu_address gpu_address
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetGraphicsRootDescriptorTable(
			root_param_index,
			{ gpu_address }
		);
	}
	void HD_directx12_command_list::ZG_bind_root_constants(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		const TG_span<u32>& constant_span,
		u32 offset_in_constants
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetGraphicsRoot32BitConstants(
			root_param_index,
			constant_span.size(),
			(void*)(constant_span.data()),
			offset_in_constants
		);
	}
	void HD_directx12_command_list::ZG_bind_root_constant(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		u32 root_constant,
		u32 offset_in_constants
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetGraphicsRoot32BitConstant(
			root_param_index,
			root_constant,
			offset_in_constants
		);
	}
	void HD_directx12_command_list::ZG_bind_root_srv(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		KPA_valid_srv_handle srv_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		return ZG_bind_root_srv_with_resource(
			command_list_p,
			root_param_index,
			NCPP_FOH_VALID(srv_p->desc().resource_p)
		);
	}
	void HD_directx12_command_list::ZG_bind_root_uav(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		KPA_valid_uav_handle uav_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		return ZG_bind_root_uav_with_resource(
			command_list_p,
			root_param_index,
			NCPP_FOH_VALID(uav_p->desc().resource_p)
		);
	}
	void HD_directx12_command_list::ZG_bind_root_cbv_with_resource(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		ZG_bind_root_cbv_with_gpu_virtual_address(
			command_list_p,
			root_param_index,
			HD_directx12_resource::gpu_virtual_address(resource_p)
		);
	}
	void HD_directx12_command_list::ZG_bind_root_srv_with_resource(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		ZG_bind_root_srv_with_gpu_virtual_address(
			command_list_p,
			root_param_index,
			HD_directx12_resource::gpu_virtual_address(resource_p)
		);
	}
	void HD_directx12_command_list::ZG_bind_root_uav_with_resource(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		TKPA_valid<A_resource> resource_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		ZG_bind_root_uav_with_gpu_virtual_address(
			command_list_p,
			root_param_index,
			HD_directx12_resource::gpu_virtual_address(resource_p)
		);
	}
	void HD_directx12_command_list::ZG_bind_root_cbv_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetGraphicsRootConstantBufferView(
			root_param_index,
			gpu_virtual_address
		);
	}
	void HD_directx12_command_list::ZG_bind_root_srv_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetGraphicsRootShaderResourceView(
			root_param_index,
			gpu_virtual_address
		);
	}
	void HD_directx12_command_list::ZG_bind_root_uav_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		u32 root_param_index,
		F_resource_gpu_virtual_address gpu_virtual_address
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetGraphicsRootUnorderedAccessView(
			root_param_index,
			gpu_virtual_address
		);
	}

	void HD_directx12_command_list::ZRS_bind_viewport(
		TKPA_valid<A_command_list> command_list_p,
		const F_viewport& viewport
	)
	{
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& d3d12_command_list_p = command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p();

		D3D12_VIEWPORT d3d12_viewport;
		d3d12_viewport.TopLeftX = viewport.min_x;
		d3d12_viewport.TopLeftY = viewport.min_y;
		d3d12_viewport.Width = viewport.max_x - viewport.min_x;
		d3d12_viewport.Height = viewport.max_y - viewport.min_y;
		d3d12_viewport.MinDepth = viewport.min_z;
		d3d12_viewport.MaxDepth = viewport.max_z;
		d3d12_command_list_p->RSSetViewports(
			1,
			&d3d12_viewport
		);

		D3D12_RECT d3d12_scissor_rect {
			(LONG)(viewport.min_x),
			(LONG)(viewport.min_y),
			(LONG)(viewport.max_x),
			(LONG)(viewport.max_y)
		};
		d3d12_command_list_p->RSSetScissorRects(
			1,
			&d3d12_scissor_rect
		);
	}

	void HD_directx12_command_list::ZOM_bind_frame_buffer(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_frame_buffer> frame_buffer_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";
		NCPP_ASSERT(frame_buffer_p->is_valid_generation()) << "frame buffer's generation is not valid";

		const auto& d3d12_command_list_p = command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p();

		if(frame_buffer_p->management_type() == E_frame_buffer_management_type::MANAGED)
		{
			const auto& frame_buffer_desc = frame_buffer_p->desc();

			const auto& color_attachments = frame_buffer_desc.color_attachments;
			u32 color_attachment_count = (u32)(color_attachments.size());

			F_descriptor_cpu_address d3d12_rtv_addresses[NRHI_MAX_RENDER_TARGET_COUNT_PER_DRAWCALL];
			for(u32 i = 0; i < color_attachment_count; ++i) {

				NCPP_ASSERT(color_attachments[i]->is_valid_generation()) << "color attachment's generation is not valid";

				d3d12_rtv_addresses[i] = color_attachments[i]
					->descriptor_handle().cpu_address;
			}

			F_descriptor_cpu_address d3d12_dsv_address = 0;
			if(frame_buffer_p->is_has_dsv()) {

				const auto& depth_stencil_attachment = frame_buffer_desc.depth_stencil_attachment;

				NCPP_ASSERT(depth_stencil_attachment->is_valid_generation()) << "depth stencil attachment's generation is not valid";

				d3d12_dsv_address = depth_stencil_attachment
					->descriptor_handle().cpu_address;
			}

			d3d12_command_list_p->OMSetRenderTargets(
				color_attachment_count,
				(const D3D12_CPU_DESCRIPTOR_HANDLE*)d3d12_rtv_addresses,
				false,
				(const D3D12_CPU_DESCRIPTOR_HANDLE*)d3d12_dsv_address
			);
		}
		else
		{
			const auto& rtv_addresses = frame_buffer_p->unmanaged_color_attachment_descriptor_cpu_addresses();
			F_descriptor_cpu_address dsv_address = frame_buffer_p->unmanaged_depth_stencil_attachment_descriptor_cpu_address();

			F_descriptor_cpu_address* dsv_address_p = 0;
			if(dsv_address)
			{
				dsv_address_p = &dsv_address;
			}

			d3d12_command_list_p->OMSetRenderTargets(
				rtv_addresses.size(),
				(const D3D12_CPU_DESCRIPTOR_HANDLE*)(rtv_addresses.data()),
				false,
				(const D3D12_CPU_DESCRIPTOR_HANDLE*)dsv_address_p
			);
		}
	}

	void HD_directx12_command_list::ZIA_bind_index_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle index_buffer_p,
		u32 offset
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		NCPP_ASSERT(
			flag_is_has(
				index_buffer_p->desc().flags,
				ED_resource_flag::INDEX_BUFFER
			)
		) << "invalid resource bind flag";

		const auto& index_buffer_desc = index_buffer_p->desc();

		u32 offset_in_bytes = offset * index_buffer_desc.stride;

		D3D12_INDEX_BUFFER_VIEW d3d12_index_buffer_view;
		d3d12_index_buffer_view.BufferLocation = HD_directx12_resource::gpu_virtual_address(
			NCPP_AOH_VALID(index_buffer_p)
		) + offset_in_bytes;
		d3d12_index_buffer_view.SizeInBytes = index_buffer_desc.size - offset_in_bytes;
		d3d12_index_buffer_view.Format = DXGI_FORMAT(index_buffer_desc.format);

		d3d12_command_list_p->IASetIndexBuffer(
			&d3d12_index_buffer_view
		);
	}
	void HD_directx12_command_list::ZIA_bind_input_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& input_buffer_p_span,
		const TG_span<u32>& offset_span,
		u32 base_slot_index
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		u32 buffer_count = input_buffer_p_span.size();

		D3D12_VERTEX_BUFFER_VIEW d3d12_buffer_views[NRHI_MAX_INPUT_BUFFER_COUNT_PER_DRAWCALL];
		for(u32 i = 0; i < buffer_count; ++i)
		{
			const auto& input_buffer_p = input_buffer_p_span[i];
			const auto& desc = input_buffer_p->desc();

			u32 offset_in_bytes = offset_span[i] * desc.stride;

			auto& d3d12_buffer_view = d3d12_buffer_views[i];
			d3d12_buffer_view.BufferLocation = HD_directx12_resource::gpu_virtual_address(
				NCPP_AOH_VALID(input_buffer_p)
			) + offset_in_bytes;
			d3d12_buffer_view.SizeInBytes = desc.size - offset_in_bytes;
			d3d12_buffer_view.StrideInBytes = desc.stride;
		}

		d3d12_command_list_p->IASetVertexBuffers(
			base_slot_index,
			buffer_count,
			d3d12_buffer_views
		);
	}
	void HD_directx12_command_list::ZIA_bind_input_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle input_buffer_p,
		u32 offset,
		u32 slot_index
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		const auto& desc = input_buffer_p->desc();

		u32 offset_in_bytes = offset * desc.stride;

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		D3D12_VERTEX_BUFFER_VIEW d3d12_buffer_view;
		d3d12_buffer_view.BufferLocation = HD_directx12_resource::gpu_virtual_address(
			NCPP_AOH_VALID(input_buffer_p)
		) + offset_in_bytes;
		d3d12_buffer_view.SizeInBytes = desc.size - offset_in_bytes;
		d3d12_buffer_view.StrideInBytes = desc.stride;

		d3d12_command_list_p->IASetVertexBuffers(
			slot_index,
			1,
			&d3d12_buffer_view
		);
	}

	void HD_directx12_command_list::ZIA_bind_index_buffer_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		F_resource_gpu_virtual_address gpu_virtual_address,
		u32 size,
		ED_format format
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		D3D12_INDEX_BUFFER_VIEW d3d12_index_buffer_view;
		d3d12_index_buffer_view.BufferLocation = gpu_virtual_address;
		d3d12_index_buffer_view.SizeInBytes = size;
		d3d12_index_buffer_view.Format = DXGI_FORMAT(format);

		d3d12_command_list_p->IASetIndexBuffer(
			&d3d12_index_buffer_view
		);
	}
	void HD_directx12_command_list::ZIA_bind_input_buffers_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		TG_span<F_resource_gpu_virtual_address> gpu_virtual_addresses,
		TG_span<u32> sizes,
		TG_span<u32> strides,
		u32 base_slot_index
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		u32 buffer_count = gpu_virtual_addresses.size();

		D3D12_VERTEX_BUFFER_VIEW d3d12_buffer_views[NRHI_MAX_INPUT_BUFFER_COUNT_PER_DRAWCALL];
		for(u32 i = 0; i < buffer_count; ++i)
		{
			auto& d3d12_buffer_view = d3d12_buffer_views[i];
			d3d12_buffer_view.BufferLocation = gpu_virtual_addresses[i];
			d3d12_buffer_view.SizeInBytes = sizes[i];
			d3d12_buffer_view.StrideInBytes = strides[i];
		}

		d3d12_command_list_p->IASetVertexBuffers(
			base_slot_index,
			buffer_count,
			d3d12_buffer_views
		);
	}
	void HD_directx12_command_list::ZIA_bind_input_buffer_with_gpu_virtual_address(
		TKPA_valid<A_command_list> command_list_p,
		F_resource_gpu_virtual_address gpu_virtual_address,
		u32 size,
		u32 stride,
		u32 slot_index
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		D3D12_VERTEX_BUFFER_VIEW d3d12_buffer_view;
		d3d12_buffer_view.BufferLocation = gpu_virtual_address;
		d3d12_buffer_view.SizeInBytes = size;
		d3d12_buffer_view.StrideInBytes = stride;

		d3d12_command_list_p->IASetVertexBuffers(
			slot_index,
			1,
			&d3d12_buffer_view
		);
	}

	void HD_directx12_command_list::async_copy_resource(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_resource> dst_resource_p,
		TKPA_valid<A_resource> src_resource_p
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_blit()) << "command list does not support blit";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		NCPP_ASSERT(
			dst_resource_p->desc().size
			== src_resource_p->desc().size
		) << "src resource and dst resource are required to have the same size";

		dx12_command_list_p->d3d12_command_list_p()->CopyResource(
			dst_resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p(),
			src_resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p()
		);
	}
	void HD_directx12_command_list::async_copy_buffer_region(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_resource> dst_resource_p,
		TKPA_valid<A_resource> src_resource_p,
		u64 dst_offset,
		u64 src_offset,
		u64 size
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_blit()) << "command list does not support blit";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		dx12_command_list_p->d3d12_command_list_p()->CopyBufferRegion(
			dst_resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p(),
			dst_offset,
			src_resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p(),
			src_offset,
			size
		);
	}
	void HD_directx12_command_list::async_copy_texture_region(
		TKPA_valid<A_command_list> command_list_p,
		const F_texture_copy_location& dst_location,
		const F_texture_copy_location& src_location,
		PA_vector3_u32 dst_coord,
		PA_vector3_u32 src_coord,
		PA_vector3_u32 volume
	)
    {
    	NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
    	NCPP_ASSERT(command_list_p->supports_blit()) << "command list does not support blit";

    	const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		D3D12_TEXTURE_COPY_LOCATION d3d12_texture_copy_location_dst;
    	d3d12_texture_copy_location_dst.pResource = dst_location.resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
    	d3d12_texture_copy_location_dst.Type = D3D12_TEXTURE_COPY_TYPE(dst_location.type);
    	if(d3d12_texture_copy_location_dst.Type == D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX)
    	{
    		d3d12_texture_copy_location_dst.SubresourceIndex = dst_location.subresource_index;
    	}
    	else
    	{
    		d3d12_texture_copy_location_dst.PlacedFootprint.Offset = dst_location.subresource_footprint.offset;
    		d3d12_texture_copy_location_dst.PlacedFootprint.Footprint.Width = dst_location.subresource_footprint.width;
    		d3d12_texture_copy_location_dst.PlacedFootprint.Footprint.Height = dst_location.subresource_footprint.height;
    		d3d12_texture_copy_location_dst.PlacedFootprint.Footprint.Depth = dst_location.subresource_footprint.depth;
    		d3d12_texture_copy_location_dst.PlacedFootprint.Footprint.Format = DXGI_FORMAT(dst_location.subresource_footprint.format);
    		d3d12_texture_copy_location_dst.PlacedFootprint.Footprint.RowPitch = dst_location.subresource_footprint.first_pitch;
    	}

    	D3D12_TEXTURE_COPY_LOCATION d3d12_texture_copy_location_src;
    	d3d12_texture_copy_location_src.pResource = src_location.resource_p.T_cast<F_directx12_resource>()->d3d12_resource_p();
    	d3d12_texture_copy_location_src.Type = D3D12_TEXTURE_COPY_TYPE(src_location.type);
    	if(d3d12_texture_copy_location_src.Type == D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX)
    	{
    		d3d12_texture_copy_location_src.SubresourceIndex = src_location.subresource_index;
    	}
    	else
    	{
    		d3d12_texture_copy_location_src.PlacedFootprint.Offset = src_location.subresource_footprint.offset;
    		d3d12_texture_copy_location_src.PlacedFootprint.Footprint.Width = src_location.subresource_footprint.width;
    		d3d12_texture_copy_location_src.PlacedFootprint.Footprint.Height = src_location.subresource_footprint.height;
    		d3d12_texture_copy_location_src.PlacedFootprint.Footprint.Depth = src_location.subresource_footprint.depth;
    		d3d12_texture_copy_location_src.PlacedFootprint.Footprint.Format = DXGI_FORMAT(src_location.subresource_footprint.format);
    		d3d12_texture_copy_location_src.PlacedFootprint.Footprint.RowPitch = src_location.subresource_footprint.first_pitch;
    	}

    	D3D12_BOX d3d12_box;
    	d3d12_box.left = src_coord.x;
    	d3d12_box.top = src_coord.y;
    	d3d12_box.front = src_coord.z;

		F_vector3_u32 box_end = src_coord + volume;
    	d3d12_box.right = box_end.x;
    	d3d12_box.bottom = box_end.y;
    	d3d12_box.back = box_end.z;

    	dx12_command_list_p->d3d12_command_list_p()->CopyTextureRegion(
			&d3d12_texture_copy_location_dst,
			dst_coord.x,
			dst_coord.y,
			dst_coord.z,
			&d3d12_texture_copy_location_src,
			&d3d12_box
		);
    }

	void HD_directx12_command_list::async_draw(
		TKPA_valid<A_command_list> command_list_p,
		u32 vertex_count,
		u32 base_vertex_location
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		d3d12_command_list_p->DrawInstanced(
			vertex_count,
			1,
			base_vertex_location,
			0
		);
	}
	void HD_directx12_command_list::async_draw_instanced(
		TKPA_valid<A_command_list> command_list_p,
		u32 vertex_count_per_instance,
		u32 instance_count,
		u32 base_vertex_location,
		u32 base_instance_location
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		d3d12_command_list_p->DrawInstanced(
			vertex_count_per_instance,
			instance_count,
			base_vertex_location,
			base_instance_location
		);
	}
	void HD_directx12_command_list::async_draw_indexed(
		TKPA_valid<A_command_list> command_list_p,
		u32 index_count,
		u32 base_index_location,
		u32 base_vertex_location
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		d3d12_command_list_p->DrawIndexedInstanced(
			index_count,
			1,
			base_index_location,
			base_vertex_location,
			0
		);
	}
	void HD_directx12_command_list::async_draw_indexed_instanced(
		TKPA_valid<A_command_list> command_list_p,
		u32 index_count_per_instance,
		u32 instance_count,
		u32 base_index_location,
		u32 base_vertex_location,
		u32 base_instance_location
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		d3d12_command_list_p->DrawIndexedInstanced(
			index_count_per_instance,
			instance_count,
			base_index_location,
			base_vertex_location,
			base_instance_location
		);
	}

	void HD_directx12_command_list::async_dispatch(
		TKPA_valid<A_command_list> command_list_p,
		PA_vector3_u32 thread_group_count_3d
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";
		NCPP_ASSERT(command_list_p->supports_compute()) << "command list does not support compute";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		d3d12_command_list_p->Dispatch(
			thread_group_count_3d.x,
			thread_group_count_3d.y,
			thread_group_count_3d.z
		);
	}

	void HD_directx12_command_list::async_execute_indirect(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_command_signature> command_signature_p,
		u32 max_command_count,
		KPA_buffer_handle argument_buffer_p,
		u64 argument_buffer_offset_in_bytes,
		KPA_buffer_handle count_buffer_p,
		u64 count_buffer_offset_in_bytes
	) {
		NCPP_ASSERT(command_list_p.T_cast<F_directx12_command_list>()->is_in_record_) << "not in record";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		NCPP_ASSERT(
			flag_is_has(
				argument_buffer_p->desc().flags,
				ED_resource_flag::INDIRECT_ARGUMENT_BUFFER
			)
		) << "invalid argument buffer's flags";

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		d3d12_command_list_p->ExecuteIndirect(
			command_signature_p.T_cast<F_directx12_command_signature>()->d3d12_command_signature_p(),
			max_command_count,
			argument_buffer_p.T_cast<F_directx12_resource>()->d3d12_resource_p(),
			argument_buffer_offset_in_bytes,
			count_buffer_p.T_cast<F_directx12_resource>()->d3d12_resource_p(),
			count_buffer_offset_in_bytes
		);
	}



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	TU<A_command_list> HD_directx12_command_list::create(TKPA_valid<A_device> device_p, const F_command_list_desc& desc) {

    	return H_command_list::ALTERNATIVE::create(
    		device_p,
    		desc
		);
	}

	void HD_directx12_command_list::begin(
		TKPA_valid<A_command_list> command_list_p
	) {
		H_command_list::ALTERNATIVE::begin(
			command_list_p
		);
	}
	void HD_directx12_command_list::end(
		TKPA_valid<A_command_list> command_list_p
	) {
		H_command_list::ALTERNATIVE::end(
			command_list_p
		);
	}

	void HD_directx12_command_list::clear_rtv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_rtv_handle rtv_p,
		PA_vector4_f32 color
	) {
		H_command_list::ALTERNATIVE::clear_rtv(
			command_list_p,
			rtv_p,
			color
		);
	}
	void HD_directx12_command_list::clear_dsv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_dsv_handle dsv_p,
		ED_clear_flag flag,
		f32 depth,
		u8 stencil
	) {
		H_command_list::ALTERNATIVE::clear_dsv(
			command_list_p,
			dsv_p,
			flag,
			depth,
			stencil
		);
	}

	void HD_directx12_command_list::ZVS_bind_constant_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZVS_bind_constant_buffers(
			command_list_p,
			constant_buffer_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZVS_bind_constant_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle constant_buffer_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZVS_bind_constant_buffer(
			command_list_p,
			constant_buffer_p,
			slot_index
		);
	}
	void HD_directx12_command_list::ZVS_bind_srvs(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZVS_bind_srvs(
			command_list_p,
			srv_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZVS_bind_srv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZVS_bind_srv(
			command_list_p,
			srv_p,
			slot_index
		);
	}
	void HD_directx12_command_list::ZVS_bind_sampler_states(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZVS_bind_sampler_states(
			command_list_p,
			sampler_state_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZVS_bind_sampler_state(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_sampler_state> sampler_state_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZVS_bind_sampler_state(
			command_list_p,
			sampler_state_p,
			slot_index
		);
	}

	void HD_directx12_command_list::ZPS_bind_constant_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZPS_bind_constant_buffers(
			command_list_p,
			constant_buffer_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZPS_bind_constant_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle constant_buffer_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZPS_bind_constant_buffer(
			command_list_p,
			constant_buffer_p,
			slot_index
		);
	}
	void HD_directx12_command_list::ZPS_bind_srvs(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZPS_bind_srvs(
			command_list_p,
			srv_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZPS_bind_srv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZPS_bind_srv(
			command_list_p,
			srv_p,
			slot_index
		);
	}
	void HD_directx12_command_list::ZPS_bind_sampler_states(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZPS_bind_sampler_states(
			command_list_p,
			sampler_state_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZPS_bind_sampler_state(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_sampler_state> sampler_state_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZPS_bind_sampler_state(
			command_list_p,
			sampler_state_p,
			slot_index
		);
	}

	void HD_directx12_command_list::ZCS_bind_constant_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZCS_bind_constant_buffers(
			command_list_p,
			constant_buffer_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZCS_bind_constant_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle constant_buffer_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZCS_bind_constant_buffer(
			command_list_p,
			constant_buffer_p,
			slot_index
		);
	}
	void HD_directx12_command_list::ZCS_bind_srvs(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZCS_bind_srvs(
			command_list_p,
			srv_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZCS_bind_srv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZCS_bind_srv(
			command_list_p,
			srv_p,
			slot_index
		);
	}
	void HD_directx12_command_list::ZCS_bind_uavs(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_uav_handle>& uav_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZCS_bind_uavs(
			command_list_p,
			uav_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZCS_bind_uav(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_uav_handle uav_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZCS_bind_uav(
			command_list_p,
			uav_p,
			slot_index
		);
	}
	void HD_directx12_command_list::ZCS_bind_sampler_states(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
		u32 base_slot_index
	) {
		H_command_list::ALTERNATIVE::ZCS_bind_sampler_states(
			command_list_p,
			sampler_state_p_span,
			base_slot_index
		);
	}
	void HD_directx12_command_list::ZCS_bind_sampler_state(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_sampler_state> sampler_state_p,
		u32 slot_index
	) {
		H_command_list::ALTERNATIVE::ZCS_bind_sampler_state(
			command_list_p,
			sampler_state_p,
			slot_index
		);
	}

	void HD_directx12_command_list::draw(
		TKPA_valid<A_command_list> command_list_p,
		u32 vertex_count,
		u32 base_vertex_location
	) {
		H_command_list::ALTERNATIVE::draw(
			command_list_p,
			vertex_count,
			base_vertex_location
		);
	}
	void HD_directx12_command_list::draw_instanced(
		TKPA_valid<A_command_list> command_list_p,
		u32 vertex_count_per_instance,
		u32 instance_count,
		u32 base_vertex_location,
		u32 base_instance_location
	) {
		H_command_list::ALTERNATIVE::draw_instanced(
			command_list_p,
			vertex_count_per_instance,
			instance_count,
			base_vertex_location,
			base_instance_location
		);
	}
	void HD_directx12_command_list::draw_indexed(
		TKPA_valid<A_command_list> command_list_p,
		u32 index_count,
		u32 base_index_location,
		u32 base_vertex_location
	) {
		H_command_list::ALTERNATIVE::draw_indexed(
			command_list_p,
			index_count,
			base_index_location,
			base_vertex_location
		);
	}
	void HD_directx12_command_list::draw_indexed_instanced(
		TKPA_valid<A_command_list> command_list_p,
		u32 index_count_per_instance,
		u32 instance_count,
		u32 base_index_location,
		u32 base_vertex_location,
		u32 base_instance_location
	) {
		H_command_list::ALTERNATIVE::draw_indexed_instanced(
			command_list_p,
			index_count_per_instance,
			instance_count,
			base_index_location,
			base_vertex_location,
			base_instance_location
		);
	}

	void HD_directx12_command_list::dispatch(
		TKPA_valid<A_command_list> command_list_p,
		PA_vector3_u32 thread_group_count_3d
	) {
		H_command_list::ALTERNATIVE::dispatch(
			command_list_p,
			thread_group_count_3d
		);
	}

	void HD_directx12_command_list::draw_instanced_indirect(
		TKPA_valid<A_command_list> command_list_p,
		KPA_buffer_handle buffer_p,
		u32 buffer_offset
	) {
		H_command_list::ALTERNATIVE::draw_instanced_indirect(
			command_list_p,
			buffer_p,
			buffer_offset
		);
	}
	void HD_directx12_command_list::draw_indexed_instanced_indirect(
		TKPA_valid<A_command_list> command_list_p,
		KPA_buffer_handle buffer_p,
		u32 buffer_offset
	) {
		H_command_list::ALTERNATIVE::draw_indexed_instanced_indirect(
			command_list_p,
			buffer_p,
			buffer_offset
		);
	}

	void HD_directx12_command_list::dispatch_indirect(
		TKPA_valid<A_command_list> command_list_p,
		KPA_buffer_handle buffer_p,
		u32 buffer_offset
	) {
		H_command_list::ALTERNATIVE::dispatch_indirect(
			command_list_p,
			buffer_p,
			buffer_offset
		);
	}

	void HD_directx12_command_list::update_resource_data(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_resource> resource_p,
		void* data_p,
		u32 data_size,
		u32 src_data_offset,
		u32 dst_data_offset
	) {
		H_command_list::ALTERNATIVE::update_resource_data(
			command_list_p,
			resource_p,
			data_p,
			data_size,
			src_data_offset,
			dst_data_offset
		);
	}

	void HD_directx12_command_list::generate_mips(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_srv_handle srv_p
	) {
		H_command_list::ALTERNATIVE::generate_mips(
			command_list_p,
			srv_p
		);
	}
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

}
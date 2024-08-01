#include <nrhi/directx12/command_list.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/resource.hpp>
#include <nrhi/directx12/resource_view.hpp>
#include <nrhi/directx12/frame_buffer.hpp>
#include <nrhi/directx12/descriptor_heap.hpp>
#include <nrhi/directx12/root_signature.hpp>
#include <nrhi/directx12/pipeline_state.hpp>
#include <nrhi/directx12/command_signature.hpp>
#include <nrhi/command_allocator.hpp>



namespace nrhi {

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



    TU<A_command_list> HD_directx12_command_list::create_with_command_allocator(TKPA_valid<A_device> device_p, const F_command_list_desc& desc){

        return TU<F_directx12_command_list>()(device_p, desc, F_directx12_command_list_create_with_command_allocator_flag {});
    }

	void HD_directx12_command_list::async_begin(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_command_allocator> command_allocator_p
	) {
		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->Reset(
			command_allocator_p.T_cast<F_directx12_command_allocator>()->d3d12_command_allocator_p(),
			0
		);
	}
	void HD_directx12_command_list::async_end(TKPA_valid<A_command_list> command_list_p) {

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->Close();
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
					ED_resource_barrier_type::UNORDERED_ACCESS,
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

	void HD_directx12_command_list::clear_state(
		TKPA_valid<A_command_list> command_list_p
	) {
		auto dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		dx12_command_list_p->d3d12_command_list_p()->ClearState(0);
	}
	void HD_directx12_command_list::async_clear_rtv(
		TKPA_valid<A_command_list> command_list_p,
		K_valid_rtv_handle rtv_p,
		PA_vector4_f32 color
	) {
		async_clear_rtv_with_descriptor(
			command_list_p,
			rtv_p->descriptor().handle.cpu_address,
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
		async_clear_dsv_with_descriptor(
			command_list_p,
			dsv_p->descriptor().handle.cpu_address,
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
		NCPP_ASSERT(command_list_p->supports_compute() || command_list_p->supports_graphics()) << "command list does not support compute or graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetPipelineState(
			pipeline_state_p.T_cast<F_directx12_pipeline_state>()->d3d12_pipeline_state_p()
		);
	}

	void HD_directx12_command_list::ZC_bind_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_compute_pipeline_state_handle pipeline_state_p
	) {
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
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p()->SetGraphicsRootUnorderedAccessView(
			root_param_index,
			gpu_virtual_address
		);
	}

	void HD_directx12_command_list::ZOM_bind_frame_buffer(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_frame_buffer> frame_buffer_p
	) {
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		NCPP_ASSERT(frame_buffer_p->is_valid_generation()) << "frame buffer's generation is not valid";

		const auto& frame_buffer_desc = frame_buffer_p->desc();

		const auto& color_attachments = frame_buffer_desc.color_attachments;
		u32 color_attachment_count = (u32)(color_attachments.size());

		const auto& d3d12_command_list_p = command_list_p.T_cast<F_directx12_command_list>()->d3d12_command_list_p();

		d3d12_command_list_p->RSSetViewports(
			1,
			&(frame_buffer_p.T_cast<F_directx12_frame_buffer>()->d3d12_viewport())
		);
		d3d12_command_list_p->RSSetScissorRects(
			1,
			&(frame_buffer_p.T_cast<F_directx12_frame_buffer>()->d3d12_scissor_rect())
		);

		F_descriptor_cpu_address d3d12_rtv_addresses[NRHI_MAX_RENDER_TARGET_COUNT_PER_DRAWCALL];
		for(u32 i = 0; i < color_attachment_count; ++i) {

			NCPP_ASSERT(color_attachments[i]->is_valid_generation()) << "color attachment's generation is not valid";

			d3d12_rtv_addresses[i] = color_attachments[i]
				.T_cast<F_directx12_resource_view>()
				->descriptor().handle.cpu_address;
		}

		F_descriptor_cpu_address d3d12_dsv_address = 0;
		if(frame_buffer_p->is_has_dsv()) {

			const auto& depth_stencil_attachment = frame_buffer_desc.depth_stencil_attachment;

			NCPP_ASSERT(depth_stencil_attachment->is_valid_generation()) << "depth stencil attachment's generation is not valid";

			d3d12_dsv_address = depth_stencil_attachment
				.T_cast<F_directx12_resource_view>()
				->descriptor().handle.cpu_address;
		}

		d3d12_command_list_p->OMSetRenderTargets(
			color_attachment_count,
			(const D3D12_CPU_DESCRIPTOR_HANDLE*)d3d12_rtv_addresses,
			false,
			(const D3D12_CPU_DESCRIPTOR_HANDLE*)d3d12_dsv_address
		);
	}

	void HD_directx12_command_list::ZIA_bind_index_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle index_buffer_p,
		u32 offset
	) {
		NCPP_ASSERT(command_list_p->supports_graphics()) << "command list does not support graphics";

		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

		ID3D12GraphicsCommandList* d3d12_command_list_p = dx12_command_list_p->d3d12_command_list_p();

		NCPP_ASSERT(
			u32(
				flag_combine(
					index_buffer_p->desc().flags,
					ED_resource_flag::INDEX_BUFFER
				)
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

	void HD_directx12_command_list::async_draw(
		TKPA_valid<A_command_list> command_list_p,
		u32 vertex_count,
		u32 base_vertex_location
	) {
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
		const auto& dx12_command_list_p = command_list_p.T_cast<F_directx12_command_list>();

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

}
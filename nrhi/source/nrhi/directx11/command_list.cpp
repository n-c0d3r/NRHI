#include <nrhi/directx11/command_list.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/frame_buffer.hpp>
#include <nrhi/directx11/pipeline_state.hpp>
#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/directx11/depth_stencil_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/buffer.hpp>



namespace nrhi {

    F_directx11_command_list::F_directx11_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc) :
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

        if(d3d11_device_context_p_)
            d3d11_device_context_p_->Release();
    }



    TU<A_command_list> HD_directx11_command_list::create(TKPA_valid<A_device> device_p, const F_command_list_desc& desc){

        return TU<F_directx11_command_list>()(device_p, desc);
    }

	void HD_directx11_command_list::clear_state(
		TKPA_valid<A_command_list> command_list_p
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		directx11_command_list_p->temp_state_ = F_directx11_temp_command_list_state {};

		ID3D11DeviceContext* d3d11_device_context_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		d3d11_device_context_p->ClearState();
	}
	void HD_directx11_command_list::set_frame_buffer(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_frame_buffer> frame_buffer_p
	) {

		const auto& frame_buffer_desc = frame_buffer_p->desc();

		const auto& color_attachment_p_vector = frame_buffer_desc.color_attachment_p_vector;
		u32 color_attachment_count = color_attachment_p_vector.size();

		ID3D11DeviceContext* d3d11_device_context_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		d3d11_device_context_p->RSSetViewports(
			1,
			&(frame_buffer_p.T_cast<F_directx11_frame_buffer>()->d3d11_viewport())
		);

		ID3D11RenderTargetView* d3d11_rtv_array[NRHI_MAX_RTV_COUNT_PER_DRAWCALL];
		for(u32 i = 0; i < color_attachment_count; ++i) {

			d3d11_rtv_array[i] = (ID3D11RenderTargetView*)(
				color_attachment_p_vector[i]
				.T_cast<F_directx11_render_target_view>()
				->d3d11_view_p()
			);
		}

		ID3D11DepthStencilView* d3d11_depth_stencil_view_p = 0;
		if(frame_buffer_p->is_has_dsv()) {

			const auto& depth_stencil_attachment_p = frame_buffer_desc.depth_stencil_attachment_p;

			d3d11_depth_stencil_view_p = (ID3D11DepthStencilView*)(
				depth_stencil_attachment_p
				.T_cast<F_directx11_depth_stencil_view>()
				->d3d11_view_p()
			);
		}

		d3d11_device_context_p->OMSetRenderTargets(
			color_attachment_count,
			d3d11_rtv_array,
			d3d11_depth_stencil_view_p
		);
	}
	void HD_directx11_command_list::clear_rtv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_rtv_handle rtv_p,
		PA_vector4 color
	) {

		ID3D11DeviceContext* d3d11_device_context_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		if constexpr (std::is_same_v<F_vector4, F_vector4_f32>)
			d3d11_device_context_p->ClearRenderTargetView(
				(ID3D11RenderTargetView*)(rtv_p.T_cast<F_directx11_render_target_view>()->d3d11_view_p()),
				(const f32*)&color
			);
		else {
			f32 color_f32[4] = {
				color.x,
				color.y,
				color.z,
				color.w
			};
			d3d11_device_context_p->ClearRenderTargetView(
				(ID3D11RenderTargetView*)(rtv_p.T_cast<F_directx11_render_target_view>()->d3d11_view_p()),
				color_f32
			);
		}
	}
	void HD_directx11_command_list::set_graphics_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p
	) {

		const auto desc = graphics_pipeline_state_p->desc();

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		temp_state.vertex_buffer_count = graphics_pipeline_state_p->vertex_buffer_count();
		temp_state.instance_buffer_count = graphics_pipeline_state_p->instance_buffer_count();

		TK_valid<F_directx11_graphics_pipeline_state> directx11_graphics_pipeline_state_p = NCPP_FHANDLE_VALID_AS_OREF(
			graphics_pipeline_state_p
		).T_cast<F_directx11_graphics_pipeline_state>();

		ID3D11DeviceContext* d3d11_device_context_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		// bind d3d11 states
		{
			ID3D11RasterizerState* d3d11_rasterizer_state_p = directx11_graphics_pipeline_state_p->d3d11_rasterizer_state_p();
			if(d3d11_rasterizer_state_p)
				d3d11_device_context_p->RSSetState(d3d11_rasterizer_state_p);

			ID3D11DepthStencilState* d3d11_depth_stencil_state_p = directx11_graphics_pipeline_state_p->d3d11_depth_stencil_state_p();
			if(d3d11_depth_stencil_state_p)
				d3d11_device_context_p->OMSetDepthStencilState(d3d11_depth_stencil_state_p, 0);
		}

		// bind topology
		d3d11_device_context_p->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY(desc.primitive_topology)
		);

		// bind shaders and input layout
		{
			if(auto d3d11_vertex_shader_p = directx11_graphics_pipeline_state_p->d3d11_vertex_shader_p()) {

				d3d11_device_context_p->IASetInputLayout(
					directx11_graphics_pipeline_state_p->d3d11_input_layout_p()
				);
				d3d11_device_context_p->VSSetShader(
					d3d11_vertex_shader_p,
					0,
					0
				);
			}

			if(auto d3d11_pixel_shader_p = directx11_graphics_pipeline_state_p->d3d11_pixel_shader_p()) {

				d3d11_device_context_p->PSSetShader(
					d3d11_pixel_shader_p,
					0,
					0
				);
			}
		}
	}
	void HD_directx11_command_list::set_compute_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p
	) {

	}
	void HD_directx11_command_list::set_vertex_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& vertex_buffer_p_span,
		const TG_span<u32>& offset_span,
		u32 base_slot_index
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 vertex_buffer_count = vertex_buffer_p_span.size();

		for(u32 i = 0; i < vertex_buffer_count; ++i) {

			u32 slot_index = base_slot_index + i;

			const auto& vertex_buffer_p = vertex_buffer_p_span[i];

			NCPP_ASSERT(
				u32(
					flag_combine(
						vertex_buffer_p->desc().bind_flags,
						E_resource_bind_flag::VBV
					)
				)
			) << "invalid resource bind flag";

			temp_state.d3d11_vertex_buffers[slot_index] = (ID3D11Buffer*)(
				vertex_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p()
			);
			temp_state.d3d11_vertex_buffer_offsets[slot_index] = offset_span[i];
			temp_state.d3d11_vertex_buffer_strides[slot_index] = vertex_buffer_p->desc().stride;

			NCPP_ENABLE_IF_DEBUG(
				temp_state.vertex_buffer_orefs[slot_index] = vertex_buffer_p.no_requirements();
			);
		}
	}
	void HD_directx11_command_list::set_instance_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& instance_buffer_p_span,
		const TG_span<u32>& offset_span,
		u32 base_slot_index
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 instance_buffer_count = instance_buffer_p_span.size();

		for(u32 i = 0; i < instance_buffer_count; ++i) {

			u32 slot_index = base_slot_index + i;

			const auto& instance_buffer_p = instance_buffer_p_span[i];

			NCPP_ASSERT(
				u32(
					flag_combine(
						instance_buffer_p->desc().bind_flags,
						E_resource_bind_flag::INSTBV
					)
				)
			) << "invalid resource bind flag";

			temp_state.d3d11_instance_buffers[slot_index] = (ID3D11Buffer*)(
				instance_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p()
			);
			temp_state.d3d11_instance_buffer_offsets[slot_index] = offset_span[i];
			temp_state.d3d11_instance_buffer_strides[slot_index] = instance_buffer_p->desc().stride;

			NCPP_ENABLE_IF_DEBUG(
				temp_state.instance_buffer_orefs[slot_index] = instance_buffer_p.no_requirements();
			);
		}
	}
	void HD_directx11_command_list::set_vertex_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle& vertex_buffer_p,
		u32 offset,
		u32 slot_index
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		NCPP_ASSERT(
			u32(
				flag_combine(
					vertex_buffer_p->desc().bind_flags,
					E_resource_bind_flag::VBV
				)
			)
		) << "invalid resource bind flag";

		temp_state.d3d11_vertex_buffers[slot_index] = (ID3D11Buffer*)(
			vertex_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p()
		);
		temp_state.d3d11_vertex_buffer_offsets[slot_index] = offset;
		temp_state.d3d11_vertex_buffer_strides[slot_index] = vertex_buffer_p->desc().stride;

		NCPP_ENABLE_IF_DEBUG(
			temp_state.vertex_buffer_orefs[slot_index] = vertex_buffer_p.no_requirements();
		);
	}
	void HD_directx11_command_list::set_instance_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle& instance_buffer_p,
		u32 offset,
		u32 slot_index
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		NCPP_ASSERT(
			u32(
				flag_combine(
					instance_buffer_p->desc().bind_flags,
					E_resource_bind_flag::INSTBV
				)
			)
		) << "invalid resource bind flag";

		temp_state.d3d11_instance_buffers[slot_index] = (ID3D11Buffer*)(
			instance_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p()
		);
		temp_state.d3d11_instance_buffer_offsets[slot_index] = offset;
		temp_state.d3d11_instance_buffer_strides[slot_index] = instance_buffer_p->desc().stride;

		NCPP_ENABLE_IF_DEBUG(
			temp_state.instance_buffer_orefs[slot_index] = instance_buffer_p.no_requirements();
		);
	}
	void HD_directx11_command_list::set_index_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle index_buffer_p,
		u32 offset
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		NCPP_ASSERT(
			u32(
				flag_combine(
					index_buffer_p->desc().bind_flags,
					E_resource_bind_flag::IBV
				)
			)
		) << "invalid resource bind flag";

		d3d11_device_context_p->IASetIndexBuffer(
			(ID3D11Buffer*)(index_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p()),
			DXGI_FORMAT(index_buffer_p->desc().format),
			offset
		);
	}
	void HD_directx11_command_list::draw_indexed(
		TKPA_valid<A_command_list> command_list_p,
		u32 index_count,
		u32 base_index_location,
		u32 base_vertex_location
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		apply_temp_state_for_indexed_drawing(
			temp_state,
			d3d11_device_context_p
		);

		d3d11_device_context_p->DrawIndexed(
			index_count,
			base_index_location,
			base_vertex_location
		);

	}
	void HD_directx11_command_list::draw_indexed_instanced(
		TKPA_valid<A_command_list> command_list_p,
		u32 index_count_per_instance,
		u32 instance_count,
		u32 base_index_location,
		u32 base_vertex_location,
		u32 base_instance_location
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		apply_temp_state_for_indexed_instanced_drawing(
			temp_state,
			d3d11_device_context_p
		);

		d3d11_device_context_p->DrawIndexedInstanced(
			index_count_per_instance,
			instance_count,
			base_index_location,
			base_vertex_location,
			base_instance_location
		);

	}



	void HD_directx11_command_list::apply_temp_state_for_indexed_drawing(
		const F_directx11_temp_command_list_state& temp_state,
		ID3D11DeviceContext* d3d11_device_context_p
	) {

#ifdef NCPP_ENABLE_ASSERT
		for(u32 i = 0; i < temp_state.vertex_buffer_count; ++i) {

			const auto& vertex_buffer_p = temp_state.vertex_buffer_orefs[i];

			NCPP_ASSERT(vertex_buffer_p.is_valid()) << "invalid vertex buffer";
		}
#endif
		d3d11_device_context_p->IASetVertexBuffers(
			0,
			temp_state.vertex_buffer_count,
			temp_state.d3d11_vertex_buffers,
			temp_state.d3d11_vertex_buffer_strides,
			temp_state.d3d11_vertex_buffer_offsets
		);

		// indexed drawing does not support instance buffer
//#ifdef NCPP_ENABLE_ASSERT
//		for(u32 i = 0; i < temp_state.instance_buffer_count; ++i) {
//
//			const auto& instance_buffer_p = temp_state.instance_buffer_orefs[i];
//
//			NCPP_ASSERT(instance_buffer_p.is_valid()) << "invalid instance buffer";
//		}
//#endif
//		d3d11_device_context_p->IASetVertexBuffers(
//			temp_state.vertex_buffer_count,
//			temp_state.instance_buffer_count,
//			temp_state.d3d11_instance_buffers,
//			temp_state.d3d11_instance_buffer_strides,
//			temp_state.d3d11_instance_buffer_offsets
//		);

	}
	void HD_directx11_command_list::apply_temp_state_for_indexed_instanced_drawing(
		const F_directx11_temp_command_list_state& temp_state,
		ID3D11DeviceContext* d3d11_device_context_p
	) {

#ifdef NCPP_ENABLE_ASSERT
		for(u32 i = 0; i < temp_state.vertex_buffer_count; ++i) {

			const auto& vertex_buffer_p = temp_state.vertex_buffer_orefs[i];

			NCPP_ASSERT(vertex_buffer_p.is_valid()) << "invalid vertex buffer";
		}
#endif
		d3d11_device_context_p->IASetVertexBuffers(
			0,
			temp_state.vertex_buffer_count,
			temp_state.d3d11_vertex_buffers,
			temp_state.d3d11_vertex_buffer_strides,
			temp_state.d3d11_vertex_buffer_offsets
		);

		// indexed drawing does not support instance buffer
//#ifdef NCPP_ENABLE_ASSERT
//		for(u32 i = 0; i < temp_state.instance_buffer_count; ++i) {
//
//			const auto& instance_buffer_p = temp_state.instance_buffer_orefs[i];
//
//			NCPP_ASSERT(instance_buffer_p.is_valid()) << "invalid instance buffer";
//		}
//#endif
//		d3d11_device_context_p->IASetVertexBuffers(
//			temp_state.vertex_buffer_count,
//			temp_state.instance_buffer_count,
//			temp_state.d3d11_instance_buffers,
//			temp_state.d3d11_instance_buffer_strides,
//			temp_state.d3d11_instance_buffer_offsets
//		);

	}

}
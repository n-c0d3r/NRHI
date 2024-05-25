#include <nrhi/directx11/command_list.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/frame_buffer.hpp>
#include <nrhi/directx11/pipeline_state.hpp>
#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/compute_pipeline_state.hpp>
#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/directx11/depth_stencil_view.hpp>
#include <nrhi/directx11/unordered_access_view.hpp>
#include <nrhi/directx11/shader_resource_view.hpp>
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
	void HD_directx11_command_list::clear_rtv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_rtv_handle rtv_p,
		PA_vector4 color
	) {
		NCPP_ASSERT(rtv_p->is_valid_generation()) << "render target view's generation is not valid";

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
	void HD_directx11_command_list::clear_dsv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_dsv_handle dsv_p,
		E_clear_flag flag,
		f32 depth,
		u8 stencil
	) {
		ID3D11DeviceContext* d3d11_device_context_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		d3d11_device_context_p->ClearDepthStencilView(
			(ID3D11DepthStencilView*)(dsv_p.T_cast<F_directx11_depth_stencil_view>()->d3d11_view_p()),
			D3D11_CLEAR_FLAG(flag),
			depth,
			stencil
		);
	}

	void HD_directx11_command_list::bind_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_pipeline_state> pipeline_state_p
	) {
		switch(pipeline_state_p->desc().type) {

		case E_pipeline_state_type::GRAPHICS:
			bind_graphics_pipeline_state(
				command_list_p,
				{
					pipeline_state_p
				}
			);
			break;
		case E_pipeline_state_type::COMPUTE:
			bind_compute_pipeline_state(
				command_list_p,
				{
					pipeline_state_p
				}
			);
			break;
		}
	}
	void HD_directx11_command_list::bind_graphics_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p
	) {
		const auto desc = graphics_pipeline_state_p->desc();

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		temp_state.vertex_buffer_count = graphics_pipeline_state_p->vertex_buffer_count();
		temp_state.instance_buffer_count = graphics_pipeline_state_p->instance_buffer_count();

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			temp_state.is_pipeline_state_binded = true;
			temp_state.pipeline_state_p = graphics_pipeline_state_p.oref.no_requirements();
		);

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
	void HD_directx11_command_list::bind_compute_pipeline_state(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p
	) {
		const auto desc = compute_pipeline_state_p->desc();

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			temp_state.is_pipeline_state_binded = true;
			temp_state.pipeline_state_p = compute_pipeline_state_p.oref.no_requirements();
		);

		TK_valid<F_directx11_compute_pipeline_state> directx11_compute_pipeline_state_p = NCPP_FHANDLE_VALID_AS_OREF(
			compute_pipeline_state_p
		).T_cast<F_directx11_compute_pipeline_state>();

		ID3D11DeviceContext* d3d11_device_context_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		// bind shader
		d3d11_device_context_p->CSSetShader(
			directx11_compute_pipeline_state_p->d3d11_compute_shader_p(),
			0,
			0
		);
	}

	void HD_directx11_command_list::ZIA_bind_index_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle index_buffer_p,
		u32 offset
	) {
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			auto& temp_state = directx11_command_list_p->temp_state_;
			temp_state.index_buffer_p = index_buffer_p.no_requirements();
		);

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
	void HD_directx11_command_list::ZIA_bind_vertex_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& vertex_buffer_p_span,
		const TG_span<u32>& offset_span,
		u32 base_slot_index
	) {
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 vertex_buffer_count = (u32)(vertex_buffer_p_span.size());

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

			NCPP_ENABLE_IF_ASSERTION_ENABLED(
				temp_state.vertex_buffer_orefs[slot_index] = vertex_buffer_p.no_requirements();
			);
		}
	}
	void HD_directx11_command_list::ZIA_bind_vertex_buffer(
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

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			temp_state.vertex_buffer_orefs[slot_index] = vertex_buffer_p.no_requirements();
		);
	}
	void HD_directx11_command_list::ZIA_bind_instance_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& instance_buffer_p_span,
		const TG_span<u32>& offset_span,
		u32 base_slot_index
	) {
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 instance_buffer_count = (u32)(instance_buffer_p_span.size());

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

			NCPP_ENABLE_IF_ASSERTION_ENABLED(
				temp_state.instance_buffer_orefs[slot_index] = instance_buffer_p.no_requirements();
			);
		}
	}
	void HD_directx11_command_list::ZIA_bind_instance_buffer(
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

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			temp_state.instance_buffer_orefs[slot_index] = instance_buffer_p.no_requirements();
		);
	}

	void HD_directx11_command_list::ZVS_bind_constant_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	)
	{
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 constant_buffer_count = (u32)(constant_buffer_p_span.size());

		ID3D11Buffer* d3d11_constant_buffer_p_array[NRHI_MAX_CONSTANT_BUFFER_COUNT_PER_DRAWCALL];

		for(u32 i = 0; i < constant_buffer_count; ++i) {

			const auto& constant_buffer_p = constant_buffer_p_span[i];

			NCPP_ASSERT(
				u32(
					flag_combine(
						constant_buffer_p->desc().bind_flags,
						E_resource_bind_flag::CBV
					)
				)
			) << "invalid resource bind flag";

			d3d11_constant_buffer_p_array[i] = (ID3D11Buffer*)(constant_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p());
		}

		d3d11_device_context_p->VSSetConstantBuffers(
			base_slot_index,
			constant_buffer_count,
			d3d11_constant_buffer_p_array
		);
	}
	void HD_directx11_command_list::ZVS_bind_constant_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle& constant_buffer_p,
		u32 slot_index
	)
	{
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		NCPP_ASSERT(
			u32(
				flag_combine(
					constant_buffer_p->desc().bind_flags,
					E_resource_bind_flag::CBV
				)
			)
		) << "invalid resource bind flag";

		ID3D11Buffer* d3d11_constant_buffer_p =(ID3D11Buffer*)(constant_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p());

		d3d11_device_context_p->VSSetConstantBuffers(
			slot_index,
			1,
			&d3d11_constant_buffer_p
		);
	}
	void HD_directx11_command_list::ZVS_bind_srvs(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			for(const auto& srv_p : srv_p_span)
				NCPP_ASSERT(srv_p->is_valid_generation()) << "shader resource view's generation is not valid";
		);

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 srv_count = (u32)(srv_p_span.size());

		TG_vector<ID3D11ShaderResourceView*> d3d11_srv_p_vector(srv_count);

		for(u32 i = 0; i < srv_count; ++i) {

			const auto& srv_p = srv_p_span[i];

			d3d11_srv_p_vector[i] = (ID3D11ShaderResourceView*)(srv_p.T_cast<F_directx11_shader_resource_view>()->d3d11_view_p());
		}

		d3d11_device_context_p->VSSetShaderResources(
			base_slot_index,
			srv_count,
			d3d11_srv_p_vector.data()
		);
	}
	void HD_directx11_command_list::ZVS_bind_srv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		NCPP_ASSERT(srv_p->is_valid_generation()) << "shader resource view's generation is not valid";

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		ID3D11ShaderResourceView* d3d11_srv_p =(ID3D11ShaderResourceView*)(srv_p.T_cast<F_directx11_shader_resource_view>()->d3d11_view_p());

		d3d11_device_context_p->VSSetShaderResources(
			slot_index,
			1,
			&d3d11_srv_p
		);
	}

	void HD_directx11_command_list::ZPS_bind_constant_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	)
	{
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 constant_buffer_count = (u32)(constant_buffer_p_span.size());

		ID3D11Buffer* d3d11_constant_buffer_p_array[NRHI_MAX_CONSTANT_BUFFER_COUNT_PER_DRAWCALL];

		for(u32 i = 0; i < constant_buffer_count; ++i) {

			const auto& constant_buffer_p = constant_buffer_p_span[i];

			NCPP_ASSERT(
				u32(
					flag_combine(
						constant_buffer_p->desc().bind_flags,
						E_resource_bind_flag::CBV
					)
				)
			) << "invalid resource bind flag";

			d3d11_constant_buffer_p_array[i] = (ID3D11Buffer*)(constant_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p());
		}

		d3d11_device_context_p->PSSetConstantBuffers(
			base_slot_index,
			constant_buffer_count,
			d3d11_constant_buffer_p_array
		);
	}
	void HD_directx11_command_list::ZPS_bind_constant_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle& constant_buffer_p,
		u32 slot_index
	)
	{
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		NCPP_ASSERT(
			u32(
				flag_combine(
					constant_buffer_p->desc().bind_flags,
					E_resource_bind_flag::CBV
				)
			)
		) << "invalid resource bind flag";

		ID3D11Buffer* d3d11_constant_buffer_p =(ID3D11Buffer*)(constant_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p());

		d3d11_device_context_p->PSSetConstantBuffers(
			slot_index,
			1,
			&d3d11_constant_buffer_p
		);
	}
	void HD_directx11_command_list::ZPS_bind_srvs(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			for(const auto& srv_p : srv_p_span)
				NCPP_ASSERT(srv_p->is_valid_generation()) << "shader resource view's generation is not valid";
		);

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 srv_count = (u32)(srv_p_span.size());

		TG_vector<ID3D11ShaderResourceView*> d3d11_srv_p_vector(srv_count);

		for(u32 i = 0; i < srv_count; ++i) {

			const auto& srv_p = srv_p_span[i];

			d3d11_srv_p_vector[i] = (ID3D11ShaderResourceView*)(srv_p.T_cast<F_directx11_shader_resource_view>()->d3d11_view_p());
		}

		d3d11_device_context_p->PSSetShaderResources(
			base_slot_index,
			srv_count,
			d3d11_srv_p_vector.data()
		);
	}
	void HD_directx11_command_list::ZPS_bind_srv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		NCPP_ASSERT(srv_p->is_valid_generation()) << "shader resource view's generation is not valid";

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		ID3D11ShaderResourceView* d3d11_srv_p =(ID3D11ShaderResourceView*)(srv_p.T_cast<F_directx11_shader_resource_view>()->d3d11_view_p());

		d3d11_device_context_p->PSSetShaderResources(
			slot_index,
			1,
			&d3d11_srv_p
		);
	}

	void HD_directx11_command_list::ZOM_bind_frame_buffer(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_frame_buffer> frame_buffer_p
	) {
		NCPP_ASSERT(frame_buffer_p->is_valid_generation()) << "frame buffer's generation is not valid";

		const auto& frame_buffer_desc = frame_buffer_p->desc();

		const auto& color_attachments = frame_buffer_desc.color_attachments;
		u32 color_attachment_count = (u32)(color_attachments.size());

		ID3D11DeviceContext* d3d11_device_context_p = command_list_p.T_cast<F_directx11_command_list>()->d3d11_device_context_p();

		d3d11_device_context_p->RSSetViewports(
			1,
			&(frame_buffer_p.T_cast<F_directx11_frame_buffer>()->d3d11_viewport())
		);

		ID3D11RenderTargetView* d3d11_rtv_array[NRHI_MAX_RTV_COUNT_PER_DRAWCALL];
		for(u32 i = 0; i < color_attachment_count; ++i) {

			NCPP_ASSERT(color_attachments[i]->is_valid_generation()) << "color attachment's generation is not valid";

			d3d11_rtv_array[i] = (ID3D11RenderTargetView*)(
				color_attachments[i]
					.T_cast<F_directx11_render_target_view>()
					->d3d11_view_p()
			);
		}

		ID3D11DepthStencilView* d3d11_depth_stencil_view_p = 0;
		if(frame_buffer_p->is_has_dsv()) {

			const auto& depth_stencil_attachment = frame_buffer_desc.depth_stencil_attachment;

			NCPP_ASSERT(depth_stencil_attachment->is_valid_generation()) << "depth stencil attachment's generation is not valid";

			d3d11_depth_stencil_view_p = (ID3D11DepthStencilView*)(
				depth_stencil_attachment
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

	void HD_directx11_command_list::ZCS_bind_constant_buffers(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
		u32 base_slot_index
	) {
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 constant_buffer_count = (u32)(constant_buffer_p_span.size());

		ID3D11Buffer* d3d11_constant_buffer_p_array[NRHI_MAX_CONSTANT_BUFFER_COUNT_PER_DRAWCALL];

		for(u32 i = 0; i < constant_buffer_count; ++i) {

			const auto& constant_buffer_p = constant_buffer_p_span[i];

			NCPP_ASSERT(
				u32(
					flag_combine(
						constant_buffer_p->desc().bind_flags,
						E_resource_bind_flag::CBV
					)
				)
			) << "invalid resource bind flag";

			d3d11_constant_buffer_p_array[i] = (ID3D11Buffer*)(constant_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p());
		}

		d3d11_device_context_p->CSSetConstantBuffers(
			base_slot_index,
			constant_buffer_count,
			d3d11_constant_buffer_p_array
		);
	}
	void HD_directx11_command_list::ZCS_bind_constant_buffer(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_buffer_handle constant_buffer_p,
		u32 slot_index
	) {
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		NCPP_ASSERT(
			u32(
				flag_combine(
					constant_buffer_p->desc().bind_flags,
					E_resource_bind_flag::CBV
				)
			)
		) << "invalid resource bind flag";

		ID3D11Buffer* d3d11_constant_buffer_p =(ID3D11Buffer*)(constant_buffer_p.T_cast<F_directx11_buffer>()->d3d11_resource_p());

		d3d11_device_context_p->CSSetConstantBuffers(
			slot_index,
			1,
			&d3d11_constant_buffer_p
		);
	}
	void HD_directx11_command_list::ZCS_bind_srvs(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_srv_handle>& srv_p_span,
		u32 base_slot_index
	) {
		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			for(const auto& srv_p : srv_p_span)
				NCPP_ASSERT(srv_p->is_valid_generation()) << "shader resource view's generation is not valid";
		);

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 srv_count = (u32)(srv_p_span.size());

		TG_vector<ID3D11ShaderResourceView*> d3d11_srv_p_vector(srv_count);

		for(u32 i = 0; i < srv_count; ++i) {

			const auto& srv_p = srv_p_span[i];

			d3d11_srv_p_vector[i] = (ID3D11ShaderResourceView*)(srv_p.T_cast<F_directx11_shader_resource_view>()->d3d11_view_p());
		}

		d3d11_device_context_p->CSSetShaderResources(
			base_slot_index,
			srv_count,
			d3d11_srv_p_vector.data()
		);
	}
	void HD_directx11_command_list::ZCS_bind_srv(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_srv_handle srv_p,
		u32 slot_index
	) {
		NCPP_ASSERT(srv_p->is_valid_generation()) << "shader resource view's generation is not valid";

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		ID3D11ShaderResourceView* d3d11_srv_p =(ID3D11ShaderResourceView*)(srv_p.T_cast<F_directx11_shader_resource_view>()->d3d11_view_p());

		d3d11_device_context_p->CSSetShaderResources(
			slot_index,
			1,
			&d3d11_srv_p
		);
	}
	void HD_directx11_command_list::ZCS_bind_uavs(
		TKPA_valid<A_command_list> command_list_p,
		const TG_span<K_valid_uav_handle>& uav_p_span,
		u32 base_slot_index
	) {
		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			for(const auto& uav_p : uav_p_span)
				NCPP_ASSERT(uav_p->is_valid_generation()) << "unordered access view's generation is not valid";
		);

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		u32 uav_count = (u32)(uav_p_span.size());

		TG_vector<ID3D11UnorderedAccessView*> d3d11_uav_p_vector(uav_count);

		for(u32 i = 0; i < uav_count; ++i) {

			const auto& uav_p = uav_p_span[i];

			d3d11_uav_p_vector[i] = (ID3D11UnorderedAccessView*)(uav_p.T_cast<F_directx11_unordered_access_view>()->d3d11_view_p());
		}

		d3d11_device_context_p->CSSetUnorderedAccessViews(
			base_slot_index,
			uav_count,
			d3d11_uav_p_vector.data(),
			0
		);
	}
	void HD_directx11_command_list::ZCS_bind_uav(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_uav_handle uav_p,
		u32 slot_index
	) {
		NCPP_ASSERT(uav_p->is_valid_generation()) << "unordered access view's generation is not valid";

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		ID3D11UnorderedAccessView* d3d11_uav_p =(ID3D11UnorderedAccessView*)(uav_p.T_cast<F_directx11_unordered_access_view>()->d3d11_view_p());

		d3d11_device_context_p->CSSetUnorderedAccessViews(
			slot_index,
			1,
			&d3d11_uav_p,
			0
		);
	}

	void HD_directx11_command_list::draw(
		TKPA_valid<A_command_list> command_list_p,
		u32 vertex_count,
		u32 base_vertex_location
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			NCPP_ASSERT(temp_state.is_pipeline_state_binded) << "no pipeline state binded";
			NCPP_ASSERT(temp_state.pipeline_state_p->desc().type == E_pipeline_state_type::GRAPHICS) << "invalid pipeline state type";
		);

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		temp_state_apply_vertex_buffers(
			temp_state,
			d3d11_device_context_p
		);

		d3d11_device_context_p->Draw(
			vertex_count,
			base_vertex_location
		);

	}
	void HD_directx11_command_list::draw_instanced(
		TKPA_valid<A_command_list> command_list_p,
		u32 vertex_count_per_instance,
		u32 instance_count,
		u32 base_vertex_location,
		u32 base_instance_location
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			NCPP_ASSERT(temp_state.is_pipeline_state_binded) << "no pipeline state binded";
			NCPP_ASSERT(temp_state.pipeline_state_p->desc().type == E_pipeline_state_type::GRAPHICS) << "invalid pipeline state type";
		);

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		temp_state_apply_vertex_buffers_instance_buffers(
			temp_state,
			d3d11_device_context_p
		);

		d3d11_device_context_p->DrawInstanced(
			vertex_count_per_instance,
			instance_count,
			base_vertex_location,
			base_instance_location
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

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			NCPP_ASSERT(temp_state.is_pipeline_state_binded) << "no pipeline state binded";
			NCPP_ASSERT(temp_state.pipeline_state_p->desc().type == E_pipeline_state_type::GRAPHICS) << "invalid pipeline state type";
			NCPP_ASSERT(temp_state.index_buffer_p.is_valid()) << "index buffer is required";
		);

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		temp_state_apply_vertex_buffers(
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

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			NCPP_ASSERT(temp_state.is_pipeline_state_binded) << "no pipeline state binded";
			NCPP_ASSERT(temp_state.pipeline_state_p->desc().type == E_pipeline_state_type::GRAPHICS) << "invalid pipeline state type";
			NCPP_ASSERT(temp_state.index_buffer_p.is_valid()) << "index buffer is required";
		);

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		temp_state_apply_vertex_buffers_instance_buffers(
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

	void HD_directx11_command_list::dispatch(
		TKPA_valid<A_command_list> command_list_p,
		PA_vector3_u32 thread_group_count_3d
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			NCPP_ASSERT(temp_state.is_pipeline_state_binded) << "no pipeline state binded";
			NCPP_ASSERT(temp_state.pipeline_state_p->desc().type == E_pipeline_state_type::COMPUTE) << "invalid pipeline state type";
		);

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		d3d11_device_context_p->Dispatch(
			thread_group_count_3d.x,
			thread_group_count_3d.y,
			thread_group_count_3d.z
		);
	}

	void HD_directx11_command_list::draw_instanced_indirect(
		TKPA_valid<A_command_list> command_list_p,
		KPA_indirect_buffer_handle indirect_buffer_p,
		u32 indirect_buffer_offset
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			NCPP_ASSERT(temp_state.is_pipeline_state_binded) << "no pipeline state binded";
			NCPP_ASSERT(temp_state.pipeline_state_p->desc().type == E_pipeline_state_type::GRAPHICS) << "invalid pipeline state type";
		);

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		temp_state_apply_vertex_buffers_instance_buffers(
			temp_state,
			d3d11_device_context_p
		);

		d3d11_device_context_p->DrawInstancedIndirect(
			(ID3D11Buffer*)(indirect_buffer_p.T_cast<F_directx11_indirect_buffer>()->d3d11_resource_p()),
			indirect_buffer_offset
		);
	}
	void HD_directx11_command_list::draw_indexed_instanced_indirect(
		TKPA_valid<A_command_list> command_list_p,
		KPA_indirect_buffer_handle indirect_buffer_p,
		u32 indirect_buffer_offset
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			NCPP_ASSERT(temp_state.is_pipeline_state_binded) << "no pipeline state binded";
			NCPP_ASSERT(temp_state.pipeline_state_p->desc().type == E_pipeline_state_type::GRAPHICS) << "invalid pipeline state type";
			NCPP_ASSERT(temp_state.index_buffer_p.is_valid()) << "index buffer is required";
		);

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		temp_state_apply_vertex_buffers_instance_buffers(
			temp_state,
			d3d11_device_context_p
		);

		d3d11_device_context_p->DrawIndexedInstancedIndirect(
			(ID3D11Buffer*)(indirect_buffer_p.T_cast<F_directx11_indirect_buffer>()->d3d11_resource_p()),
			indirect_buffer_offset
		);
	}

	void HD_directx11_command_list::dispatch_indirect(
		TKPA_valid<A_command_list> command_list_p,
		KPA_indirect_buffer_handle indirect_buffer_p,
		u32 indirect_buffer_offset
	) {

		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();

		auto& temp_state = directx11_command_list_p->temp_state_;

		NCPP_ENABLE_IF_ASSERTION_ENABLED(
			NCPP_ASSERT(temp_state.is_pipeline_state_binded) << "no pipeline state binded";
			NCPP_ASSERT(temp_state.pipeline_state_p->desc().type == E_pipeline_state_type::COMPUTE) << "invalid pipeline state type";
		);

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();

		d3d11_device_context_p->DispatchIndirect(
			(ID3D11Buffer*)(indirect_buffer_p.T_cast<F_directx11_indirect_buffer>()->d3d11_resource_p()),
			indirect_buffer_offset
		);
	}

	void HD_directx11_command_list::update_resource_data(
		TKPA_valid<A_command_list> command_list_p,
		TKPA_valid<A_resource> resource_p,
		void* data_p,
		u32 data_size,
		u32 src_data_offset,
		u32 dst_data_offset
	) {
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();
		const auto& directx11_resource_p = resource_p.T_cast<F_directx11_resource>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();
		ID3D11Resource* d3d11_resource_p = directx11_resource_p->d3d11_resource_p();

		NCPP_ASSERT(resource_p->desc().heap_type == E_resource_heap_type::GREAD_CWRITE) << "invalid resource heap type";

		NCPP_ASSERT(data_p) << "invalid data";
		NCPP_ASSERT(data_size) << "invalid data size";
		NCPP_ASSERT((src_data_offset + data_size) <= resource_p->desc().size) << "invalid src data range";
		NCPP_ASSERT((dst_data_offset + data_size) <= resource_p->desc().size) << "invalid dst data range";

		D3D11_MAPPED_SUBRESOURCE d3d11_mapped_resource;
		d3d11_device_context_p->Map(
			d3d11_resource_p,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&d3d11_mapped_resource
		);
		memcpy(((u8*)(d3d11_mapped_resource.pData)) + dst_data_offset, ((u8*)data_p) + src_data_offset, data_size);
		d3d11_device_context_p->Unmap(
			d3d11_resource_p,
			0
		);
	}

	void HD_directx11_command_list::generate_mips(
		TKPA_valid<A_command_list> command_list_p,
		KPA_valid_srv_handle srv_p
	) {
		const auto& directx11_command_list_p = command_list_p.T_cast<F_directx11_command_list>();
		const auto& directx11_srv_p = srv_p.T_cast<F_directx11_shader_resource_view>();

		ID3D11DeviceContext* d3d11_device_context_p = directx11_command_list_p->d3d11_device_context_p();
		ID3D11ShaderResourceView* d3d11_srv_p = (ID3D11ShaderResourceView*)(directx11_srv_p->d3d11_view_p());

		NCPP_ASSERT(directx11_srv_p->desc().resource_p->desc().is_mip_map_generatable) << "this resource is not mip map generatable";

		d3d11_device_context_p->GenerateMips(d3d11_srv_p);
	}



	void HD_directx11_command_list::temp_state_apply_vertex_buffers(
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

	}
	void HD_directx11_command_list::temp_state_apply_vertex_buffers_instance_buffers(
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

#ifdef NCPP_ENABLE_ASSERT
		for(u32 i = 0; i < temp_state.instance_buffer_count; ++i) {

			const auto& instance_buffer_p = temp_state.instance_buffer_orefs[i];

			NCPP_ASSERT(instance_buffer_p.is_valid()) << "invalid instance buffer";
		}
#endif
		d3d11_device_context_p->IASetVertexBuffers(
			temp_state.vertex_buffer_count,
			temp_state.instance_buffer_count,
			temp_state.d3d11_instance_buffers,
			temp_state.d3d11_instance_buffer_strides,
			temp_state.d3d11_instance_buffer_offsets
		);

	}

}
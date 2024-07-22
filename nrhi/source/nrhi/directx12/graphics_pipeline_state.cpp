#include <nrhi/directx12/graphics_pipeline_state.hpp>
#include <nrhi/directx12/shader.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/shader_type.hpp>

namespace nrhi {

	F_rasterizer_desc F_directx12_rasterizer_state_pool::default_input() {

		return {};
	}
	ID3D12RasterizerState* F_directx12_rasterizer_state_pool::create_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc) {

		D3D12_RASTERIZER_DESC d3d12_rs_desc;
		d3d12_rs_desc.FillMode = D3D12_FILL_MODE(desc.fill_mode);
		d3d12_rs_desc.CullMode = D3D12_CULL_MODE(desc.cull_mode);
		d3d12_rs_desc.FrontCounterClockwise = desc.font_counter_clock_wise;
		d3d12_rs_desc.DepthBias = 0;
		d3d12_rs_desc.SlopeScaledDepthBias = 0.0f;
		d3d12_rs_desc.DepthClipEnable = true;
		d3d12_rs_desc.ScissorEnable = false;
		d3d12_rs_desc.MultisampleEnable = false;
		d3d12_rs_desc.AntialiasedLineEnable = false;

		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

		ID3D12RasterizerState* result_p = 0;

		HRESULT hr = d3d12_device_p->CreateRasterizerState(&d3d12_rs_desc, &result_p);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d12 rasterizer state";

		return result_p;
	}
	void F_directx12_rasterizer_state_pool::destroy_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc, ID3D12RasterizerState* object_p) {

		object_p->Release();
	}



	F_depth_stencil_desc F_directx12_depth_stencil_state_pool::default_input() {

		return {};
	}
	ID3D12DepthStencilState* F_directx12_depth_stencil_state_pool::create_object(TKPA_valid<A_device> device_p, const F_depth_stencil_desc& desc) {

		D3D12_DEPTH_STENCIL_DESC d3d12_ds_desc;
		memset(&d3d12_ds_desc, 0, sizeof(d3d12_ds_desc));
		d3d12_ds_desc.DepthEnable = desc.enable_depth_test;
		d3d12_ds_desc.DepthWriteMask = (desc.depth_buffer_write ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO);
		d3d12_ds_desc.DepthFunc = D3D12_COMPARISON_FUNC(desc.depth_comparison_func);

		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

		ID3D12DepthStencilState* result_p = 0;

		HRESULT hr = d3d12_device_p->CreateDepthStencilState(&d3d12_ds_desc, &result_p);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d12 rasterizer state";

		return result_p;
	}
	void F_directx12_depth_stencil_state_pool::destroy_object(TKPA_valid<A_device> device_p, const F_depth_stencil_desc& desc, ID3D12DepthStencilState* object_p) {

		object_p->Release();
	}



	F_directx12_graphics_pipeline_state::F_directx12_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		ED_pipeline_state_type overrided_type
	) :
		F_directx12_pipeline_state(device_p, desc, overrided_type)
	{

		// get d3d12 shaders
		for(auto shader_p : desc.shader_p_vector) {

			const auto& shader_desc = shader_p->desc();

			NRHI_ENUM_SWITCH(
				shader_desc.type,
				NRHI_ENUM_CASE(
					ED_shader_type::VERTEX,
					NCPP_ASSERT(!d3d12_vertex_shader_p_) << "only accept 1 vertex shader";
					d3d12_vertex_shader_p_ = shader_p.T_cast<F_directx12_vertex_shader>()->d3d12_vertex_shader_p();
					d3d12_input_layout_p_ = shader_p.T_cast<F_directx12_vertex_shader>()->d3d12_input_layout_p();
					NRHI_ENUM_BREAK;
            	)
				NRHI_ENUM_CASE(
					ED_shader_type::PIXEL,
					NCPP_ASSERT(!d3d12_pixel_shader_p_) << "only accept 1 pixel shader";
					d3d12_pixel_shader_p_ = shader_p.T_cast<F_directx12_pixel_shader>()->d3d12_pixel_shader_p();
					NRHI_ENUM_BREAK;
            	)
				NRHI_ENUM_DEFAULT(
					NCPP_ASSERT(false) << "invalid shader type";
					NRHI_ENUM_BREAK;
				)
			);
		}
		NCPP_ASSERT(d3d12_vertex_shader_p_) << "vertex shader is required";
		NCPP_ASSERT(d3d12_input_layout_p_) << "d3d12 input layout is required";

		// acquire d3d12 rasterizer state
		d3d12_rasterizer_state_p_ = F_directx12_rasterizer_state_pool::acquire_object(
			device_p,
			desc.rasterizer_desc
		);

		// acquire d3d12 depth stencil state
		d3d12_depth_stencil_state_p_ = F_directx12_depth_stencil_state_pool::acquire_object(
			device_p,
			desc.depth_stencil_desc
		);
	}
	F_directx12_graphics_pipeline_state::~F_directx12_graphics_pipeline_state(){

		// release d3d12 rasterizer state
		F_directx12_rasterizer_state_pool::release_object(
			device_p(),
			desc().rasterizer_desc,
			d3d12_rasterizer_state_p_
		);
		d3d12_rasterizer_state_p_ = 0;

		// release d3d12 depth stencil state
		F_directx12_depth_stencil_state_pool::release_object(
			device_p(),
			desc().depth_stencil_desc,
			d3d12_depth_stencil_state_p_
		);
		d3d12_depth_stencil_state_p_ = 0;
	}

}
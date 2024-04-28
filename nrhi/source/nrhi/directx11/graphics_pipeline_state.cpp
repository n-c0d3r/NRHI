#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/shader.hpp>
#include <nrhi/directx11/shader_blob.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/shader_type.hpp>

namespace nrhi {

	F_rasterizer_desc F_directx11_rasterizer_state_pool::default_input() {

		return {};
	}
	ID3D11RasterizerState* F_directx11_rasterizer_state_pool::create_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc) {

		D3D11_RASTERIZER_DESC d3d11_rs_desc;
		d3d11_rs_desc.FillMode = D3D11_FILL_MODE(desc.fill_mode);
		d3d11_rs_desc.CullMode = D3D11_CULL_MODE(desc.cull_mode);
		d3d11_rs_desc.FrontCounterClockwise = desc.font_counter_clock_wise;
		d3d11_rs_desc.DepthBias = 0;
		d3d11_rs_desc.SlopeScaledDepthBias = 0.0f;
		d3d11_rs_desc.DepthClipEnable = true;
		d3d11_rs_desc.ScissorEnable = false;
		d3d11_rs_desc.MultisampleEnable = false;
		d3d11_rs_desc.AntialiasedLineEnable = false;

		ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		ID3D11RasterizerState* result_p = 0;

		HRESULT hr = d3d11_device_p->CreateRasterizerState(&d3d11_rs_desc, &result_p);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 rasterizer state";

		return result_p;
	}
	void F_directx11_rasterizer_state_pool::destroy_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc, ID3D11RasterizerState* object_p) {

		object_p->Release();
	}



	F_depth_stencil_desc F_directx11_depth_stencil_state_pool::default_input() {

		return {};
	}
	ID3D11DepthStencilState* F_directx11_depth_stencil_state_pool::create_object(TKPA_valid<A_device> device_p, const F_depth_stencil_desc& desc) {

		D3D11_DEPTH_STENCIL_DESC d3d11_ds_desc;
		memset(&d3d11_ds_desc, 0, sizeof(d3d11_ds_desc));
		d3d11_ds_desc.DepthEnable = desc.enable_depth_test;
		d3d11_ds_desc.DepthWriteMask = (desc.depth_buffer_write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO);
		d3d11_ds_desc.DepthFunc = D3D11_COMPARISON_FUNC(desc.depth_comparison_func);

		ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		ID3D11DepthStencilState* result_p = 0;

		HRESULT hr = d3d11_device_p->CreateDepthStencilState(&d3d11_ds_desc, &result_p);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 rasterizer state";

		return result_p;
	}
	void F_directx11_depth_stencil_state_pool::destroy_object(TKPA_valid<A_device> device_p, const F_depth_stencil_desc& desc, ID3D11DepthStencilState* object_p) {

		object_p->Release();
	}



	F_directx11_graphics_pipeline_state::F_directx11_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		E_pipeline_state_type overrided_type
	) :
		F_directx11_pipeline_state(device_p, desc, overrided_type)
	{

		// get d3d11 shaders
		for(auto shader_p : desc.shader_p_vector) {

			const auto& shader_blob_p = shader_p->desc().blob_p;
			const auto& shader_blob_desc = shader_blob_p->desc();

			switch (shader_blob_desc.type) {

			case E_shader_type::VERTEX:
				NCPP_ASSERT(!d3d11_vertex_shader_p_) << "only accept 1 vertex shader";
				d3d11_vertex_shader_p_ = shader_p.T_cast<F_directx11_vertex_shader>()->d3d11_vertex_shader_p();
				d3d11_input_layout_p_ = shader_p.T_cast<F_directx11_vertex_shader>()->d3d11_input_layout_p();

				vertex_shader_blob_desc_ = shader_blob_desc;
				break;

			case E_shader_type::PIXEL:
				NCPP_ASSERT(!d3d11_pixel_shader_p_) << "only accept 1 pixel shader";
				d3d11_pixel_shader_p_ = shader_p.T_cast<F_directx11_pixel_shader>()->d3d11_pixel_shader_p();

				pixel_shader_blob_desc_ = shader_blob_desc;
				break;

			default:
				NCPP_ASSERT(false) << "invalid shader type";
				break;
			}
		}
		NCPP_ASSERT(d3d11_vertex_shader_p_) << "vertex shader is required";
		NCPP_ASSERT(d3d11_input_layout_p_) << "d3d11 input layout is required";

		// acquire d3d11 rasterizer state
		d3d11_rasterizer_state_p_ = F_directx11_rasterizer_state_pool::acquire_object(
			device_p,
			desc.rasterizer_desc
		);

		// acquire d3d11 depth stencil state
		d3d11_depth_stencil_state_p_ = F_directx11_depth_stencil_state_pool::acquire_object(
			device_p,
			desc.depth_stencil_desc
		);
	}
	F_directx11_graphics_pipeline_state::~F_directx11_graphics_pipeline_state(){

		// release d3d11 rasterizer state
		F_directx11_rasterizer_state_pool::release_object(
			device_p(),
			desc().rasterizer_desc,
			d3d11_rasterizer_state_p_
		);
		d3d11_rasterizer_state_p_ = 0;

		// release d3d11 depth stencil state
		F_directx11_depth_stencil_state_pool::release_object(
			device_p(),
			desc().depth_stencil_desc,
			d3d11_depth_stencil_state_p_
		);
		d3d11_depth_stencil_state_p_ = 0;
	}

}
#include <nrhi/directx12/graphics_pipeline_state.hpp>
#include <nrhi/directx12/shader.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/root_signature.hpp>
#include <nrhi/shader_type.hpp>

namespace nrhi {

	F_directx12_graphics_pipeline_state::F_directx12_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		F_directx12_pipeline_state_direct_flag,
		ED_pipeline_state_type overrided_type
	) :
		F_directx12_pipeline_state(
			device_p,
			desc,
			overrided_type,
			create_d3d12_graphics_pipeline_state_direct(
				device_p,
				desc
			)
		)
	{
	}
	F_directx12_graphics_pipeline_state::F_directx12_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		ED_pipeline_state_type overrided_type,
		ID3D12PipelineState* d3d12_graphics_pipeline_state_p
	) :
		F_directx12_pipeline_state(device_p, desc, overrided_type, d3d12_graphics_pipeline_state_p)
	{
	}
	F_directx12_graphics_pipeline_state::~F_directx12_graphics_pipeline_state(){
	}

	ID3D12PipelineState* F_directx12_graphics_pipeline_state::create_d3d12_graphics_pipeline_state_direct(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();
		ID3D12RootSignature* d3d12_root_signature_p = desc.root_signature_p.T_cast<F_directx12_root_signature>()->d3d12_root_signature_p();

		ID3D12PipelineState* d3d12_pipeline_state_p = 0;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC  d3d12_pipeline_state_desc = {};
		d3d12_pipeline_state_desc.pRootSignature = d3d12_root_signature_p;

		d3d12_pipeline_state_desc.RasterizerState.CullMode = D3D12_CULL_MODE(desc.rasterizer_desc.cull_mode);
		d3d12_pipeline_state_desc.RasterizerState.FillMode = D3D12_FILL_MODE(desc.rasterizer_desc.fill_mode);
		d3d12_pipeline_state_desc.RasterizerState.FrontCounterClockwise = desc.rasterizer_desc.front_counter_clock_wise;

		d3d12_pipeline_state_desc.DepthStencilState.DepthEnable = desc.depth_stencil_desc.enable_depth_test;
		d3d12_pipeline_state_desc.DSVFormat = DXGI_FORMAT(desc.depth_stencil_desc.format);
		d3d12_pipeline_state_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC(desc.depth_stencil_desc.depth_comparison_func);
		d3d12_pipeline_state_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK(desc.depth_stencil_desc.depth_buffer_write);

		return 0;
	}

}
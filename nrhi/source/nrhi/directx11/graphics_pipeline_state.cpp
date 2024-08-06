#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>
#include <nrhi/shader_type.hpp>

namespace nrhi {

	F_rasterizer_desc F_directx11_rasterizer_state_pool::default_input() {

		return {};
	}
	ID3D11RasterizerState* F_directx11_rasterizer_state_pool::create_object(TKPA_valid<A_device> device_p, const F_rasterizer_desc& desc) {

		D3D11_RASTERIZER_DESC d3d11_rs_desc;
		d3d11_rs_desc.FillMode = D3D11_FILL_MODE(desc.fill_mode);
		d3d11_rs_desc.CullMode = D3D11_CULL_MODE(desc.cull_mode);
		d3d11_rs_desc.FrontCounterClockwise = desc.front_counter_clock_wise;
		d3d11_rs_desc.DepthBias = 0;
		d3d11_rs_desc.DepthBiasClamp = 0;
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
		const F_graphics_pipeline_state_options& options
	) :
		F_directx11_pipeline_state(device_p, ED_pipeline_state_type::GRAPHICS),
		options_(options)
	{
		auto d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		const auto& shader_binaries = options.shader_binaries;

		// create vertex shader
		{
			const auto& shader_binary = options.shader_binaries.vertex;

			HRESULT hr = d3d11_device_p->CreateVertexShader(
				shader_binary.data(),
				shader_binary.size(),
				0,
				&d3d11_vertex_shader_p_
			);

			NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 vertex shader";
		}

		// create pixel shader
		if(shader_binaries.pixel) {

			const auto& shader_binary = options.shader_binaries.pixel.value();

			HRESULT hr = d3d11_device_p->CreatePixelShader(
				shader_binary.data(),
				shader_binary.size(),
				0,
				&d3d11_pixel_shader_p_
			);

			NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 pixel shader";
		}

		// create input layout
		{
			const auto& shader_binary = shader_binaries.vertex;
			const auto& input_assembler_desc = options.input_assembler_desc;

			u32 input_attribute_group_count = (u32)(input_assembler_desc.attribute_groups.size());

			NCPP_ASSERT(
				input_attribute_group_count
				<= NRHI_MAX_INPUT_BUFFER_COUNT_PER_DRAWCALL
			) << "the number of vertex attribute groups exceeds the limit";

			u32 element_count = 0;
			for(u32 i = 0; i < input_attribute_group_count; ++i) {

				const auto& input_attribute_group = input_assembler_desc.attribute_groups[i];
				u32 attribute_count = (u32)(input_attribute_group.size());

				for(u32 j = 0; j < attribute_count; ++j) {

					element_count += input_attribute_group[j].duplicate_count;
				}
			}

			TG_vector<D3D11_INPUT_ELEMENT_DESC> d3d11_input_element_desc_vector(element_count);

			u32 element_index = 0;
			u32 byte_offset = 0;
			for(u32 i = 0; i < input_attribute_group_count; ++i) {

				const auto& input_attribute_group = input_assembler_desc.attribute_groups[i];
				u32 attribute_count = (u32)(input_attribute_group.size());

				for(u32 j = 0; j < attribute_count; ++j) {

					const auto& input_attribute_desc = input_attribute_group[j];

					for(u32 t = 0; t < input_attribute_desc.duplicate_count; ++t) {

						u32 aligned_attribute_stride = H_format::aligned_stride(input_attribute_desc.format);

						byte_offset = align_address(byte_offset, aligned_attribute_stride);

						d3d11_input_element_desc_vector[element_index] = {
							.SemanticName = input_attribute_desc.name.c_str(),
							.SemanticIndex = t,
							.Format = DXGI_FORMAT(input_attribute_desc.format),
							.InputSlot = i,
							.AlignedByteOffset = ((input_attribute_desc.offset == -1) ? byte_offset : input_attribute_desc.offset),
							.InputSlotClass = D3D11_INPUT_CLASSIFICATION(input_attribute_desc.classification),
							.InstanceDataStepRate = 0
						};

						if(input_attribute_desc.classification == ED_input_classification::PER_INSTANCE_DATA)
							d3d11_input_element_desc_vector[element_index].InstanceDataStepRate = 1;

						++element_index;
						byte_offset += aligned_attribute_stride;
					}
				}

				byte_offset = 0;
			}

			HRESULT hr = d3d11_device_p->CreateInputLayout(
				d3d11_input_element_desc_vector.data(),
				element_count,
				shader_binary.data(),
				shader_binary.size(),
				&d3d11_input_layout_p_
			);

			NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 input layout";
		}

		// acquire d3d11 rasterizer state
		d3d11_rasterizer_state_p_ = F_directx11_rasterizer_state_pool::acquire_object(
			device_p,
			options.rasterizer_desc
		);

		// acquire d3d11 depth stencil state
		d3d11_depth_stencil_state_p_ = F_directx11_depth_stencil_state_pool::acquire_object(
			device_p,
			options.depth_stencil_desc
		);
	}
	F_directx11_graphics_pipeline_state::~F_directx11_graphics_pipeline_state(){

		if(d3d11_input_layout_p_)
			d3d11_input_layout_p_->Release();

		if(d3d11_vertex_shader_p_)
			d3d11_vertex_shader_p_->Release();

		if(d3d11_pixel_shader_p_)
			d3d11_pixel_shader_p_->Release();

		// release d3d11 rasterizer state
		F_directx11_rasterizer_state_pool::release_object(
			device_p(),
			options_.rasterizer_desc,
			d3d11_rasterizer_state_p_
		);
		d3d11_rasterizer_state_p_ = 0;

		// release d3d11 depth stencil state
		F_directx11_depth_stencil_state_pool::release_object(
			device_p(),
			options_.depth_stencil_desc,
			d3d11_depth_stencil_state_p_
		);
		d3d11_depth_stencil_state_p_ = 0;
	}

}
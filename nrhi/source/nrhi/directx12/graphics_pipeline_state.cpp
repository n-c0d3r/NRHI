#include <nrhi/directx12/graphics_pipeline_state.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/root_signature.hpp>
#include <nrhi/format_helper.hpp>
#include <nrhi/shader_type.hpp>
#include <nrhi/primitive_topology.hpp>

namespace nrhi {

	static D3D12_PRIMITIVE_TOPOLOGY_TYPE directx12_primitive_topology_type_table[65];

	struct F_directx12_primitive_topology_type_setup
	{
		F_directx12_primitive_topology_type_setup()
		{
			memset(directx12_primitive_topology_type_table, 0xFF, sizeof(directx12_primitive_topology_type_table));

			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_POINTLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_LINELIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
			directx12_primitive_topology_type_table[
				u32(D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST)
			] = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
		}
	};

	static F_directx12_primitive_topology_type_setup directx12_primitive_topology_type_setup;

	F_directx12_graphics_pipeline_state::F_directx12_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_graphics_pipeline_state_options& options,
		TKPA_valid<A_root_signature> root_signature_p
	) :
		F_directx12_pipeline_state(
			device_p,
			ED_pipeline_state_type::GRAPHICS,
			root_signature_p,
			create_d3d12_graphics_pipeline_state(
				device_p,
				options,
				root_signature_p
			)
		),
		options_(options)
	{
	}
	F_directx12_graphics_pipeline_state::F_directx12_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_graphics_pipeline_state_options& options,
		TKPA_valid<A_root_signature> root_signature_p,
		ID3D12PipelineState* d3d12_graphics_pipeline_state_p
	) :
		F_directx12_pipeline_state(
			device_p,
			ED_pipeline_state_type::GRAPHICS,
			root_signature_p,
			d3d12_graphics_pipeline_state_p
		),
		options_(options)
	{
	}
	F_directx12_graphics_pipeline_state::~F_directx12_graphics_pipeline_state(){

		if(d3d12_pipeline_state_p_)
			d3d12_pipeline_state_p_->Release();
	}

	TG_vector<D3D12_INPUT_ELEMENT_DESC> F_directx12_graphics_pipeline_state::input_assembler_desc_desc_to_d3d12_input_element_descs(
		const F_input_assembler_desc& input_assembler_desc
	) {
		u32 input_attribute_group_count = (u32)(input_assembler_desc.attribute_groups.size());

		NCPP_ASSERT(
			input_attribute_group_count
			<= NRHI_MAX_INPUT_BUFFER_COUNT_PER_DRAWCALL
		) << "the number of input attribute groups exceeds the limit";

		u32 element_count = 0;
		for(u32 i = 0; i < input_attribute_group_count; ++i) {

			const auto& input_attribute_group = input_assembler_desc.attribute_groups[i];
			u32 attribute_count = (u32)(input_attribute_group.size());

			for(u32 j = 0; j < attribute_count; ++j) {

				element_count += input_attribute_group[j].duplicate_count;
			}
		}

		TG_vector<D3D12_INPUT_ELEMENT_DESC> d3d12_input_element_desc_vector(element_count);

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

					d3d12_input_element_desc_vector[element_index] = {
						.SemanticName = input_attribute_desc.name.c_str(),
						.SemanticIndex = t,
						.Format = DXGI_FORMAT(input_attribute_desc.format),
						.InputSlot = i,
						.AlignedByteOffset = ((input_attribute_desc.offset == -1) ? byte_offset : input_attribute_desc.offset),
						.InputSlotClass = D3D12_INPUT_CLASSIFICATION(input_attribute_desc.classification),
						.InstanceDataStepRate = 0
					};

					if(input_attribute_desc.classification == ED_input_classification::PER_INSTANCE_DATA)
						d3d12_input_element_desc_vector[element_index].InstanceDataStepRate = 1;

					++element_index;
					byte_offset += aligned_attribute_stride;
				}
			}

			byte_offset = 0;
		}

		return std::move(d3d12_input_element_desc_vector);
	}
	ID3D12PipelineState* F_directx12_graphics_pipeline_state::create_d3d12_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_graphics_pipeline_state_options& options,
		TKPA_valid<A_root_signature> root_signature_p
	) {
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();
		ID3D12RootSignature* d3d12_root_signature_p = root_signature_p.T_cast<F_directx12_root_signature>()->d3d12_root_signature_p();

		ID3D12PipelineState* d3d12_pipeline_state_p = 0;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC  d3d12_pipeline_state_desc = {};
		d3d12_pipeline_state_desc.pRootSignature = d3d12_root_signature_p;

		d3d12_pipeline_state_desc.RasterizerState.DepthClipEnable = true;
		d3d12_pipeline_state_desc.RasterizerState.CullMode = D3D12_CULL_MODE(options.rasterizer_desc.cull_mode);
		d3d12_pipeline_state_desc.RasterizerState.FillMode = D3D12_FILL_MODE(options.rasterizer_desc.fill_mode);
		d3d12_pipeline_state_desc.RasterizerState.FrontCounterClockwise = options.rasterizer_desc.front_counter_clock_wise;

		d3d12_pipeline_state_desc.DepthStencilState.DepthEnable = options.depth_stencil_desc.enable_depth_test;
		d3d12_pipeline_state_desc.DSVFormat = DXGI_FORMAT(options.depth_stencil_desc.format);
		d3d12_pipeline_state_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC(options.depth_stencil_desc.depth_comparison_func);
		d3d12_pipeline_state_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK(options.depth_stencil_desc.depth_buffer_write);

		d3d12_pipeline_state_desc.BlendState.AlphaToCoverageEnable = options.blend_desc.enable_alpha_to_coverage;
		d3d12_pipeline_state_desc.BlendState.IndependentBlendEnable = DXGI_FORMAT(options.blend_desc.enable_independent_blend);
		for(u32 i = 0; i < 8; ++i) {

			auto& d3d12_blend_rt = d3d12_pipeline_state_desc.BlendState.RenderTarget[i];
			const auto& blend_rt = options.blend_desc.render_targets[i];
			d3d12_blend_rt.BlendEnable = blend_rt.enable_blend;

			d3d12_blend_rt.LogicOpEnable = blend_rt.enable_logic_operation;

			d3d12_blend_rt.SrcBlend = D3D12_BLEND(blend_rt.src_blend_factor);
			d3d12_blend_rt.DestBlend = D3D12_BLEND(blend_rt.dst_blend_factor);
			d3d12_blend_rt.BlendOp = D3D12_BLEND_OP(blend_rt.blend_operation);

			d3d12_blend_rt.SrcBlendAlpha = D3D12_BLEND(blend_rt.src_alpha_blend_factor);
			d3d12_blend_rt.DestBlendAlpha = D3D12_BLEND(blend_rt.dst_alpha_blend_factor);
			d3d12_blend_rt.BlendOpAlpha = D3D12_BLEND_OP(blend_rt.alpha_blend_operation);

			d3d12_blend_rt.LogicOp = D3D12_LOGIC_OP(blend_rt.logic_operation);

			d3d12_blend_rt.RenderTargetWriteMask = u8(blend_rt.write_mode);
		}

		const auto& shader_binaries = options.shader_binaries;

		const auto& vertex_shader_binary = options.shader_binaries.vertex;
		d3d12_pipeline_state_desc.VS = { vertex_shader_binary.data(), vertex_shader_binary.size() };

		TG_vector<D3D12_INPUT_ELEMENT_DESC> d3d12_input_element_descs = input_assembler_desc_desc_to_d3d12_input_element_descs(
			options.input_assembler_desc
		);
		d3d12_pipeline_state_desc.InputLayout.NumElements = d3d12_input_element_descs.size();
		d3d12_pipeline_state_desc.InputLayout.pInputElementDescs = d3d12_input_element_descs.data();

		if(shader_binaries.pixel)
		{
			const auto& pixel_shader_binary = shader_binaries.pixel.value();
			d3d12_pipeline_state_desc.PS = { pixel_shader_binary.data(), pixel_shader_binary.size() };
		}

		d3d12_pipeline_state_desc.NumRenderTargets = options.color_formats.size();
		memcpy(
			d3d12_pipeline_state_desc.RTVFormats,
			options.color_formats.data(),
			options.color_formats.size() * sizeof(DXGI_FORMAT)
		);

		d3d12_pipeline_state_desc.PrimitiveTopologyType = directx12_primitive_topology_type_table[u32(options.primitive_topology)];

		d3d12_pipeline_state_desc.SampleMask = UINT_MAX;
		d3d12_pipeline_state_desc.SampleDesc.Count = 1;

		d3d12_device_p->CreateGraphicsPipelineState(
			&d3d12_pipeline_state_desc,
			IID_PPV_ARGS(&d3d12_pipeline_state_p)
		);
		NCPP_ASSERT(d3d12_pipeline_state_p) << "can't create d3d12 pipeline state";

		return d3d12_pipeline_state_p;
	}

}
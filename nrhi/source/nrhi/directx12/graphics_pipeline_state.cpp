#include <nrhi/directx12/graphics_pipeline_state.hpp>
#include <nrhi/directx12/shader.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/root_signature.hpp>
#include <nrhi/format_helper.hpp>
#include <nrhi/shader_type.hpp>
#include <nrhi/primitive_topology.hpp>

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

		if(d3d12_pipeline_state_p_)
			d3d12_pipeline_state_p_->Release();
	}

	TG_vector<D3D12_INPUT_ELEMENT_DESC> F_directx12_graphics_pipeline_state::vertex_shader_desc_to_d3d12_input_element_descs(
		const F_shader_desc& shader_desc
	) {
		const auto& input_assembler_desc = shader_desc.input_assembler_desc;

		u32 vertex_attribute_group_count = (u32)(input_assembler_desc.vertex_attribute_groups.size());
		u32 instance_attribute_group_count = (u32)(input_assembler_desc.instance_attribute_groups.size());

		NCPP_ASSERT(
			vertex_attribute_group_count
				<= NRHI_MAX_VERTEX_BUFFER_COUNT_PER_DRAWCALL
		) << "the number of vertex attribute groups exceeds the limit";

		NCPP_ASSERT(
			instance_attribute_group_count
				<= NRHI_MAX_INSTANCE_BUFFER_COUNT_PER_DRAWCALL
		) << "the number of instance attribute groups exceeds the limit";

		NCPP_ASSERT(
			(
				vertex_attribute_group_count
					+ instance_attribute_group_count
			)
				<= NRHI_MAX_VERTEX_INSTANCE_BUFFER_COUNT_PER_DRAWCALL
		) << "the number of vetex attribute groups and instance attribute groups exceeds the limit";

		u32 element_count = 0;
		for(u32 i = 0; i < vertex_attribute_group_count; ++i) {

			const auto& vertex_attribute_group = input_assembler_desc.vertex_attribute_groups[i];
			u32 attribute_count = (u32)(vertex_attribute_group.size());

			for(u32 j = 0; j < attribute_count; ++j) {

				element_count += vertex_attribute_group[j].duplicate_count;
			}
		}
		for(u32 i = 0; i < instance_attribute_group_count; ++i) {

			const auto& instance_attribute_group = input_assembler_desc.instance_attribute_groups[i];
			u32 attribute_count = (u32)(instance_attribute_group.size());

			for(u32 j = 0; j < attribute_count; ++j) {

				element_count += instance_attribute_group[j].duplicate_count;
			}
		}

		TG_vector<D3D12_INPUT_ELEMENT_DESC> d3d12_input_element_desc_vector(element_count);

		u32 element_index = 0;
		u32 byte_offset = 0;
		for(u32 i = 0; i < vertex_attribute_group_count; ++i) {

			const auto& vertex_attribute_group = input_assembler_desc.vertex_attribute_groups[i];
			u32 attribute_count = (u32)(vertex_attribute_group.size());

			for(u32 j = 0; j < attribute_count; ++j) {

				const auto& vertex_attribute_desc = vertex_attribute_group[j];

				for(u32 t = 0; t < vertex_attribute_desc.duplicate_count; ++t) {

					u32 aligned_attribute_stride = H_format::aligned_stride(vertex_attribute_desc.format);

					byte_offset = align_address(byte_offset, aligned_attribute_stride);

					d3d12_input_element_desc_vector[element_index] = {
						.SemanticName = vertex_attribute_desc.name.c_str(),
						.SemanticIndex = t,
						.Format = DXGI_FORMAT(vertex_attribute_desc.format),
						.InputSlot = i,
						.AlignedByteOffset = ((vertex_attribute_desc.offset == -1) ? byte_offset : vertex_attribute_desc.offset),
						.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
						.InstanceDataStepRate = 0
					};

					++element_index;
					byte_offset += aligned_attribute_stride;
				}
			}

			byte_offset = 0;
		}

		for(u32 i = 0; i < instance_attribute_group_count; ++i) {

			const auto& instance_attribute_group = input_assembler_desc.instance_attribute_groups[i];
			u32 attribute_count = (u32)(instance_attribute_group.size());

			for(u32 j = 0; j < attribute_count; ++j) {

				const auto& instance_attribute_desc = instance_attribute_group[j];

				for(u32 t = 0; t < instance_attribute_desc.duplicate_count; ++t) {

					u32 aligned_attribute_stride = H_format::aligned_stride(instance_attribute_desc.format);

					byte_offset = align_address(byte_offset, aligned_attribute_stride);

					d3d12_input_element_desc_vector[element_index] = {
						.SemanticName = instance_attribute_desc.name.c_str(),
						.SemanticIndex = t,
						.Format = DXGI_FORMAT(instance_attribute_desc.format),
						.InputSlot = vertex_attribute_group_count + i,
						.AlignedByteOffset = ((instance_attribute_desc.offset == -1) ? byte_offset : instance_attribute_desc.offset),
						.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,
						.InstanceDataStepRate = 1
					};

					++element_index;
					byte_offset += aligned_attribute_stride;
				}
			}

			byte_offset = 0;
		}

		return std::move(d3d12_input_element_desc_vector);
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

		d3d12_pipeline_state_desc.BlendState.AlphaToCoverageEnable = desc.blend_desc.enable_alpha_to_coverage;
		d3d12_pipeline_state_desc.BlendState.IndependentBlendEnable = DXGI_FORMAT(desc.blend_desc.enable_independent_blend);
		for(u32 i = 0; i < 8; ++i) {

			auto& d3d12_blend_rt = d3d12_pipeline_state_desc.BlendState.RenderTarget[i];
			const auto& blend_rt = desc.blend_desc.render_targets[i];
			d3d12_blend_rt.BlendEnable = blend_rt.enable_blend;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
			d3d12_blend_rt.LogicOpEnable = blend_rt.enable_logic_operation;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING

			d3d12_blend_rt.SrcBlend = D3D12_BLEND(blend_rt.src_blend_factor);
			d3d12_blend_rt.DestBlend = D3D12_BLEND(blend_rt.dst_blend_factor);
			d3d12_blend_rt.BlendOp = D3D12_BLEND_OP(blend_rt.blend_operation);

			d3d12_blend_rt.SrcBlendAlpha = D3D12_BLEND(blend_rt.src_alpha_blend_factor);
			d3d12_blend_rt.DestBlendAlpha = D3D12_BLEND(blend_rt.dst_alpha_blend_factor);
			d3d12_blend_rt.BlendOpAlpha = D3D12_BLEND_OP(blend_rt.alpha_blend_operation);

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
			d3d12_blend_rt.LogicOp = D3D12_LOGIC_OP(blend_rt.logic_operation);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_RESOURCE_BINDING
		}

		u32 shader_count = desc.direct_shader_descs.size();
		const auto& direct_shader_descs = desc.direct_shader_descs;
		TG_vector<D3D12_INPUT_ELEMENT_DESC> d3d12_input_element_descs;
		for(u32 i = 0; i < shader_count; ++i) {

			const auto& direct_shader_desc = direct_shader_descs[i];
			const auto& direct_shader_binary = direct_shader_desc.binary;

			NRHI_ENUM_SWITCH(
				direct_shader_desc.type,
				NRHI_ENUM_CASE(
					ED_shader_type::VERTEX,
					d3d12_input_element_descs = vertex_shader_desc_to_d3d12_input_element_descs(
						direct_shader_desc
					);
					d3d12_pipeline_state_desc.VS = { direct_shader_binary.data(), direct_shader_binary.size() };
					d3d12_pipeline_state_desc.InputLayout.NumElements = d3d12_input_element_descs.size();
					d3d12_pipeline_state_desc.InputLayout.pInputElementDescs = d3d12_input_element_descs.data();
				)
				NRHI_ENUM_CASE(
					ED_shader_type::PIXEL,
					d3d12_pipeline_state_desc.PS = { direct_shader_binary.data(), direct_shader_binary.size() };
				)
			);
		}

		d3d12_pipeline_state_desc.NumRenderTargets = desc.color_formats.size();
		memcpy(
			d3d12_pipeline_state_desc.RTVFormats,
			desc.color_formats.data(),
			desc.color_formats.size() * sizeof(DXGI_FORMAT)
		);

		d3d12_pipeline_state_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE(desc.primitive_topology);

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
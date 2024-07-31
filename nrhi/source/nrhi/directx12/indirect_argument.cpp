#include <nrhi/directx12/indirect_argument.hpp>



namespace nrhi {

	static u32 directx12_indirect_argument_stride_table[11];

	struct F_directx12_indirect_argument_stride_setup
	{
		F_directx12_indirect_argument_stride_setup()
		{
			memset(directx12_indirect_argument_stride_table, 0xFF, sizeof(directx12_indirect_argument_stride_table));

			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::DRAW)
			] = sizeof(D3D12_DRAW_ARGUMENTS);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::DRAW_INDEXED)
			] = sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::DISPATCH)
			] = sizeof(D3D12_DISPATCH_ARGUMENTS);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::INPUT_BUFFER)
			] = sizeof(D3D12_VERTEX_BUFFER_VIEW);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::INDEX_BUFFER)
			] = sizeof(D3D12_INDEX_BUFFER_VIEW);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::CONSTANTS)
			] = 4;
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::CONSTANT_BUFFER)
			] = sizeof(D3D12_CONSTANT_BUFFER_VIEW_DESC);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::SHADER_RESOURCE)
			] = sizeof(F_resource_gpu_virtual_address);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::UNORDERED_ACCESS)
			] = sizeof(F_resource_gpu_virtual_address);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::DISPATCH_RAYS)
			] = sizeof(D3D12_DISPATCH_RAYS_DESC);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::DISPATCH_MESH)
			] = sizeof(D3D12_DISPATCH_MESH_ARGUMENTS);
		}
	};

	static F_directx12_indirect_argument_stride_setup directx12_indirect_argument_stride_setup;



	static u32 directx12_indirect_argument_alignment_table[11];

	struct F_directx12_indirect_argument_alignment_setup
	{
		F_directx12_indirect_argument_alignment_setup()
		{
			memset(directx12_indirect_argument_alignment_table, 0xFF, sizeof(directx12_indirect_argument_alignment_table));

			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DRAW)
			] = sizeof(D3D12_DRAW_ARGUMENTS);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DRAW_INDEXED)
			] = sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DISPATCH)
			] = sizeof(D3D12_DISPATCH_ARGUMENTS);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::INPUT_BUFFER)
			] = sizeof(D3D12_VERTEX_BUFFER_VIEW);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::INDEX_BUFFER)
			] = sizeof(D3D12_INDEX_BUFFER_VIEW);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::CONSTANTS)
			] = 4;
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::CONSTANT_BUFFER)
			] = sizeof(D3D12_CONSTANT_BUFFER_VIEW_DESC);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::SHADER_RESOURCE)
			] = sizeof(F_resource_gpu_virtual_address);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::UNORDERED_ACCESS)
			] = sizeof(F_resource_gpu_virtual_address);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DISPATCH_RAYS)
			] = sizeof(D3D12_DISPATCH_RAYS_DESC);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DISPATCH_MESH)
			] = sizeof(D3D12_DISPATCH_MESH_ARGUMENTS);
		}
	};

	static F_directx12_indirect_argument_alignment_setup directx12_indirect_argument_alignment_setup;



	u32 HD_directx12_indirect_argument::size(const F_indirect_argument_desc& desc) {

		u32 stride = directx12_indirect_argument_stride_table[
			u32(desc.type)
		];

		if(desc.type == ED_indirect_argument_type::CONSTANTS)
			return stride + desc.constants.constant_count;

		return stride;
	}
	u32 HD_directx12_indirect_argument::alignment(ED_indirect_argument_type type) {

		return directx12_indirect_argument_stride_table[
			u32(type)
		];
	}

}
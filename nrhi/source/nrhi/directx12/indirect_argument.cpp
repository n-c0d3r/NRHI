#include <nrhi/directx12/indirect_argument.hpp>



namespace nrhi {

	static u32 directx12_indirect_argument_stride_table[11];

	struct F_directx12_indirect_argument_stride_setup
	{
		F_directx12_indirect_argument_stride_setup()
		{
			memset(directx12_indirect_argument_stride_table, 0xFF, sizeof(directx12_indirect_argument_stride_table));

			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::DRAW_INSTANCED)
			] = sizeof(D3D12_DRAW_ARGUMENTS);
			directx12_indirect_argument_stride_table[
				u32(ED_indirect_argument_type::DRAW_INDEXED_INSTANCED)
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
			] = sizeof(u32);
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
				u32(ED_indirect_argument_type::DRAW_INSTANCED)
			] = NCPP_ALIGNOF(D3D12_DRAW_ARGUMENTS);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DRAW_INDEXED_INSTANCED)
			] = NCPP_ALIGNOF(D3D12_DRAW_INDEXED_ARGUMENTS);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DISPATCH)
			] = NCPP_ALIGNOF(D3D12_DISPATCH_ARGUMENTS);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::INPUT_BUFFER)
			] = NCPP_ALIGNOF(D3D12_VERTEX_BUFFER_VIEW);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::INDEX_BUFFER)
			] = NCPP_ALIGNOF(D3D12_INDEX_BUFFER_VIEW);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::CONSTANTS)
			] = NCPP_ALIGNOF(u32);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::CONSTANT_BUFFER)
			] = NCPP_ALIGNOF(D3D12_CONSTANT_BUFFER_VIEW_DESC);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::SHADER_RESOURCE)
			] = NCPP_ALIGNOF(F_resource_gpu_virtual_address);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::UNORDERED_ACCESS)
			] = NCPP_ALIGNOF(F_resource_gpu_virtual_address);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DISPATCH_RAYS)
			] = NCPP_ALIGNOF(D3D12_DISPATCH_RAYS_DESC);
			directx12_indirect_argument_alignment_table[
				u32(ED_indirect_argument_type::DISPATCH_MESH)
			] = NCPP_ALIGNOF(D3D12_DISPATCH_MESH_ARGUMENTS);
		}
	};

	static F_directx12_indirect_argument_alignment_setup directx12_indirect_argument_alignment_setup;



	u32 HD_directx12_indirect_argument::size(const F_indirect_argument_desc& desc) {

		u32 stride = directx12_indirect_argument_stride_table[
			u32(desc.type)
		];

		if(desc.type == ED_indirect_argument_type::CONSTANTS)
			return stride * desc.constants.constant_count;

		return stride;
	}
	u32 HD_directx12_indirect_argument::alignment(ED_indirect_argument_type type) {

		return directx12_indirect_argument_alignment_table[
			u32(type)
		];
	}

	void HD_directx12_indirect_argument::draw(
		void* data_p,
		u32 vertex_count,
		u32 vertex_offset
	)
	{
		*((D3D12_DRAW_ARGUMENTS*)data_p) = {
			.VertexCountPerInstance = vertex_count,
			.InstanceCount = 1,
			.StartVertexLocation = vertex_offset,
			.StartInstanceLocation = 0
		};
	}
	void HD_directx12_indirect_argument::draw_indexed(
		void* data_p,
		u32 index_count,
		u32 index_offset,
		u32 vertex_offset
	)
	{
		*((D3D12_DRAW_INDEXED_ARGUMENTS*)data_p) = {
			.IndexCountPerInstance = index_count,
			.InstanceCount = 1,
			.StartIndexLocation = index_offset,
			.BaseVertexLocation = (INT)vertex_offset,
			.StartInstanceLocation = 0
		};
	}
	void HD_directx12_indirect_argument::draw_instanced(
		void* data_p,
		u32 vertex_count_per_instance,
		u32 instance_count,
		u32 vertex_offset,
		u32 instance_offset
	)
	{
		*((D3D12_DRAW_ARGUMENTS*)data_p) = {
			.VertexCountPerInstance = vertex_count_per_instance,
			.InstanceCount = instance_count,
			.StartVertexLocation = vertex_offset,
			.StartInstanceLocation = instance_offset
		};
	}
	void HD_directx12_indirect_argument::draw_indexed_instanced(
		void* data_p,
		u32 index_count_per_instance,
		u32 instance_count,
		u32 index_offset,
		u32 vertex_offset,
		u32 instance_offset
	)
	{
		*((D3D12_DRAW_INDEXED_ARGUMENTS*)data_p) = {
			.IndexCountPerInstance = index_count_per_instance,
			.InstanceCount = instance_count,
			.StartIndexLocation = index_offset,
			.BaseVertexLocation = (INT)vertex_offset,
			.StartInstanceLocation = instance_offset
		};
	}
	void HD_directx12_indirect_argument::dispatch(
		void* data_p,
		F_vector3_u32 thread_group_counts
	)
	{
		*((D3D12_DISPATCH_ARGUMENTS*)data_p) = {
			.ThreadGroupCountX = thread_group_counts.x,
			.ThreadGroupCountY = thread_group_counts.y,
			.ThreadGroupCountZ = thread_group_counts.z
		};
	}
	void HD_directx12_indirect_argument::input_buffer(
		void* data_p,
		F_resource_gpu_virtual_address gpu_virtual_address,
		u32 size,
		u32 stride
	)
	{
		*((D3D12_VERTEX_BUFFER_VIEW*)data_p) = {
			.BufferLocation = gpu_virtual_address,
			.SizeInBytes = size,
			.StrideInBytes = stride
		};
	}
	void HD_directx12_indirect_argument::index_buffer(
		void* data_p,
		F_resource_gpu_virtual_address gpu_virtual_address,
		u32 size,
		ED_format format
	)
	{
		*((D3D12_INDEX_BUFFER_VIEW*)data_p) = {
			.BufferLocation = gpu_virtual_address,
			.SizeInBytes = size,
			.Format = DXGI_FORMAT(format)
		};
	}
	void HD_directx12_indirect_argument::constants(
		void* data_p,
		const TG_span<u32>& values
	)
	{
		for(u32 i = 0; i < values.size(); ++i)
		{
			((u32*)data_p)[i] = values[i];
		}
	}
	void HD_directx12_indirect_argument::constant_buffer(
		void* data_p,
		F_resource_gpu_virtual_address gpu_virtual_address,
		u32 size
	)
	{
		*((D3D12_CONSTANT_BUFFER_VIEW_DESC*)data_p) = {
			.BufferLocation = gpu_virtual_address,
			.SizeInBytes = size
		};
	}
	void HD_directx12_indirect_argument::srv(
		void* data_p,
		F_descriptor_gpu_address gpu_virtual_address
	)
	{
		*((D3D12_GPU_VIRTUAL_ADDRESS*)data_p) = gpu_virtual_address;
	}
	void HD_directx12_indirect_argument::uav(
		void* data_p,
		F_descriptor_gpu_address gpu_virtual_address
	)
	{
		*((D3D12_GPU_VIRTUAL_ADDRESS*)data_p) = gpu_virtual_address;
	}
	void HD_directx12_indirect_argument::dispatch_mesh(
		void* data_p,
		F_vector3_u32 thread_group_counts
	)
	{
		*((D3D12_DISPATCH_MESH_ARGUMENTS*)data_p) = {
			.ThreadGroupCountX = thread_group_counts.x,
			.ThreadGroupCountY = thread_group_counts.y,
			.ThreadGroupCountZ = thread_group_counts.z
		};
	}
}
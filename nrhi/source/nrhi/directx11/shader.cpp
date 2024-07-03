#include <nrhi/directx11/shader.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/shader_blob.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

	F_directx11_shader::F_directx11_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		A_shader(device_p, desc)
	{
	}
	F_directx11_shader::~F_directx11_shader() {
	}



	F_directx11_vertex_shader::F_directx11_vertex_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		F_directx11_shader(device_p, desc)
	{
		NCPP_ASSERT(desc.blob_p->desc().type == E_shader_type::VERTEX) << "invalid blob's shader type";

		const auto& blob_desc = desc.blob_p->desc();
		const auto& input_assembler_desc = blob_desc.input_assembler_desc;

		const auto& d3d11_shader_blob_p = desc.blob_p.T_cast<F_directx11_shader_blob>();

		auto d3d_blob_p = d3d11_shader_blob_p->d3d_blob_p();
		auto d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();



		// create vertex shader
		HRESULT hr = d3d11_device_p->CreateVertexShader(
			d3d_blob_p->GetBufferPointer(),
			d3d_blob_p->GetBufferSize(),
			0,
			&d3d11_vertex_shader_p_
		);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 vertex shader";



		// create input layout
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

		TG_vector<D3D11_INPUT_ELEMENT_DESC> d3d11_input_element_desc_vector(element_count);

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

					d3d11_input_element_desc_vector[element_index] = {
						.SemanticName = vertex_attribute_desc.name.c_str(),
						.SemanticIndex = t,
						.Format = DXGI_FORMAT(vertex_attribute_desc.format),
						.InputSlot = i,
						.AlignedByteOffset = ((vertex_attribute_desc.offset == -1) ? byte_offset : vertex_attribute_desc.offset),
						.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
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

					d3d11_input_element_desc_vector[element_index] = {
						.SemanticName = instance_attribute_desc.name.c_str(),
						.SemanticIndex = t,
						.Format = DXGI_FORMAT(instance_attribute_desc.format),
						.InputSlot = vertex_attribute_group_count + i,
						.AlignedByteOffset = ((instance_attribute_desc.offset == -1) ? byte_offset : instance_attribute_desc.offset),
						.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA,
						.InstanceDataStepRate = 1
					};

					++element_index;
					byte_offset += aligned_attribute_stride;
				}
			}

			byte_offset = 0;
		}

		hr = d3d11_device_p->CreateInputLayout(
			d3d11_input_element_desc_vector.data(),
			element_count,
			d3d_blob_p->GetBufferPointer(),
			d3d_blob_p->GetBufferSize(),
			&d3d11_input_layout_p_
		);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 input layout";
	}
	F_directx11_vertex_shader::~F_directx11_vertex_shader() {

		if(d3d11_input_layout_p_)
			d3d11_input_layout_p_->Release();
	}



	F_directx11_pixel_shader::F_directx11_pixel_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		F_directx11_shader(device_p, desc)
	{
		NCPP_ASSERT(desc.blob_p->desc().type == E_shader_type::PIXEL) << "invalid blob's shader type";

		const auto& d3d11_shader_blob_p = desc.blob_p.T_cast<F_directx11_shader_blob>();

		auto d3d_blob_p = d3d11_shader_blob_p->d3d_blob_p();
		auto d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		HRESULT hr = d3d11_device_p->CreatePixelShader(
			d3d_blob_p->GetBufferPointer(),
			d3d_blob_p->GetBufferSize(),
			0,
			&d3d11_pixel_shader_p_
		);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 pixel shader";
	}
	F_directx11_pixel_shader::~F_directx11_pixel_shader() {
	}



	F_directx11_compute_shader::F_directx11_compute_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		F_directx11_shader(device_p, desc)
	{
		NCPP_ASSERT(desc.blob_p->desc().type == E_shader_type::COMPUTE) << "invalid blob's shader type";

		const auto& d3d11_shader_blob_p = desc.blob_p.T_cast<F_directx11_shader_blob>();

		auto d3d_blob_p = d3d11_shader_blob_p->d3d_blob_p();
		auto d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		HRESULT hr = d3d11_device_p->CreateComputeShader(
			d3d_blob_p->GetBufferPointer(),
			d3d_blob_p->GetBufferSize(),
			0,
			&d3d11_compute_shader_p_
		);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 compute shader";
	}
	F_directx11_compute_shader::~F_directx11_compute_shader() {
	}



	TU<A_shader> HD_directx11_shader::create(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) {

		NRHI_ENUM_SWITCH(
			desc.blob_p->desc().type,
			NRHI_ENUM_CASE(E_shader_type::VERTEX)
				return TU<F_directx11_vertex_shader>()(device_p, desc);
			NRHI_ENUM_CASE(E_shader_type::PIXEL)
				return TU<F_directx11_pixel_shader>()(device_p, desc);
			NRHI_ENUM_CASE(E_shader_type::COMPUTE)
				return TU<F_directx11_compute_shader>()(device_p, desc);
		);

		return null;
	}

	U_vertex_shader_handle HD_directx11_shader::create_vertex_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) {

		return { TU<F_directx11_vertex_shader>()(device_p, desc) };
	}
	U_pixel_shader_handle HD_directx11_shader::create_pixel_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) {

		return { TU<F_directx11_pixel_shader>()(device_p, desc) };
	}
	U_compute_shader_handle HD_directx11_shader::create_compute_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) {

		return { TU<F_directx11_compute_shader>()(device_p, desc) };
	}

}
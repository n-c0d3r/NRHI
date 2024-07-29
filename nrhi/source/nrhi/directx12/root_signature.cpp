#include <nrhi/directx12/root_signature.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi {

	F_directx12_root_signature::F_directx12_root_signature(
		TKPA_valid<A_device> device_p,
		const F_root_signature_desc& desc
	) :
		A_root_signature(device_p, desc),
		d3d12_root_signature_p_(
			create_d3d12_root_signature(
				device_p,
				desc
			)
		)
	{
	}
	F_directx12_root_signature::F_directx12_root_signature(
		TKPA_valid<A_device> device_p,
		const F_root_signature_desc& desc,
		ID3D12RootSignature* d3d12_root_signature_p
	) :
		A_root_signature(device_p, desc),
		d3d12_root_signature_p_(d3d12_root_signature_p)
	{
	}
	F_directx12_root_signature::~F_directx12_root_signature()
	{
	}

	ID3D12RootSignature* F_directx12_root_signature::create_d3d12_root_signature(
		TKPA_valid<A_device> device_p,
		const F_root_signature_desc& desc
	) {
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

		ID3D12RootSignature* d3d12_root_signature_p = 0;

		D3D12_ROOT_SIGNATURE_DESC d3d12_root_signature_desc;
		d3d12_root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAGS(desc.flags);

		const auto& root_param_descs = desc.param_descs;
		TG_vector<D3D12_ROOT_PARAMETER> d3d12_root_params(root_param_descs.size());
		u32 d3d12_root_param_count = d3d12_root_params.size();
		d3d12_root_signature_desc.NumParameters = d3d12_root_param_count;
		for(u32 i = 0; i < d3d12_root_param_count; ++i) {

			const auto& root_param_desc = root_param_descs[i];
			auto& d3d12_root_param = d3d12_root_params[i];

			d3d12_root_param.ParameterType = D3D12_ROOT_PARAMETER_TYPE(root_param_desc.type);
			d3d12_root_param.ShaderVisibility = D3D12_SHADER_VISIBILITY(root_param_desc.shader_visibility);

			const auto& root_descriptor_table_desc = root_param_desc.descriptor_table_desc;
			const auto& root_descriptor_desc = root_param_desc.descriptor_desc;
			const auto& root_constants_desc = root_param_desc.constants_desc;

			switch (d3d12_root_param.ParameterType)
			{
			case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
				{
					TG_vector<D3D12_DESCRIPTOR_RANGE> d3d12_descriptor_ranges(root_descriptor_table_desc.range_descs.size());
					u32 j_end = d3d12_descriptor_ranges.size();
					for(u32 j = 0; j < j_end; ++j) {

						const auto& range_desc = root_descriptor_table_desc.range_descs[j];
						auto& d3d12_descriptor_range = d3d12_descriptor_ranges[j];

						d3d12_descriptor_range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE(range_desc.type);
						d3d12_descriptor_range.NumDescriptors = range_desc.descriptor_count;
						d3d12_descriptor_range.BaseShaderRegister = range_desc.base_register;
						d3d12_descriptor_range.RegisterSpace = range_desc.register_space;

						// note: for the case of D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND, it's equal to nrhi::offset_in_descriptors_from_table_start_append
						d3d12_descriptor_range.OffsetInDescriptorsFromTableStart = range_desc.offset_in_descriptors_from_table_start;
					}

					d3d12_root_param.DescriptorTable.NumDescriptorRanges = d3d12_descriptor_ranges.size();
					d3d12_root_param.DescriptorTable.pDescriptorRanges = d3d12_descriptor_ranges.data();
				}
				break;
			case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
				d3d12_root_param.Constants.ShaderRegister = root_constants_desc.base_register;
				d3d12_root_param.Constants.RegisterSpace = root_constants_desc.register_space;
				d3d12_root_param.Constants.Num32BitValues = root_constants_desc.constant_count;
				break;
			case D3D12_ROOT_PARAMETER_TYPE_CBV:
				d3d12_root_param.Descriptor.ShaderRegister = root_descriptor_desc.base_register;
				d3d12_root_param.Descriptor.RegisterSpace = root_descriptor_desc.register_space;
				break;
			case D3D12_ROOT_PARAMETER_TYPE_SRV:
				d3d12_root_param.Descriptor.ShaderRegister = root_descriptor_desc.base_register;
				d3d12_root_param.Descriptor.RegisterSpace = root_descriptor_desc.register_space;
				break;
			case D3D12_ROOT_PARAMETER_TYPE_UAV:
				d3d12_root_param.Descriptor.ShaderRegister = root_descriptor_desc.base_register;
				d3d12_root_param.Descriptor.RegisterSpace = root_descriptor_desc.register_space;
				break;
			}
		}
		d3d12_root_signature_desc.pParameters = d3d12_root_params.data();

		const auto& static_sampler_state_descs = desc.static_sampler_state_descs;
		TG_vector<D3D12_STATIC_SAMPLER_DESC> d3d12_static_sampler_state_descs(static_sampler_state_descs.size());
		u32 d3d12_static_sampler_state_count = static_sampler_state_descs.size();
		d3d12_root_signature_desc.NumStaticSamplers = d3d12_static_sampler_state_count;
		for(u32 i = 0; i < d3d12_static_sampler_state_count; ++i) {

			const auto& static_sampler_state_desc = static_sampler_state_descs[i];
			const auto& sampler_state_desc = static_sampler_state_desc.sampler_state_desc;
			auto& d3d12_static_sampler_state_desc = d3d12_static_sampler_state_descs[i];

			d3d12_static_sampler_state_desc.ShaderRegister = static_sampler_state_desc.base_register;
			d3d12_static_sampler_state_desc.RegisterSpace = static_sampler_state_desc.register_space;
			d3d12_static_sampler_state_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY(static_sampler_state_desc.shader_visibility);

			d3d12_static_sampler_state_desc.Filter = D3D12_FILTER(sampler_state_desc.filter);
			d3d12_static_sampler_state_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE(sampler_state_desc.texcoord_address_modes[0]);
			d3d12_static_sampler_state_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE(sampler_state_desc.texcoord_address_modes[1]);
			d3d12_static_sampler_state_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE(sampler_state_desc.texcoord_address_modes[2]);
			d3d12_static_sampler_state_desc.MipLODBias = sampler_state_desc.lod_offset;
			d3d12_static_sampler_state_desc.MinLOD = sampler_state_desc.min_lod;
			d3d12_static_sampler_state_desc.MaxLOD = sampler_state_desc.max_lod;
		}
		d3d12_root_signature_desc.pStaticSamplers = d3d12_static_sampler_state_descs.data();

		ID3DBlob* d3d12_root_signature_blob_p = 0;
		ID3DBlob* d3d12_error_blob_p = 0;

		HRESULT hr = D3D12SerializeRootSignature(
			&d3d12_root_signature_desc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			&d3d12_root_signature_blob_p,
			&d3d12_error_blob_p
		);
		NCPP_ASSERT(!FAILED(hr))
			<< "invalid root signature desc"
			<< std::endl
			<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
			<< (char*)d3d12_error_blob_p->GetBufferPointer();

		d3d12_device_p->CreateRootSignature(
			0,
			d3d12_root_signature_blob_p->GetBufferPointer(),
			d3d12_root_signature_blob_p->GetBufferSize(),
			IID_PPV_ARGS(&d3d12_root_signature_p)
		);
		NCPP_ASSERT(d3d12_root_signature_p) << "can't create d3d12 root signature";

		return d3d12_root_signature_p;
	}



	TU<A_root_signature> HD_directx12_root_signature::create(
		TKPA_valid<A_device> device_p,
		const F_root_signature_desc& desc
	) {
		return TU<F_directx12_root_signature>()(device_p, desc);
	}

}
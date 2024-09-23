#include <nrhi/directx12/command_signature.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/root_signature.hpp>



namespace nrhi {

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
	void F_directx12_command_signature::set_debug_name(const F_debug_name& value)
	{
		A_render_object::set_debug_name(value);

		auto wvalue = G_to_wstring(value.c_str());
		d3d12_command_signature_p_->SetName(wvalue.data());
	}
#endif



	F_directx12_command_signature::F_directx12_command_signature(
		TKPA_valid<A_device> device_p,
		const F_command_signature_desc& desc
	) :
		A_command_signature(device_p, desc),
		d3d12_command_signature_p_(
			create_d3d12_command_signature(
				device_p,
				desc
			)
		)
	{
	}
	F_directx12_command_signature::F_directx12_command_signature(
		TKPA_valid<A_device> device_p,
		const F_command_signature_desc& desc,
		ID3D12CommandSignature* d3d12_command_signature_p
	) :
		A_command_signature(device_p, desc),
		d3d12_command_signature_p_(d3d12_command_signature_p)
	{
	}
	F_directx12_command_signature::~F_directx12_command_signature() {
	}

	ID3D12CommandSignature* F_directx12_command_signature::create_d3d12_command_signature(
		TKPA_valid<A_device> device_p,
		const F_command_signature_desc& desc
	) {
		ID3D12Device* d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();
		ID3D12RootSignature* d3d12_root_signature_p = 0;
		ID3D12CommandSignature* d3d12_command_signature_p = 0;

		if(desc.root_signature_p)
		{
			d3d12_root_signature_p = desc.root_signature_p.T_cast<F_directx12_root_signature>()->d3d12_root_signature_p();
		}

		D3D12_COMMAND_SIGNATURE_DESC d3d12_command_signature_desc = {};
		d3d12_command_signature_desc.ByteStride = desc.stride;

		const auto& indirect_arguments = desc.indirect_argument_descs;
		u32 indirect_argument_count = indirect_arguments.size();
		TG_fixed_vector<D3D12_INDIRECT_ARGUMENT_DESC, 16> d3d12_indirect_arguments(indirect_argument_count);

		for(u32 i = 0; i < indirect_argument_count; ++i) {

			const auto& indirect_argument = indirect_arguments[i];
			auto& d3d12_indirect_argument = d3d12_indirect_arguments[i];

			d3d12_indirect_argument.Type = D3D12_INDIRECT_ARGUMENT_TYPE(indirect_argument.type);

			switch (d3d12_indirect_argument.Type)
			{
			case D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW:
				d3d12_indirect_argument.VertexBuffer.Slot = indirect_argument.input_buffer.slot_index;
				break;
			case D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT:
				d3d12_indirect_argument.Constant.RootParameterIndex = indirect_argument.constants.root_param_index;
				d3d12_indirect_argument.Constant.DestOffsetIn32BitValues = indirect_argument.constants.offset_in_constants;
				d3d12_indirect_argument.Constant.Num32BitValuesToSet = indirect_argument.constants.constant_count;
				break;
			case D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW:
				d3d12_indirect_argument.ConstantBufferView.RootParameterIndex = indirect_argument.constant_buffer.root_param_index;
				break;
			case D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW:
				d3d12_indirect_argument.ShaderResourceView.RootParameterIndex = indirect_argument.shader_resource.root_param_index;
				break;
			case D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW:
				d3d12_indirect_argument.UnorderedAccessView.RootParameterIndex = indirect_argument.unordered_access.root_param_index;
				break;
			}
		}

		d3d12_command_signature_desc.pArgumentDescs = d3d12_indirect_arguments.data();
		d3d12_command_signature_desc.NumArgumentDescs = indirect_argument_count;

		d3d12_device_p->CreateCommandSignature(
			&d3d12_command_signature_desc,
			d3d12_root_signature_p,
			IID_PPV_ARGS(&d3d12_command_signature_p)
		);

		NCPP_ASSERT(d3d12_command_signature_p) << "can't create d3d12 command signature";

		return d3d12_command_signature_p;
	}

	void F_directx12_command_signature::rebuild(
		const F_command_signature_desc& desc
	) {
		finalize_rebuild(desc);
	}



	TU<A_command_signature> HD_directx12_command_signature::create(
		TKPA_valid<A_device> device_p,
		const F_command_signature_desc& desc
	) {
		return TU<F_directx12_command_signature>()(
			device_p,
			desc
		);
	}

}
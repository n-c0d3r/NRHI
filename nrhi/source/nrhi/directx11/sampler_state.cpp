#include <nrhi/directx11/sampler_state.hpp>
#include <nrhi/directx11/device.hpp>



namespace nrhi {

	F_directx11_sampler_state::F_directx11_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc
	) :
		A_sampler_state(device_p, desc),
		d3d11_sampler_state_p_(
			create_d3d11_sampler_state(
				device_p,
				desc
			)
		)
	{
	}
	F_directx11_sampler_state::F_directx11_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc,
		ID3D11SamplerState* d3d11_sampler_state_p
	) :
		A_sampler_state(device_p, desc),
		d3d11_sampler_state_p_(d3d11_sampler_state_p)
	{
	}
	F_directx11_sampler_state::~F_directx11_sampler_state() {

		if(d3d11_sampler_state_p_)
			d3d11_sampler_state_p_->Release();
	}

	ID3D11SamplerState* F_directx11_sampler_state::create_d3d11_sampler_state(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc
	) {
		ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		ID3D11SamplerState* d3d11_sampler_state_p = 0;

		D3D11_SAMPLER_DESC d3d11_sampler_state_desc;
		memset(&d3d11_sampler_state_desc, 0, sizeof(D3D11_SAMPLER_DESC));
		d3d11_sampler_state_desc.Filter = D3D11_FILTER(desc.filter);
		d3d11_sampler_state_desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE(desc.texcoord_address_modes[0]);
		d3d11_sampler_state_desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE(desc.texcoord_address_modes[1]);
		d3d11_sampler_state_desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE(desc.texcoord_address_modes[2]);
		d3d11_sampler_state_desc.MipLODBias = desc.lod_offset;
		d3d11_sampler_state_desc.MinLOD = desc.min_lod;
		d3d11_sampler_state_desc.MaxLOD = desc.max_lod;

		d3d11_device_p->CreateSamplerState(
			&d3d11_sampler_state_desc,
			&d3d11_sampler_state_p
		);

		NCPP_ASSERT(d3d11_sampler_state_p) << "sampler state creation failed";

		return d3d11_sampler_state_p;
	}

	void F_directx11_sampler_state::rebuild(
		const F_sampler_state_desc& desc
	) {
		if(d3d11_sampler_state_p_)
			d3d11_sampler_state_p_->Release();
		d3d11_sampler_state_p_ = create_d3d11_sampler_state(
			device_p(),
			desc
		);
		finalize_rebuild(desc);
	}



	TU<A_sampler_state> HD_directx11_sampler_state::create(
		TKPA_valid<A_device> device_p,
		const F_sampler_state_desc& desc
	) {
		return TU<F_directx11_sampler_state>()(
			device_p,
			desc
		);
	}

}
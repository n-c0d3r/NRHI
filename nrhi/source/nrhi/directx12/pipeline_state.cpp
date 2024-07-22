#include <nrhi/directx12/pipeline_state.hpp>
#include <nrhi/directx12/graphics_pipeline_state.hpp>
#include <nrhi/directx12/compute_pipeline_state.hpp>



namespace nrhi {

	F_directx12_pipeline_state::F_directx12_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		ED_pipeline_state_type overrided_type,
		ID3D12PipelineState* d3d12_pipeline_state_p
	) :
		A_pipeline_state(device_p, desc, overrided_type),
		d3d12_pipeline_state_p_(d3d12_pipeline_state_p)
	{
	}
	F_directx12_pipeline_state::~F_directx12_pipeline_state()
	{
	}



	TU<A_pipeline_state> HD_directx12_pipeline_state::create(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::GRAPHICS,
				return TU<F_directx12_graphics_pipeline_state>()(device_p, desc);
            )
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::COMPUTE,
				return TU<F_directx12_compute_pipeline_state>()(device_p, desc);
            )
		);
	}

	U_graphics_pipeline_state_handle HD_directx12_pipeline_state::create_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {

		return { TU<F_directx12_graphics_pipeline_state>()(device_p, desc) };
	}
	U_compute_pipeline_state_handle HD_directx12_pipeline_state::create_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {

		return { TU<F_directx12_compute_pipeline_state>()(device_p, desc) };
	}



	TU<A_pipeline_state> HD_directx12_pipeline_state::create_direct(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::GRAPHICS,
				return TU<F_directx12_graphics_pipeline_state>()(device_p, desc, F_directx12_pipeline_state_direct_flag{});
			)
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::COMPUTE,
				return TU<F_directx12_compute_pipeline_state>()(device_p, desc, F_directx12_pipeline_state_direct_flag{});
			)
		);
	}

	U_graphics_pipeline_state_handle HD_directx12_pipeline_state::create_graphics_pipeline_state_direct(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {

		return { TU<F_directx12_graphics_pipeline_state>()(device_p, desc, F_directx12_pipeline_state_direct_flag{}) };
	}
	U_compute_pipeline_state_handle HD_directx12_pipeline_state::create_compute_pipeline_state_direct(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {

		return { TU<F_directx12_compute_pipeline_state>()(device_p, desc, F_directx12_pipeline_state_direct_flag{}) };
	}

}
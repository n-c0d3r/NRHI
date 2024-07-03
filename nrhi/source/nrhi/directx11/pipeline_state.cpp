#include <nrhi/directx11/pipeline_state.hpp>
#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/compute_pipeline_state.hpp>



namespace nrhi {

	F_directx11_pipeline_state::F_directx11_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc,
		E_pipeline_state_type overrided_type
	) :
		A_pipeline_state(device_p, desc, overrided_type)
	{
	}
	F_directx11_pipeline_state::~F_directx11_pipeline_state()
	{
	}



	TU<A_pipeline_state> HD_directx11_pipeline_state::create(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
				E_pipeline_state_type::GRAPHICS,
				return TU<F_directx11_graphics_pipeline_state>()(device_p, desc);
            )
			NRHI_ENUM_CASE(
				E_pipeline_state_type::COMPUTE,
				return TU<F_directx11_compute_pipeline_state>()(device_p, desc);
            )
		);
	}

	U_graphics_pipeline_state_handle HD_directx11_pipeline_state::create_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {

		return { TU<F_directx11_graphics_pipeline_state>()(device_p, desc) };
	}
	U_compute_pipeline_state_handle HD_directx11_pipeline_state::create_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_pipeline_state_desc& desc
	) {

		return { TU<F_directx11_compute_pipeline_state>()(device_p, desc) };
	}

}
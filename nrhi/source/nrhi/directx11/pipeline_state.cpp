#include <nrhi/directx11/pipeline_state.hpp>
#include <nrhi/directx11/graphics_pipeline_state.hpp>
#include <nrhi/directx11/compute_pipeline_state.hpp>



namespace nrhi {

	F_directx11_pipeline_state::F_directx11_pipeline_state(
		TKPA_valid<A_device> device_p,
		ED_pipeline_state_type overrided_type
	) :
		A_pipeline_state(device_p, overrided_type)
	{
	}
	F_directx11_pipeline_state::~F_directx11_pipeline_state()
	{
	}



	TU<A_pipeline_state> HD_directx11_pipeline_state::create(
		TKPA_valid<A_device> device_p,
		const A_pipeline_state_desc& desc
	) {
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::GRAPHICS,
				return TU<F_directx11_graphics_pipeline_state>()(device_p, ((const F_graphics_pipeline_state_desc&)desc).options);
            )
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::COMPUTE,
				return TU<F_directx11_compute_pipeline_state>()(device_p, ((const F_compute_pipeline_state_desc&)desc).options);
            )
			NRHI_ENUM_DEFAULT(
				NCPP_ASSERT(false) << "invalid pipeline state type";
			)
		);
	}

	U_graphics_pipeline_state_handle HD_directx11_pipeline_state::create_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_graphics_pipeline_state_options& options
	) {
		return { TU<F_directx11_graphics_pipeline_state>()(device_p, options) };
	}
	U_compute_pipeline_state_handle HD_directx11_pipeline_state::create_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_compute_pipeline_state_options& options
	) {
		return { TU<F_directx11_compute_pipeline_state>()(device_p, options) };
	}

	const F_graphics_pipeline_state_options& HD_directx11_pipeline_state::graphics_options(
		KPA_valid_graphics_pipeline_state_handle pso_p
	) {
		return pso_p.T_cast<F_directx11_graphics_pipeline_state>()->options();
	}
	const F_compute_pipeline_state_options& HD_directx11_pipeline_state::compute_options(
		KPA_valid_compute_pipeline_state_handle pso_p
	) {
		return pso_p.T_cast<F_directx11_compute_pipeline_state>()->options();
	}

}
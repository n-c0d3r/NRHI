#include <nrhi/directx12/pipeline_state.hpp>
#include <nrhi/directx12/graphics_pipeline_state.hpp>
#include <nrhi/directx12/compute_pipeline_state.hpp>
#include <nrhi/pipeline_state.hpp>



namespace nrhi {

	F_directx12_pipeline_state::F_directx12_pipeline_state(
		TKPA_valid<A_device> device_p,
		ED_pipeline_state_type overrided_type,
		TKPA_valid<A_root_signature> root_signature_p,
		ID3D12PipelineState* d3d12_pipeline_state_p
	) :
		A_pipeline_state(device_p, root_signature_p, overrided_type),
		d3d12_pipeline_state_p_(d3d12_pipeline_state_p)
	{
	}
	F_directx12_pipeline_state::~F_directx12_pipeline_state()
	{
	}



	TU<A_pipeline_state> HD_directx12_pipeline_state::create_with_root_signature(
		TKPA_valid<A_device> device_p,
		const A_pipeline_state_desc& desc,
		TKPA_valid<A_root_signature> root_signature_p
	) {
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::GRAPHICS,
				return { TU<F_directx12_graphics_pipeline_state>()(device_p, ((const F_graphics_pipeline_state_desc&)(desc)).options, root_signature_p) };
			)
			NRHI_ENUM_CASE(
				ED_pipeline_state_type::COMPUTE,
				return { TU<F_directx12_compute_pipeline_state>()(device_p, ((const F_compute_pipeline_state_desc&)(desc)).options, root_signature_p) };
			)
		);
	}

	U_graphics_pipeline_state_handle HD_directx12_pipeline_state::create_graphics_pipeline_state_with_root_signature(
		TKPA_valid<A_device> device_p,
		const F_graphics_pipeline_state_options& options,
		TKPA_valid<A_root_signature> root_signature_p
	) {
		return { TU<F_directx12_graphics_pipeline_state>()(device_p, options, root_signature_p) };
	}
	U_compute_pipeline_state_handle HD_directx12_pipeline_state::create_compute_pipeline_state_with_root_signature(
		TKPA_valid<A_device> device_p,
		const F_compute_pipeline_state_options& options,
		TKPA_valid<A_root_signature> root_signature_p
	) {
		return { TU<F_directx12_compute_pipeline_state>()(device_p, options, root_signature_p) };
	}

	const F_graphics_pipeline_state_options& HD_directx12_pipeline_state::graphics_options(
		KPA_valid_graphics_pipeline_state_handle pso_p
	) {
		return pso_p.T_cast<F_directx12_graphics_pipeline_state>()->options();
	}
	const F_compute_pipeline_state_options& HD_directx12_pipeline_state::compute_options(
		KPA_valid_compute_pipeline_state_handle pso_p
	) {
		return pso_p.T_cast<F_directx12_compute_pipeline_state>()->options();
	}



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	TU<A_pipeline_state> HD_directx12_pipeline_state::create(
		TKPA_valid<A_device> device_p,
		const A_pipeline_state_desc& desc
	) {
		return H_pipeline_state::ALTERNATIVE::create(
			device_p,
			desc
		);
	}

	U_graphics_pipeline_state_handle HD_directx12_pipeline_state::create_graphics_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_graphics_pipeline_state_options& options
	) {
		return H_pipeline_state::ALTERNATIVE::create_graphics_pipeline_state(
			device_p,
			options
		);
	}
	U_compute_pipeline_state_handle HD_directx12_pipeline_state::create_compute_pipeline_state(
		TKPA_valid<A_device> device_p,
		const F_compute_pipeline_state_options& options
	) {
		return H_pipeline_state::ALTERNATIVE::create_compute_pipeline_state(
			device_p,
			options
		);
	}
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

}
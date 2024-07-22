#include <nrhi/directx12/shader.hpp>
#include <nrhi/directx12/device.hpp>



namespace nrhi {

	F_directx12_shader::F_directx12_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		A_shader(device_p, desc)
	{
		binary_.resize(desc.binary.size());
		memcpy(
			(void*)(binary_.data()),
			(void*)(desc.binary.data()),
			binary_.size()
		);

		((F_shader_desc&)(this->desc())).binary = binary_;
	}
	F_directx12_shader::~F_directx12_shader() {
	}



	TU<A_shader> HD_directx12_shader::create(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) {
		return TU<F_directx12_shader>()(device_p, desc);
	}

	U_vertex_shader_handle HD_directx12_shader::create_vertex_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) {
		return { TU<F_directx12_shader>()(device_p, desc) };
	}
	U_pixel_shader_handle HD_directx12_shader::create_pixel_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) {
		return { TU<F_directx12_shader>()(device_p, desc) };
	}
	U_compute_shader_handle HD_directx12_shader::create_compute_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) {
		return { TU<F_directx12_shader>()(device_p, desc) };
	}

}
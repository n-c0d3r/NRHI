#include <nrhi/directx11/shader.hpp>
#include <nrhi/directx11/device.hpp>



namespace nrhi {

	F_directx11_shader::F_directx11_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc,
		E_shader_type overrided_type
	) :
		A_shader(device_p, desc, overrided_type)
	{
	}
	F_directx11_shader::~F_directx11_shader() {
	}

}
#include <nrhi/shader_base.hpp>



namespace nrhi {

	A_shader::A_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		A_device_child(device_p),
		desc_(desc)
	{
	}
	A_shader::~A_shader() {

	}

}
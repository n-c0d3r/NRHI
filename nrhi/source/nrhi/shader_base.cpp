#include <nrhi/shader_base.hpp>



namespace nrhi {

	A_shader::A_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		device_p_(device_p),
		desc_(desc)
	{
	}
	A_shader::~A_shader() {

	}

}
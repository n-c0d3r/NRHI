#include <nrhi/shader_library_base.hpp>



namespace nrhi {

	A_shader_library::A_shader_library(
		TK_valid<A_device> device_p,
		const F_shader_library_desc& desc
	) :
		device_p_(device_p),
		desc_(desc)
	{
	}
	A_shader_library::~A_shader_library() {
	}

}
#include <nrhi/directx11/shader_library.hpp>



namespace nrhi {

	F_directx11_shader_library::F_directx11_shader_library(
		TK_valid<A_device> device_p,
		const F_shader_library_desc& desc
	) :
		A_shader_library(device_p, desc)
	{

	}
	F_directx11_shader_library::~F_directx11_shader_library() {

	}

}
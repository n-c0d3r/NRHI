#include <nrhi/directx11/shader_blob.hpp>



namespace nrhi {

	F_directx11_shader_blob::F_directx11_shader_blob(
		const F_shader_blob_desc& desc,
		E_shader_type overrided_type
	) :
		A_shader_blob(desc, overrided_type)
	{
	}
	F_directx11_shader_blob::~F_directx11_shader_blob() {
	}

}
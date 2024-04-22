#include <nrhi/shader_blob_base.hpp>



namespace nrhi {

	A_shader_blob::A_shader_blob(
		const F_shader_blob_desc& desc,
		E_shader_type overrided_type
	) :
		desc_(desc)
	{
		desc_.type = overrided_type;
	}
	A_shader_blob::~A_shader_blob(){

	}

}
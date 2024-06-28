#include <nrhi/shader_base.hpp>
#include <nrhi/shader_blob_base.hpp>



namespace nrhi {

	A_shader::A_shader(
		TKPA_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		device_p_(device_p),
		desc_(desc),
		type_(desc.blob_p->desc().type)
	{
	}
	A_shader::~A_shader() {

	}

}
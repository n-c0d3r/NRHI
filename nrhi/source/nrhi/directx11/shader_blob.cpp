#include <nrhi/directx11/shader_blob.hpp>



namespace nrhi {

	F_directx11_shader_blob::F_directx11_shader_blob(
		const F_shader_blob_desc& desc,
		ED_shader_type overrided_type,
		ID3DBlob* d3d_blob_p
	) :
		A_shader_blob(desc, overrided_type),
		d3d_blob_p_(d3d_blob_p)
	{
	}
	F_directx11_shader_blob::~F_directx11_shader_blob() {

		if(d3d_blob_p_)
			d3d_blob_p_->Release();
	}

}
#include <nrhi/directx11/shader.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/shader_blob.hpp>



namespace nrhi {

	F_directx11_shader::F_directx11_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		A_shader(device_p, desc)
	{
	}
	F_directx11_shader::~F_directx11_shader() {
	}



	F_directx11_vertex_shader::F_directx11_vertex_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		F_directx11_shader(device_p, desc)
	{
		NCPP_ASSERT(desc.blob_p->desc().type == E_shader_type::VERTEX) << "invalid blob's shader type";

		const auto& d3d11_shader_blob_p = desc.blob_p.T_cast<F_directx11_shader_blob>();

		auto d3d_blob_p = d3d11_shader_blob_p->d3d_blob_p();
		auto d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		HRESULT hr = d3d11_device_p->CreateVertexShader(
			d3d_blob_p->GetBufferPointer(),
			d3d_blob_p->GetBufferSize(),
			0,
			&d3d11_vertex_shader_p_
		);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 vertex shader";
	}
	F_directx11_vertex_shader::~F_directx11_vertex_shader() {
	}



	F_directx11_pixel_shader::F_directx11_pixel_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		F_directx11_shader(device_p, desc)
	{
		NCPP_ASSERT(desc.blob_p->desc().type == E_shader_type::PIXEL) << "invalid blob's shader type";

		const auto& d3d11_shader_blob_p = desc.blob_p.T_cast<F_directx11_shader_blob>();

		auto d3d_blob_p = d3d11_shader_blob_p->d3d_blob_p();
		auto d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		HRESULT hr = d3d11_device_p->CreatePixelShader(
			d3d_blob_p->GetBufferPointer(),
			d3d_blob_p->GetBufferSize(),
			0,
			&d3d11_pixel_shader_p_
		);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 pixel shader";
	}
	F_directx11_pixel_shader::~F_directx11_pixel_shader() {
	}



	F_directx11_compute_shader::F_directx11_compute_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) :
		F_directx11_shader(device_p, desc)
	{
		NCPP_ASSERT(desc.blob_p->desc().type == E_shader_type::COMPUTE) << "invalid blob's shader type";

		const auto& d3d11_shader_blob_p = desc.blob_p.T_cast<F_directx11_shader_blob>();

		auto d3d_blob_p = d3d11_shader_blob_p->d3d_blob_p();
		auto d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		HRESULT hr = d3d11_device_p->CreateComputeShader(
			d3d_blob_p->GetBufferPointer(),
			d3d_blob_p->GetBufferSize(),
			0,
			&d3d11_compute_shader_p_
		);

		NCPP_ASSERT(!FAILED(hr)) << "can't create d3d11 compute shader";
	}
	F_directx11_compute_shader::~F_directx11_compute_shader() {
	}



	TU<A_shader> HD_directx11_shader::create(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) {

		switch (desc.blob_p->desc().type)
		{
		case E_shader_type::VERTEX:
			return TU<F_directx11_vertex_shader>()(device_p, desc);
		case E_shader_type::PIXEL:
			return TU<F_directx11_pixel_shader>()(device_p, desc);
		case E_shader_type::COMPUTE:
			return TU<F_directx11_compute_shader>()(device_p, desc);
		}

		return null;
	}

	U_vertex_shader_handle HD_directx11_shader::create_vertex_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) {

		return { TU<F_directx11_vertex_shader>()(device_p, desc) };
	}
	U_pixel_shader_handle HD_directx11_shader::create_pixel_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) {

		return { TU<F_directx11_pixel_shader>()(device_p, desc) };
	}
	U_compute_shader_handle HD_directx11_shader::create_compute_shader(
		TK_valid<A_device> device_p,
		const F_shader_desc& desc
	) {

		return { TU<F_directx11_compute_shader>()(device_p, desc) };
	}

}
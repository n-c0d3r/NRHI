#include <nrhi/directx11/shader_compiler.hpp>
#include <nrhi/directx11/shader_class.hpp>
#include <nrhi/directx11/shader_blob.hpp>

#include <d3dcompiler.h>



namespace nrhi {

	TU<A_shader_class> HD_directx11_shader_compiler::compile_hlsl(
		V_string name,
		V_string src_content,
		TG_span<F_shader_kernel_desc> kernel_descs
	) {

		TG_vector<TS_valid<A_shader_blob>> shader_blob_p_vector;

		for(u32 i = 0; i < kernel_descs.size(); ++i) {

			const auto& kernel_desc = kernel_descs[i];
			const auto& blob_desc = kernel_desc.blob_desc;

			ID3DBlob* d3d11_shader_blob_p = 0;
			ID3DBlob* d3d11_error_blob_p = 0;

			HRESULT hr;

			switch (kernel_desc.blob_desc.type)
			{
			case E_shader_type::VERTEX:
				hr = D3DCompile(
					src_content->data(),
					src_content->size() * sizeof(char),
					(name + "::" + blob_desc.name).data(),
					0,
					0,
					blob_desc.name.data(),
					"vs_5_0",
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("vertex")
					<< " shader blob "
					<< ncpp::T_cout_value(blob_desc.name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d11_error_blob_p->GetBufferPointer();
				break;
			case E_shader_type::PIXEL:
				hr = D3DCompile(
					src_content->data(),
					src_content->size() * sizeof(char),
					(name + "::" + blob_desc.name).data(),
					0,
					0,
					blob_desc.name.data(),
					"ps_5_0",
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("pixel")
					<< " shader blob "
					<< ncpp::T_cout_value(blob_desc.name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d11_error_blob_p->GetBufferPointer();
				break;
			case E_shader_type::COMPUTE:
				hr = D3DCompile(
					src_content->data(),
					src_content->size() * sizeof(char),
					(name + "::" + blob_desc.name).data(),
					0,
					0,
					blob_desc.name.data(),
					"cs_5_0",
					0,
					0,
					&d3d11_shader_blob_p,
					&d3d11_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("pixel")
					<< " shader blob "
					<< ncpp::T_cout_value(blob_desc.name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d11_error_blob_p->GetBufferPointer();
				break;
			}

			shader_blob_p_vector.push_back(
				TS_valid<F_directx11_shader_blob>::T_make(
					kernel_desc.blob_desc,
					blob_desc.type,
					d3d11_shader_blob_p
				)
			);
		}

		return TU<F_directx11_shader_class>()(
			F_shader_class_desc {
				.name = name,
				.shader_blob_p_vector = shader_blob_p_vector
			}
		);
	}

}
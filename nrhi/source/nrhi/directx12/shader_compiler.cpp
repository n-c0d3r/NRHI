#include <nrhi/directx12/shader_compiler.hpp>

#include <d3dcompiler.h>



namespace nrhi {

	F_shader_binary HD_directx12_shader_compiler::dxcapi_compile_hlsl_from_src_content(
		const G_string& class_name,
		const G_string& shader_name,
		const G_string& entry_point_name,
		const G_string& src_content,
		const G_string& abs_path,
		u32 model_major,
		u32 model_minor,
		ED_shader_type type
	)
	{
		NCPP_ASSERT(model_major >= 6);
		NCPP_ASSERT(type != ED_shader_type::NONE);

		NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_shader_type::VERTEX,
				model_minor = eastl::min<u32>(model_minor, 6);
			)
			NRHI_ENUM_CASE(
				ED_shader_type::PIXEL,
				model_minor = eastl::min<u32>(model_minor, 6);
			)
			NRHI_ENUM_CASE(
				ED_shader_type::COMPUTE,
				model_minor = eastl::min<u32>(model_minor, 6);
			)
		);

		G_string src_name = class_name + "::" + shader_name;

		G_wstring wide_model = G_to_wstring(model_major) + L"_" + G_to_wstring(model_minor);
		G_wstring wide_entry_point_name = G_to_wstring(entry_point_name);

		G_string shader_type_name;

		NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_shader_type::VERTEX,
				shader_type_name = "vertex";
				wide_model = L"vs_" + wide_model;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::PIXEL,
				shader_type_name = "pixel";
				wide_model = L"ps_" + wide_model;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::COMPUTE,
				shader_type_name = "compute";
				wide_model = L"cs_" + wide_model;
			)
		);

		Microsoft::WRL::ComPtr<IDxcCompiler3> pCompiler;
		Microsoft::WRL::ComPtr<IDxcUtils> pUtils;
		Microsoft::WRL::ComPtr<IDxcIncludeHandler> pIncludeHandler;

		DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));
		DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
		pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);

		DxcBuffer sourceBuffer;
		sourceBuffer.Ptr = src_content.data();
		sourceBuffer.Size = src_content.length() * sizeof(char);
		sourceBuffer.Encoding = DXC_CP_UTF8;

		const wchar_t* args[] = {
			L"-T", wide_model.c_str(), // Target profile
			L"-E", wide_entry_point_name.c_str(), // Entry point
		};

		Microsoft::WRL::ComPtr<IDxcResult> pResults;
		pCompiler->Compile(
			&sourceBuffer,
			args,
			_countof(args),
			0,
			IID_PPV_ARGS(pResults.GetAddressOf())
		);

		// Check for compilation errors
		Microsoft::WRL::ComPtr<IDxcBlobUtf8> pErrors;
		pResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);
		NCPP_ASSERT(!(pErrors && pErrors->GetStringLength() > 0))
			<< "can't compile "
			<< ncpp::T_cout_field_name(shader_type_name)
			<< " shader blob "
			<< ncpp::T_cout_value(entry_point_name)
			<< std::endl
			<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
			<< (char*)pErrors->GetBufferPointer();

		// Get the compiled shader
		Microsoft::WRL::ComPtr<IDxcBlob> pShader;
		pResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pShader), nullptr);

		return {
			(u8*)(pShader->GetBufferPointer()),
			(u8*)(pShader->GetBufferPointer()) + pShader->GetBufferSize()
		};
	}

	F_shader_binary HD_directx12_shader_compiler::compile_hlsl_from_src_content(
		const G_string& class_name,
		const G_string& shader_name,
		const G_string& entry_point_name,
		const G_string& src_content,
		const G_string& abs_path,
		u32 model_major,
		u32 model_minor,
		ED_shader_type type
	) {
		NCPP_ASSERT(model_major <= 5);

		ID3DBlob* d3d12_shader_blob_p = 0;
		ID3DBlob* d3d12_error_blob_p = 0;

		HRESULT hr;

		G_string src_name = class_name + "::" + shader_name;

		G_string model = G_to_string(model_major) + "_" + G_to_string(model_minor);

		NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_shader_type::VERTEX,
				model = "vs_" + model;
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d12_shader_blob_p,
					&d3d12_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("vertex")
					<< " shader blob "
					<< ncpp::T_cout_value(entry_point_name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d12_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::PIXEL,
				model = "ps_" + model;
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d12_shader_blob_p,
					&d3d12_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("pixel")
					<< " shader blob "
					<< ncpp::T_cout_value(entry_point_name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d12_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::COMPUTE,
				model = "cs_" + model;
				hr = D3DCompile(
					src_content.data(),
					src_content.size() * sizeof(char),
					src_name.data(),
					0,
					0,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d12_shader_blob_p,
					&d3d12_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
					<< "can't compile "
					<< ncpp::T_cout_field_name("compute")
					<< " shader blob "
					<< ncpp::T_cout_value(entry_point_name)
					<< std::endl
					<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
					<< (char*)d3d12_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
		);

		F_shader_binary result(d3d12_shader_blob_p->GetBufferSize());
		memcpy(
			(void*)(result.data()),
			(void*)(d3d12_shader_blob_p->GetBufferPointer()),
			result.size()
		);

		return std::move(result);
	}
	F_shader_binary HD_directx12_shader_compiler::compile_hlsl(
		const G_string& class_name,
		const G_string& shader_name,
		const G_string& entry_point_name,
		const G_string& abs_path,
		u32 model_major,
		u32 model_minor,
		ED_shader_type type
	) {
		NCPP_ASSERT(model_major <= 5);

		ID3DBlob* d3d12_shader_blob_p = 0;
		ID3DBlob* d3d12_error_blob_p = 0;

		HRESULT hr;

		G_string src_name = class_name + "::" + shader_name;

		G_wstring abs_wpath = G_to_wstring(abs_path);

		G_string model = G_to_string(model_major) + "_" + G_to_string(model_minor);

		NRHI_ENUM_SWITCH(
			type,
			NRHI_ENUM_CASE(
				ED_shader_type::VERTEX,
				model = "vs_" + model;
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d12_shader_blob_p,
					&d3d12_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
				<< "can't compile "
				<< ncpp::T_cout_field_name("vertex")
				<< " shader blob "
				<< ncpp::T_cout_value(entry_point_name)
				<< std::endl
				<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
				<< (char*)d3d12_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::PIXEL,
				model = "ps_" + model;
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d12_shader_blob_p,
					&d3d12_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
				<< "can't compile "
				<< ncpp::T_cout_field_name("pixel")
				<< " shader blob "
				<< ncpp::T_cout_value(entry_point_name)
				<< std::endl
				<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
				<< (char*)d3d12_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
			NRHI_ENUM_CASE(
				ED_shader_type::COMPUTE,
				model = "cs_" + model;
				hr = D3DCompileFromFile(
					abs_wpath.data(),
					0,
					D3D_COMPILE_STANDARD_FILE_INCLUDE,
					entry_point_name.data(),
					model.c_str(),
					0,
					0,
					&d3d12_shader_blob_p,
					&d3d12_error_blob_p
				);
				NCPP_ASSERT(!FAILED(hr))
				<< "can't compile "
				<< ncpp::T_cout_field_name("compute")
				<< " shader blob "
				<< ncpp::T_cout_value(entry_point_name)
				<< std::endl
				<< ncpp::E_log_color::V_FOREGROUND_BRIGHT_RED
				<< (char*)d3d12_error_blob_p->GetBufferPointer();
				NRHI_ENUM_BREAK;
			)
		);

		F_shader_binary result(d3d12_shader_blob_p->GetBufferSize());
		memcpy(
			(void*)(result.data()),
			(void*)(d3d12_shader_blob_p->GetBufferPointer()),
			result.size()
		);
		
		return std::move(result);
	}
	F_shader_binary HD_directx12_shader_compiler::compile_nsl(
		const F_nsl_compiled_result& compiled_result,
		u32 shader_index
	) {
		u32 model_major = 0;
		u32 model_minor = 0;

		switch (compiled_result.output_language_enum)
		{
		case E_nsl_output_language::HLSL_4:
			model_major = 4;
			break;
		case E_nsl_output_language::HLSL_5:
			model_major = 5;
			break;
		case E_nsl_output_language::HLSL_5_1:
			model_major = 5;
			model_minor = 1;
			break;
		case E_nsl_output_language::HLSL_6_0:
			model_major = 6;
			model_minor = 0;
			break;
		case E_nsl_output_language::HLSL_6_1:
			model_major = 6;
			model_minor = 1;
			break;
		case E_nsl_output_language::HLSL_6_2:
			model_major = 6;
			model_minor = 2;
			break;
		case E_nsl_output_language::HLSL_6_3:
			model_major = 6;
			model_minor = 3;
			break;
		case E_nsl_output_language::HLSL_6_4:
			model_major = 6;
			model_minor = 4;
			break;
		case E_nsl_output_language::HLSL_6_5:
			model_major = 6;
			model_minor = 5;
			break;
		case E_nsl_output_language::HLSL_6_6:
			model_major = 6;
			model_minor = 6;
			break;
		case E_nsl_output_language::HLSL_6_7:
			model_major = 6;
			model_minor = 7;
			break;
		}

		if(model_major >= 6)
		{
			return dxcapi_compile_hlsl_from_src_content(
				compiled_result.class_name,
				compiled_result.reflection.shaders[shader_index].name,
				"main",
				compiled_result.build(shader_index),
				"",
				model_major,
				model_minor,
				compiled_result.reflection.shaders[shader_index].type
			);
		}

		return compile_hlsl_from_src_content(
			compiled_result.class_name,
			compiled_result.reflection.shaders[shader_index].name,
			"main",
			compiled_result.build(shader_index),
			"",
			model_major,
			model_minor,
			compiled_result.reflection.shaders[shader_index].type
		);
	}

}
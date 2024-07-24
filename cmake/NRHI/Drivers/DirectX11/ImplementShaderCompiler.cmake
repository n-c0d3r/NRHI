
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing shader compiler")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SHADER_COMPILER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader_compiler.hpp")
NCPP_SetGlobal(NRHI_SHADER_COMPILER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader_compiler.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_shader_compiler"
    DRIVER_SPECIFIC_NAME "HD_directx11_shader_compiler"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_SHADER_COMPILER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SHADER_COMPILER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/shader_compiler.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "compile_hlsl_from_src_content" "TG_vector<u8>(
			const G_string& class_name,
			const G_string& entry_point_name,
            const G_string& src_content,
            const G_string& abs_path,
			u32 model_major,
			u32 model_minor,
			ED_shader_type type
        )"
        "compile_hlsl" "TG_vector<u8>(
			const G_string& class_name,
			const G_string& entry_point_name,
            const G_string& abs_path,
			u32 model_major,
			u32 model_minor,
			ED_shader_type type
        )"
        "compile_nsl" "TG_vector<u8>(
			const G_string& shader_class_name,
			const F_nsl_compiled_result& compiled_result,
			u32 shader_index
		)"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement shader compiler done")
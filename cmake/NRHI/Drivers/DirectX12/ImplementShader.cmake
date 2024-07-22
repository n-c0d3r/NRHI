
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing shader")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SHADER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader.hpp")
NCPP_SetGlobal(NRHI_SHADER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_shader"
    DRIVER_SPECIFIC_NAME "HD_directx12_shader"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_SHADER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SHADER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/shader.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "create" "TU<A_shader>(
            TKPA_valid<A_device> device_p,
            const F_shader_desc& desc
        )"
        "create_vertex_shader" "U_vertex_shader_handle(
            TKPA_valid<A_device> device_p,
            const F_shader_desc& desc
        )"
        "create_pixel_shader" "U_pixel_shader_handle(
            TKPA_valid<A_device> device_p,
            const F_shader_desc& desc
        )"
        "create_compute_shader" "U_compute_shader_handle(
            TKPA_valid<A_device> device_p,
            const F_shader_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement shader done")
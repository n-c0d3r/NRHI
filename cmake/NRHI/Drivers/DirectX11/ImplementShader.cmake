
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing shader")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SHADER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader.hpp")
NCPP_SetGlobal(NRHI_SHADER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_shader"
    DRIVER_SPECIFIC_NAME "HD_directx11_shader"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_SHADER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SHADER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/shader.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement shader done")

message(STATUS "<NRHI::Drivers::DirectX11> Start implementing shader library")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SHADER_LIBRARY_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader_library.hpp")
NCPP_SetGlobal(NRHI_SHADER_LIBRARY_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader_library.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_shader_library"
    DRIVER_SPECIFIC_NAME "HD_directx11_shader_library"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_SHADER_LIBRARY_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SHADER_LIBRARY_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/shader_library.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement shader library done")
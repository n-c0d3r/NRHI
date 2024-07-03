
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing shader type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_SHADER_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader_type.hpp")
NCPP_SetGlobal(NRHI_SHADER_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_shader_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_SHADER_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SHADER_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE 0
        VERTEX 1
        PIXEL 2
        COMPUTE 3
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement shader type done")
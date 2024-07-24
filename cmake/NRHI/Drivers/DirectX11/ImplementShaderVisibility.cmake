
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing shader visibility")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_SHADER_VISIBILITY_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader_visibility.hpp")
NCPP_SetGlobal(NRHI_SHADER_VISIBILITY_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/shader_visibility.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_shader_visibility"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_SHADER_VISIBILITY_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SHADER_VISIBILITY_ENUM_CPP_FILE_PATH}"
    VALUES
        ALL 0
        VERTEX 1
        PIXEL 5
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement shader visibility done")
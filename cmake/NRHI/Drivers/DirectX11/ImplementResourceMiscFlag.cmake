
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource misc flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_MISC_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_misc_flag.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_MISC_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_misc_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_resource_misc_flag"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_MISC_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_MISC_FLAG_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE 0
        GENERATE_MIPS D3D11_RESOURCE_MISC_GENERATE_MIPS
        TEXTURECUBE D3D11_RESOURCE_MISC_TEXTURECUBE
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource misc flag done")
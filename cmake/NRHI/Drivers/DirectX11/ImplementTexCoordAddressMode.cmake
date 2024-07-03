
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing texcoord address mode")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_UV_ADDRESS_MODE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/texcoord_address_mode.hpp")
NCPP_SetGlobal(NRHI_UV_ADDRESS_MODE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/texcoord_address_mode.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_texcoord_address_mode"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_UV_ADDRESS_MODE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_UV_ADDRESS_MODE_ENUM_CPP_FILE_PATH}"
    VALUES
        WRAP D3D11_TEXTURE_ADDRESS_WRAP
        MIRROR D3D11_TEXTURE_ADDRESS_MIRROR
        CLAMP D3D11_TEXTURE_ADDRESS_CLAMP
        BORDER D3D11_TEXTURE_ADDRESS_BORDER
        MIRROR_ONCE D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement texcoord address mode done")
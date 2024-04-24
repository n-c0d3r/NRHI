
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing cull mode")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_CULL_MODE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/cull_mode.hpp")
NCPP_SetGlobal(NRHI_CULL_MODE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/cull_mode.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_cull_mode"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_CULL_MODE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_CULL_MODE_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE D3D11_CULL_NONE
        FRONT D3D11_CULL_FRONT
        BACK D3D11_CULL_BACK
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement cull mode done")
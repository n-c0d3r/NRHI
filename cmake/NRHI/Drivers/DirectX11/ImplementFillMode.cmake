
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing fill mode")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_FILL_MODE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/fill_mode.hpp")
NCPP_SetGlobal(NRHI_FILL_MODE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/fill_mode.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_fill_mode"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_FILL_MODE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_FILL_MODE_ENUM_CPP_FILE_PATH}"
    VALUES
        WIREFRAME D3D11_FILL_WIREFRAME
        SOLID D3D11_FILL_SOLID
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement fill mode done")
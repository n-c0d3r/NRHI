
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing clear flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_CLEAR_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/clear_flag.hpp")
NCPP_SetGlobal(NRHI_CLEAR_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/clear_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_clear_flag"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_CLEAR_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_CLEAR_FLAG_ENUM_CPP_FILE_PATH}"
    VALUES
        DEPTH D3D11_CLEAR_DEPTH
        STENCIL D3D11_CLEAR_STENCIL
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement clear flag done")
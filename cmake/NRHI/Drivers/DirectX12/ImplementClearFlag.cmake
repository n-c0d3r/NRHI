
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing clear flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_CLEAR_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/clear_flag.hpp")
NCPP_SetGlobal(NRHI_CLEAR_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/clear_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_clear_flag"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_CLEAR_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_CLEAR_FLAG_ENUM_CPP_FILE_PATH}"
    FLAG_OPERATORS
    VALUES
        DEPTH D3D12_CLEAR_FLAG_DEPTH
        STENCIL D3D12_CLEAR_FLAG_STENCIL
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement clear flag done")
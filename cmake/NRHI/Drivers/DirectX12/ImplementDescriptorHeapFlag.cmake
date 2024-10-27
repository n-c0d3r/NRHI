
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing descriptor heap flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_DESCRIPTOR_HEAP_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor_heap_flag.hpp")
NCPP_SetGlobal(NRHI_DESCRIPTOR_HEAP_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor_heap_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_descriptor_heap_flag"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_DESCRIPTOR_HEAP_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DESCRIPTOR_HEAP_FLAG_ENUM_CPP_FILE_PATH}"
    FLAG_OPERATORS
    VALUES
        NONE D3D12_DESCRIPTOR_HEAP_FLAG_NONE
        SHADER_VISIBLE D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement descriptor heap flag done")
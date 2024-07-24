
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing resource heap type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_HEAP_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_heap_type.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_HEAP_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_heap_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_resource_heap_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_HEAP_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_HEAP_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        DEFAULT D3D12_HEAP_TYPE_DEFAULT
        GREAD_GWRITE D3D12_HEAP_TYPE_DEFAULT
        GREAD_CWRITE D3D12_HEAP_TYPE_UPLOAD
        CREAD_GWRITE D3D12_HEAP_TYPE_READBACK
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement resource heap type done")
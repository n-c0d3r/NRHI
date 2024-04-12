
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource heap type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_HEAP_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_heap_type.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_HEAP_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_heap_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_resource_heap_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_HEAP_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_HEAP_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        DEFAULT D3D11_USAGE_DEFAULT
        GREAD_GWRITE D3D11_USAGE_DEFAULT
        GREAD_CWRITE D3D11_USAGE_DYNAMIC
        CREAD_GWRITE D3D11_USAGE_STAGING
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource heap type done")
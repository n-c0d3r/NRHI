
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource heap type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_USAGE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_usage.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_USAGE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_usage.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_resource_usage"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_USAGE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_USAGE_ENUM_CPP_FILE_PATH}"
    VALUES
        DEFAULT D3D11_USAGE_DEFAULT
        GREAD_GWRITE D3D11_USAGE_DEFAULT
        GREAD_CWRITE D3D11_USAGE_DYNAMIC
        CREAD_GWRITE D3D11_USAGE_STAGING
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource heap type done")

message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource view type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_VIEW_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_view_type.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_VIEW_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_view_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_resource_view_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_VIEW_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_VIEW_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        SRV 0
        UAV 1
        RTV 2
        DSV 3
        SAMPLER 4
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource view type done")

message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_type.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_resource_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE 0
        BUFFER 1
        TEXTURE_1D 2
        TEXTURE_1D_ARRAY 3
        TEXTURE_2D 4
        TEXTURE_2D_ARRAY 5
        TEXTURE_3D 6
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource type done")
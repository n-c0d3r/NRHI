
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_type.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_resource_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE 0
        BUFFER 1
        STRUCTURED_BUFFER 2
        SINGLE_ELEMENTED_BUFFER 3
        VERTEX_BUFFER 4
        INDEX_BUFFER 5
        TEXTURE_1D 6
        TEXTURE_1D_ARRAY 7
        TEXTURE_2D 8
        TEXTURE_2D_ARRAY 9
        TEXTURE_3D 10
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource type done")
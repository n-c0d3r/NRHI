
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing texture copy location type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_TEXTURE_COPY_LOCATION_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/texture_copy_location_type.hpp")
NCPP_SetGlobal(NRHI_TEXTURE_COPY_LOCATION_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/texture_copy_location_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_texture_copy_location_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_TEXTURE_COPY_LOCATION_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_TEXTURE_COPY_LOCATION_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        SUBRESOURCE_INDEX 0
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement texture copy location type done")
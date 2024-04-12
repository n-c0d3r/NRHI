
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource dimension")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_RESOURCE_DIMENSION_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_dimension.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_DIMENSION_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_dimension.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "E_resource_dimension"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_DIMENSION_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_DIMENSION_ENUM_CPP_FILE_PATH}"
    VALUES
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource dimension done")
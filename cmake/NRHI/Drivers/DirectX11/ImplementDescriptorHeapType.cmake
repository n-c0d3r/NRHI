
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing descriptor heap type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_DESCRIPTOR_HEAP_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor_heap_type.hpp")
NCPP_SetGlobal(NRHI_DESCRIPTOR_HEAP_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/descriptor_heap_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_descriptor_heap_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_DESCRIPTOR_HEAP_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_DESCRIPTOR_HEAP_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        MANAGED 0x4
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement descriptor heap type done")
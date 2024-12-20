
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing state object type")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_STATE_OBJECT_TYPE_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object_type.hpp")
NCPP_SetGlobal(NRHI_STATE_OBJECT_TYPE_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object_type.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_state_object_type"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_STATE_OBJECT_TYPE_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_STATE_OBJECT_TYPE_ENUM_CPP_FILE_PATH}"
    VALUES
        COLLECTION D3D12_STATE_OBJECT_TYPE_COLLECTION
        RAYTRACING_PIPELINE D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE
        EXECUTABLE D3D12_STATE_OBJECT_TYPE_EXECUTABLE
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement state object type done")

message(STATUS "<NRHI::Drivers::DirectX12> Start implementing primitive topology")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_PRIMITIVE_TOPOLOGY_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/primitive_topology.hpp")
NCPP_SetGlobal(NRHI_PRIMITIVE_TOPOLOGY_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/primitive_topology.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_primitive_topology"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_PRIMITIVE_TOPOLOGY_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_PRIMITIVE_TOPOLOGY_ENUM_CPP_FILE_PATH}"
    VALUES
        NONE D3D_PRIMITIVE_TOPOLOGY_UNDEFINED
        TRIANGLE_LIST D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
        LINE_LIST D3D_PRIMITIVE_TOPOLOGY_LINELIST
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement primitive topology done")
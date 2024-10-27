
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing bind work graph program flag")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_BIND_WORK_GRAPH_PROGRAM_FLAG_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/bind_work_graph_program_flag.hpp")
NCPP_SetGlobal(NRHI_BIND_WORK_GRAPH_PROGRAM_FLAG_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/bind_work_graph_program_flag.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_bind_work_graph_program_flag"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_BIND_WORK_GRAPH_PROGRAM_FLAG_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_BIND_WORK_GRAPH_PROGRAM_FLAG_ENUM_CPP_FILE_PATH}"
    FLAG_OPERATORS
    VALUES
        NONE D3D12_SET_WORK_GRAPH_FLAG_NONE
        INITIALIZE D3D12_SET_WORK_GRAPH_FLAG_INITIALIZE
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement bind work graph program flag done")
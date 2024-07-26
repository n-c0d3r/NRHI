
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing logic operation")

include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_LOGIC_OPERATION_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/logic_operation.hpp")
NCPP_SetGlobal(NRHI_LOGIC_OPERATION_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/logic_operation.cpp")



NRHI_EnumHelper_CreateEnum(
    NAMESPACE "nrhi"
    NAME "ED_logic_operation"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_LOGIC_OPERATION_ENUM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_LOGIC_OPERATION_ENUM_CPP_FILE_PATH}"
    VALUES
        CLEAR D3D12_LOGIC_OP_CLEAR
        SET D3D12_LOGIC_OP_SET
        COPY D3D12_LOGIC_OP_COPY
        COPY_INVERTED D3D12_LOGIC_OP_COPY_INVERTED
        NOOP D3D12_LOGIC_OP_NOOP
        INVERT D3D12_LOGIC_OP_INVERT
        AND D3D12_LOGIC_OP_AND
        NAND D3D12_LOGIC_OP_NAND
        OR D3D12_LOGIC_OP_OR
        NOR D3D12_LOGIC_OP_NOR
        XOR D3D12_LOGIC_OP_XOR
        EQUIV D3D12_LOGIC_OP_EQUIV
        AND_REVERSE D3D12_LOGIC_OP_AND_REVERSE
        AND_INVERTED D3D12_LOGIC_OP_AND_INVERTED
        OR_REVERSE D3D12_LOGIC_OP_OR_REVERSE
        OR_INVERTED D3D12_LOGIC_OP_OR_INVERTED
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement logic operation done")
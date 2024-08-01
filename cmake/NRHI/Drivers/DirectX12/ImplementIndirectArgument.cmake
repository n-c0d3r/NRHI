
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing indirect argument")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_INDIRECT_ARGUMENT_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/indirect_argument.hpp")
NCPP_SetGlobal(NRHI_INDIRECT_ARGUMENT_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/indirect_argument.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_indirect_argument"
    DRIVER_SPECIFIC_NAME "HD_directx12_indirect_argument"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_INDIRECT_ARGUMENT_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_INDIRECT_ARGUMENT_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/indirect_argument.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "size" "u32(
            const F_indirect_argument_desc& desc
        )"
        "alignment" "u32(
            ED_indirect_argument_type type
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement indirect argument done")
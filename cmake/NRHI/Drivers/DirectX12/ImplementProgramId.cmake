
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing program id")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_PROGRAM_ID_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/program_id.hpp")
NCPP_SetGlobal(NRHI_PROGRAM_ID_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/program_id.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_program_id"
    DRIVER_SPECIFIC_NAME "HD_directx12_program_id"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_PROGRAM_ID_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_PROGRAM_ID_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/program_id.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "construct_default" "void(
            void* payload_p
        )"
        "destruct" "void(
            void* payload_p
        )"
        "construct_copy" "void(
            void* payload_p,
            void* x_payload_p
        )"
        "operator_copy" "void(
            void* payload_p,
            void* x_payload_p
        )"
        "construct_move" "void(
            void* payload_p,
            void* x_payload_p
        )"
        "operator_move" "void(
            void* payload_p,
            void* x_payload_p
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement program id done")
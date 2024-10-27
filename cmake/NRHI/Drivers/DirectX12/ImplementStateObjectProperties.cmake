
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing state object properties")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_STATE_OBJECT_PROPERTIES_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object_properties.hpp")
NCPP_SetGlobal(NRHI_STATE_OBJECT_PROPERTIES_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object_properties.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_state_object_properties"
    DRIVER_SPECIFIC_NAME "HD_directx12_state_object_properties"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_STATE_OBJECT_PROPERTIES_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_STATE_OBJECT_PROPERTIES_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/state_object_properties.hpp>"
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
        "construct_with_state_object" "void(
            void* payload_p,
            TKPA_valid<A_state_object> state_object_p
        )"
        "compare" "b8(
            void* payload_p,
            void* x_payload_p
        )"
        "program_id" "F_program_id(
            void* payload_p,
            const G_string& name
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement state object properties done")

message(STATUS "<NRHI::Drivers::DirectX12> Start implementing state object builder")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_STATE_OBJECT_BUILDER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object_builder.hpp")
NCPP_SetGlobal(NRHI_STATE_OBJECT_BUILDER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object_builder.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_state_object_builder"
    DRIVER_SPECIFIC_NAME "HD_directx12_state_object_builder"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_STATE_OBJECT_BUILDER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_STATE_OBJECT_BUILDER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/state_object_builder.hpp>"
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
        "construct_with_type" "void(
            void* payload_p,
            ED_state_object_type type
        )"
        "add_state_object_config" "F_state_object_config_subobject&(
            void* payload_p
        )"
        "add_work_graph" "F_work_graph_subobject&(
            void* payload_p
        )"
        "add_library" "F_library_subobject&(
            void* payload_p
        )"
        "build" "TU<A_state_object>(
            void* payload_p,
            TKPA_valid<A_device> device_p
        )"
        "add_to" "TU<A_state_object>(
            void* payload_p,
            TKPA_valid<A_state_object> src_state_object_p
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement state object builder done")
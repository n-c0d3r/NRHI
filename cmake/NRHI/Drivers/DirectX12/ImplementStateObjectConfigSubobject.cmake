
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing state object config subobject")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_STATE_OBJECT_CONFIG_SUBOBJECT_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object_config_subobject.hpp")
NCPP_SetGlobal(NRHI_STATE_OBJECT_CONFIG_SUBOBJECT_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/state_object_config_subobject.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_state_object_config_subobject"
    DRIVER_SPECIFIC_NAME "HD_directx12_state_object_config_subobject"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_STATE_OBJECT_CONFIG_SUBOBJECT_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_STATE_OBJECT_CONFIG_SUBOBJECT_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/state_object_config_subobject.hpp>"
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
        "set_flags" "void(
            void* payload_p,
            ED_state_object_flag flags
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement state object config subobject done")
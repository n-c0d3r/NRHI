
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing global root signature subobject")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_GLOBAL_ROOT_SIGNATURE_SUBOBJECT_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/global_root_signature_subobject.hpp")
NCPP_SetGlobal(NRHI_GLOBAL_ROOT_SIGNATURE_SUBOBJECT_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/global_root_signature_subobject.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_global_root_signature_subobject"
    DRIVER_SPECIFIC_NAME "HD_directx12_global_root_signature_subobject"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_GLOBAL_ROOT_SIGNATURE_SUBOBJECT_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_GLOBAL_ROOT_SIGNATURE_SUBOBJECT_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/global_root_signature_subobject.hpp>"
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
        "set_root_signature_p" "void(
            void* payload_p,
            TKPA_valid<A_root_signature> root_signature_p
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement global root signature subobject done")
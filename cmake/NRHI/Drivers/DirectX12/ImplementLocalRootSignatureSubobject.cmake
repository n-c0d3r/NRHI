
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing local root signature subobject")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_LOCAL_ROOT_SIGNATURE_SUBOBJECT_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/local_root_signature_subobject.hpp")
NCPP_SetGlobal(NRHI_LOCAL_ROOT_SIGNATURE_SUBOBJECT_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/local_root_signature_subobject.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_local_root_signature_subobject"
    DRIVER_SPECIFIC_NAME "HD_directx12_local_root_signature_subobject"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_LOCAL_ROOT_SIGNATURE_SUBOBJECT_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_LOCAL_ROOT_SIGNATURE_SUBOBJECT_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/local_root_signature_subobject.hpp>"
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

message(STATUS "<NRHI::Drivers::DirectX12> Implement local root signature subobject done")
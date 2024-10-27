
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing library subobject")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_LIBRARY_SUBOBJECT_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/library_subobject.hpp")
NCPP_SetGlobal(NRHI_LIBRARY_SUBOBJECT_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/library_subobject.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_library_subobject"
    DRIVER_SPECIFIC_NAME "HD_directx12_library_subobject"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_LIBRARY_SUBOBJECT_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_LIBRARY_SUBOBJECT_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/library_subobject.hpp>"
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
        "set_library" "void(
            void* payload_p,
            const TG_span<u8>& binary
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement library subobject done")
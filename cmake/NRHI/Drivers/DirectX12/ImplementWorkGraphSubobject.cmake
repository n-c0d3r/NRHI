
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing work graph subobject")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_WORK_GRAPH_SUBOBJECT_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/work_graph_subobject.hpp")
NCPP_SetGlobal(NRHI_WORK_GRAPH_SUBOBJECT_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/work_graph_subobject.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_work_graph_subobject"
    DRIVER_SPECIFIC_NAME "HD_directx12_work_graph_subobject"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_WORK_GRAPH_SUBOBJECT_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_WORK_GRAPH_SUBOBJECT_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/work_graph_subobject.hpp>"
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
        "include_all_available_nodes" "void(void* payload_p)"
        "set_debug_name" "void(void* payload_p, const F_debug_name& value)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement work graph subobject done")
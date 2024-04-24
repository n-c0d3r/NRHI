
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing command_list")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_COMMAND_LIST_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list.hpp")
NCPP_SetGlobal(NRHI_COMMAND_LIST_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_list.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_command_list"
    DRIVER_SPECIFIC_NAME "HD_directx11_command_list"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_LIST_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_LIST_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/command_list.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create" "TU<A_command_list>(TK_valid<A_device> device_p, const F_command_list_desc& desc)"
        "set_frame_buffer" "void(
            TK_valid<A_command_list> command_list_p,
            TK_valid<A_frame_buffer> frame_buffer_p
        )"
        "set_graphics_pipeline_state" "void(
            TK_valid<A_command_list> command_list_p,
            K_valid_graphics_pipeline_state_handle graphics_pipeline_state_p
        )"
        "set_compute_pipeline_state" "void(
            TK_valid<A_command_list> command_list_p,
            K_valid_compute_pipeline_state_handle compute_pipeline_state_p
        )"
        "draw_indexed" "void(
            TK_valid<A_command_list> command_list_p,
            u32 index_count,
            u32 base_index_location
        )"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement command_list done")
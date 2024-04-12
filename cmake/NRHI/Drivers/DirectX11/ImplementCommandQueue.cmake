
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing command_queue")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_COMMAND_QUEUE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_queue.hpp")
NCPP_SetGlobal(NRHI_COMMAND_QUEUE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_queue.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_command_queue"
    DRIVER_SPECIFIC_NAME "HD_directx11_command_queue"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_QUEUE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_QUEUE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/command_queue.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create" "TU<A_command_queue>(TK_valid<A_device> device_p, const F_command_queue_desc& desc)"
        "execute_command_lists" "void(
            TK_valid<A_command_queue> command_queue_p,
            TG_span<TK_valid<A_command_list>> command_list_p_span
        )"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement command_queue done")
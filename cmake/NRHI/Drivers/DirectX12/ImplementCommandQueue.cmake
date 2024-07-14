
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing command_queue")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_COMMAND_QUEUE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_queue.hpp")
NCPP_SetGlobal(NRHI_COMMAND_QUEUE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_queue.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_command_queue"
    DRIVER_SPECIFIC_NAME "HD_directx12_command_queue"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_QUEUE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_QUEUE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/command_queue.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create" "TU<A_command_queue>(TKPA_valid<A_device> device_p, const F_command_queue_desc& desc)"
        "async_signal" "void(
			TKPA_valid<A_command_queue> command_queue_p,
			TKPA_valid<A_fence> fence_p,
			u64 new_value
		)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement command_queue done")

message(STATUS "<NRHI::Drivers::DirectX12> Start implementing command allocator")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_COMMAND_ALLOCATOR_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_allocator.hpp")
NCPP_SetGlobal(NRHI_COMMAND_ALLOCATOR_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_allocator.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_command_allocator"
    DRIVER_SPECIFIC_NAME "HD_directx12_command_allocator"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_ALLOCATOR_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_ALLOCATOR_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/command_allocator.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create" "TU<A_command_allocator>(TKPA_valid<A_device> device_p, const F_command_allocator_desc& desc)"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement command allocator done")
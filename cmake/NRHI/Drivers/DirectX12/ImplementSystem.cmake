
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing system")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_SYSTEM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/system.hpp")
NCPP_SetGlobal(NRHI_SYSTEM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/system.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_system"
    DRIVER_SPECIFIC_NAME "HD_directx12_system"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_SYSTEM_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_SYSTEM_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/system.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "initialize" "void()"
        "release" "void()"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement system done")
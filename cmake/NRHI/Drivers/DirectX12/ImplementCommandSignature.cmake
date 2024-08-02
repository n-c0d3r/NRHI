
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing command signature")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_COMMAND_SIGNATURE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_signature.hpp")
NCPP_SetGlobal(NRHI_COMMAND_SIGNATURE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_signature.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_command_signature"
    DRIVER_SPECIFIC_NAME "HD_directx12_command_signature"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_COMMAND_SIGNATURE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_COMMAND_SIGNATURE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/command_signature.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "create" "TU<A_command_signature>(
            TKPA_valid<A_device> device_p,
            const F_command_signature_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement command signature done")
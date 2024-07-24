
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing root signature")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_ROOT_SIGNATURE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/root_signature.hpp")
NCPP_SetGlobal(NRHI_ROOT_SIGNATURE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/root_signature.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_root_signature"
    DRIVER_SPECIFIC_NAME "HD_directx12_root_signature"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_ROOT_SIGNATURE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_ROOT_SIGNATURE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/root_signature.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
        }"
    VALUES
        "create" "TU<A_root_signature>(
			TKPA_valid<A_device> device_p,
			const F_root_signature_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement root signature done")
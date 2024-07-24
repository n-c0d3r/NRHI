
message(STATUS "<NRHI::Drivers::DirectX12> Start implementing adapter")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_ADAPTER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/adapter.hpp")
NCPP_SetGlobal(NRHI_ADAPTER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/adapter.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_adapter"
    DRIVER_SPECIFIC_NAME "HD_directx12_adapter"
    DRIVER_UPPER_CASE_NAME "DIRECTX_12"
    TARGET_HPP_FILE_PATH "${NRHI_ADAPTER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_ADAPTER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx12/adapter.hpp>"
    VALUES
        "adapter_p_vector" "const TG_vector<TK_valid<A_adapter>>&()"
)

message(STATUS "<NRHI::Drivers::DirectX12> Implement adapter done")
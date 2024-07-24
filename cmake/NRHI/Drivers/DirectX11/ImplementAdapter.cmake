
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing adapter")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_ADAPTER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/adapter.hpp")
NCPP_SetGlobal(NRHI_ADAPTER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/adapter.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_adapter"
    DRIVER_SPECIFIC_NAME "HD_directx11_adapter"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_ADAPTER_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_ADAPTER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/adapter.hpp>"
    VALUES
        "adapter_p_vector" "const TG_vector<TK_valid<A_adapter>>&()"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement adapter done")

message(STATUS "<NRHI::Drivers::DirectX11> Start implementing format helper")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_FORMAT_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/format_helper.hpp")
NCPP_SetGlobal(NRHI_FORMAT_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/format_helper.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_format"
    DRIVER_SPECIFIC_NAME "HD_directx11_format"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_FORMAT_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_FORMAT_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/format_helper.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "stride" "u32(E_format)"
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement format helper done")
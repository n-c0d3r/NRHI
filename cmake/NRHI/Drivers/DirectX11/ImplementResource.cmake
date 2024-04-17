
message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_RESOURCE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_resource"
    DRIVER_SPECIFIC_NAME "HD_directx11_resource"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/resource.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource done")

message(STATUS "<NRHI::Drivers::DirectX11> Start implementing resource view")

include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_RESOURCE_VIEW_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_view.hpp")
NCPP_SetGlobal(NRHI_RESOURCE_VIEW_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/resource_view.cpp")



NRHI_FunctionHelper_CreateFunctionClass(
    NAMESPACE "nrhi"
    NAME "H_resource_view"
    DRIVER_SPECIFIC_NAME "HD_directx11_resource_view"
    DRIVER_UPPER_CASE_NAME "DIRECTX_11"
    TARGET_HPP_FILE_PATH "${NRHI_RESOURCE_VIEW_HPP_FILE_PATH}"
    TARGET_CPP_FILE_PATH "${NRHI_RESOURCE_VIEW_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/directx11/resource_view.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
)

message(STATUS "<NRHI::Drivers::DirectX11> Implement resource view done")
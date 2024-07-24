
include(NRHI/Utilities/FunctionHelper)



NCPP_SetGlobal(NRHI_ADAPTER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/adapter.hpp")
NCPP_SetGlobal(NRHI_ADAPTER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/adapter.cpp")



function(NRHI_DXGI_ImplementAdapter)

    cmake_parse_arguments(
        PARGS
        ""
        "DRIVER_UPPER_CASE_NAME"
        ""
        ${ARGN}
    )

    NRHI_FunctionHelper_CreateFunctionClass(
        NAMESPACE "nrhi"
        NAME "H_adapter"
        DRIVER_SPECIFIC_NAME "HD_dxgi_adapter"
        DRIVER_UPPER_CASE_NAME "${PARGS_DRIVER_UPPER_CASE_NAME}"
        TARGET_HPP_FILE_PATH "${NRHI_ADAPTER_HPP_FILE_PATH}"
        TARGET_CPP_FILE_PATH "${NRHI_ADAPTER_CPP_FILE_PATH}"
        INCLUDES
            "<nrhi/dxgi/adapter.hpp>"
        VALUES
            "adapter_p_vector" "const TG_vector<TK_valid<A_adapter>>&()"
    )

endfunction()
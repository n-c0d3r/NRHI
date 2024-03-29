
include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_FORMAT_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/format.hpp")
NCPP_SetGlobal(NRHI_FORMAT_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/format.cpp")



function(NRHI_DXGI_ImplementFormats)

    cmake_parse_arguments(
        PARGS
        ""
        "DRIVER_UPPER_CASE_NAME;DRIVER_LOWER_CASE_NAME"
        ""
        ${ARGN}
    )

    NRHI_EnumHelper_CreateEnum(
        NAMESPACE "nrhi"
        NAME "E_format"
        DRIVER_UPPER_CASE_NAME ${PARGS_DRIVER_UPPER_CASE_NAME}
        DRIVER_LOWER_CASE_NAME ${PARGS_DRIVER_LOWER_CASE_NAME}
        TARGET_HPP_FILE_PATH "${NRHI_FORMAT_ENUM_HPP_FILE_PATH}"
        TARGET_CPP_FILE_PATH "${NRHI_FORMAT_ENUM_CPP_FILE_PATH}"
        VALUES
            NONE DXGI_FORMAT_UNKNOWN
    )

endfunction()
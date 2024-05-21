
include(NRHI/Utilities/EnumHelper)



NCPP_SetGlobal(NRHI_FORMAT_ENUM_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/format.hpp")
NCPP_SetGlobal(NRHI_FORMAT_ENUM_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/format.cpp")



function(NRHI_DXGI_ImplementFormats)

    cmake_parse_arguments(
        PARGS
        ""
        "DRIVER_UPPER_CASE_NAME"
        ""
        ${ARGN}
    )

    NRHI_EnumHelper_CreateEnum(
        NAMESPACE "nrhi"
        NAME "E_format"
        DRIVER_UPPER_CASE_NAME ${PARGS_DRIVER_UPPER_CASE_NAME}
        TARGET_HPP_FILE_PATH "${NRHI_FORMAT_ENUM_HPP_FILE_PATH}"
        TARGET_CPP_FILE_PATH "${NRHI_FORMAT_ENUM_CPP_FILE_PATH}"
        VALUES
            NONE DXGI_FORMAT_UNKNOWN
            R8G8B8A8_UNORM DXGI_FORMAT_R8G8B8A8_UNORM
            R32_UINT DXGI_FORMAT_R32_UINT
            R32G32B32A32_FLOAT  DXGI_FORMAT_R32G32B32A32_FLOAT
            R32G32B32_FLOAT  DXGI_FORMAT_R32G32B32_FLOAT
            R32G32_FLOAT  DXGI_FORMAT_R32G32_FLOAT
            R32_FLOAT  DXGI_FORMAT_R32_FLOAT
            D32_FLOAT  DXGI_FORMAT_D32_FLOAT
    )

endfunction()
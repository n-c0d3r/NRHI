
include(NRHI/Utilities/EnumHelper)



function(NRHI_DXGI_ImplementFormats)

    cmake_parse_arguments(
        PARGS
        ""
        "DRIVER_UPPER_CASE_NAME;DRIVER_LOWER_CASE_NAME"
        ""
        ${ARGN}
    )

#    NRHI_EnumHelper_CreateEnum(
#        NAMESPACE "nrhi"
#        NAME "E_"
#    )
#
#    cmake_parse_arguments(
#        PARGS
#        "ENABLE_MASK_MODE"
#        "NAMESPACE;NAME;DRIVER_UPPER_CASE_NAME;DRIVER_LOWER_CASE_NAME;TARGET_FILE_PATH"
#        "VALUES"
#        ${ARGN}
#    )

endfunction()

function(NRHI_EnumHelper_CreateEnum)

    cmake_parse_arguments(
        PARGS
        "ENABLE_MASK_MODE"
        "NAMESPACE;NAME;DRIVER_UPPER_CASE_NAME;DRIVER_LOWER_CASE_NAME;TARGET_FILE_PATH"
        "VALUES"
        ${ARGN}
    )

endfunction()
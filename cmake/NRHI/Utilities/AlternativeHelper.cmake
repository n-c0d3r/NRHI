
include(NCPP/Utilities/SetGlobal)
include(NCPP/Utilities/ApplyGlobal)

function(NRHI_AlternativeHelper_CreateAlternative)

    cmake_parse_arguments(
        PARGS
        ""
        "NAMESPACE;NAME;MAIN_HPP_FILE_PATH;MAIN_CPP_FILE_PATH"
        "VALUES;INCLUDES;ADDITIONAL_CODE"
        ${ARGN}
    )

    string(REPLACE "\\" "/" targetHPPFilePathParsed "${PARGS_MAIN_HPP_FILE_PATH}.alternative")
    string(REPLACE "\\" "/" targetCPPFilePathParsed "${PARGS_MAIN_CPP_FILE_PATH}.alternative")



    # Prepare variables for iterating
    list(LENGTH PARGS_VALUES valueCount)

    set(step 2)

    math(EXPR modStep "${valueCount} % ${step}")



    # Check if VALUES is valid
    if(${modStep} EQUAL 0)
    else()
        message(FATAL_ERROR "invalid VALUES argument")
    endif()



    # Prepare
    set(hppFileContent "#pragma once \n #include <nrhi/prerequisites.hpp>\n")
    set(cppFileContent "#include \"${targetHPPFilePathParsed}\" \n")



    # Generate includes and additional code
    foreach(includePath ${PARGS_INCLUDES})
        set(hppFileContent
            "${hppFileContent}
            #include ${includePath}
            "
        )
    endforeach()
    foreach(code "${PARGS_ADDITIONAL_CODE}")
        set(hppFileContent
            "${hppFileContent}
            ${code}
            "
        )
    endforeach()



    # Add file headers
    set(
        hppFileContent
        "${hppFileContent}
        namespace ${PARGS_NAMESPACE}::internal {

            struct NRHI_API ALTERNATIVE_${PARGS_NAME} {
        "
    )
    set(
        cppFileContent
        "${cppFileContent}
        namespace ${PARGS_NAMESPACE}::internal {
        "
    )



    # Declare values
    math(EXPR valueCountDivStep "${valueCount} / ${step}")
    math(EXPR valueCountDivStepMinus1 "${valueCountDivStep} - 1")
    if(${valueCount})
        foreach(indexPlus1 RANGE 1 ${valueCount} ${step})
            math(EXPR nameIndex "${indexPlus1} - 1")
            math(EXPR funcTypeValueIndex "${nameIndex} + 1")

            list(GET PARGS_VALUES ${nameIndex} name)
            list(GET PARGS_VALUES ${funcTypeValueIndex} funcTypeValue)

            math(EXPR nameIndexDivStep "${nameIndex} / ${step}")

            if(${nameIndexDivStep} EQUAL ${valueCountDivStepMinus1})
                set(safeComma "")
                set(safeSemicolon "")
            else()
                set(safeComma ",")
                set(safeSemicolon ";")
            endif()

            set(
                hppFileContent
                "${hppFileContent}
                public:
                    static ncpp::TF_first_template_targ<${funcTypeValue}>* ${name};
                "
            )
        endforeach()
    endif()



    # Define values
    if(${valueCount})
        foreach(indexPlus1 RANGE 1 ${valueCount} ${step})
            math(EXPR nameIndex "${indexPlus1} - 1")
            math(EXPR funcTypeValueIndex "${nameIndex} + 1")

            list(GET PARGS_VALUES ${nameIndex} name)
            list(GET PARGS_VALUES ${funcTypeValueIndex} funcTypeValue)

            math(EXPR nameIndexDivStep "${nameIndex} / ${step}")

            set(
                cppFileContent
                "${cppFileContent}
                ncpp::TF_first_template_targ<${funcTypeValue}>* ALTERNATIVE_${PARGS_NAME}::${name} = 0;
                "
            )
        endforeach()
    endif()



    # Add file footers
    set(
        hppFileContent
        "${hppFileContent}
            };
        }
        "
    )
    set(
        cppFileContent
        "${cppFileContent}
        }
        "
    )



    # Save files
    file(WRITE "${PARGS_MAIN_HPP_FILE_PATH}.alternative" "${hppFileContent}")
    file(WRITE "${PARGS_MAIN_CPP_FILE_PATH}.alternative" "${cppFileContent}")

endfunction()
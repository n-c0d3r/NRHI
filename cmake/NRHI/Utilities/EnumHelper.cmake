
function(NRHI_EnumHelper_CreateEnum)

    cmake_parse_arguments(
        PARGS
        ""
        "NAMESPACE;NAME;DRIVER_UPPER_CASE_NAME;DRIVER_LOWER_CASE_NAME;TARGET_HPP_FILE_PATH;TARGET_CPP_FILE_PATH;TYPE"
        "VALUES"
        ${ARGN}
    )

    string(REPLACE "\\" "/" targetHPPFilePathParsed "${PARGS_TARGET_HPP_FILE_PATH}")
    string(REPLACE "\\" "/" targetCPPFilePathParsed "${PARGS_TARGET_CPP_FILE_PATH}")

    set(driverIndex ${NRHI_DRIVER_INDEX_${PARGS_DRIVER_UPPER_CASE_NAME}})

    set(TARGET_UPDATE_MAP_BODY_FILE_PATH "${targetCPPFilePathParsed}.update_map_body")
    set(TARGET_MAX_VALUE_COUNT_DIV_STEP_FILE_PATH "${targetCPPFilePathParsed}.max_value_count_div_step")



    # Prepare variables for iterating
    list(LENGTH PARGS_VALUES valueCount)

    set(step 2)

    math(EXPR modStep "${valueCount} % ${step}")



    # Check if VALUES is valid
    if(${modStep} EQUAL 0)
    else()
        message(FATAL_ERROR "invalid VALUES argument")
    endif()



    # Check for type
    if(NOT PARGS_TYPE)
        set(PARGS_TYPE "ncpp::i32")
    endif()



    # Prepare or read files
    if(${driverIndex} EQUAL 0)
        set(hppFileContent "#pragma once \n")
        set(cppFileContent "#include \"${targetHPPFilePathParsed}\" \n")
        set(updateMapBodyContent "")
        set(maxValueCountDivStep "0")
    else()
        file(READ "${PARGS_TARGET_HPP_FILE_PATH}" hppFileContent)
        file(READ "${PARGS_TARGET_CPP_FILE_PATH}" cppFileContent)
        file(READ "${TARGET_UPDATE_MAP_BODY_FILE_PATH}" updateMapBodyContent)
        file(READ "${TARGET_MAX_VALUE_COUNT_DIV_STEP_FILE_PATH}" maxValueCountDivStep)
    endif()



    # Add file headers
    if(${driverIndex} EQUAL 0)
        set(
            hppFileContent
            "${hppFileContent}
                #include <nrhi/prerequisites.hpp>

                namespace ${PARGS_NAMESPACE} {
            "
        )
        set(
            cppFileContent
            "${cppFileContent}
                namespace ${PARGS_NAMESPACE} {
            "
        )

        if(${NRHI_DRIVER_MULTIPLE})
            set(
                hppFileContent
                "${hppFileContent}
                    struct ${PARGS_NAME} {

                        ${PARGS_TYPE} value = 0;

                        NCPP_FORCE_INLINE operator ${PARGS_TYPE} () const noexcept {

                            return value;
                        }

                        static void update_map();
                "
            )
        else()
            set(
                hppFileContent
                "${hppFileContent}
                    enum class ${PARGS_NAME} : ${PARGS_TYPE} {


                "
            )
            set(
                cppFileContent
                "${cppFileContent}
                "
            )
        endif()
    endif()



    # Declare values
    math(EXPR valueCountDivStep "${valueCount} / ${step}")
    math(EXPR valueCountDivStepMinus1 "${valueCountDivStep} - 1")
    foreach(indexPlus1 RANGE 1 ${valueCount} ${step})
        math(EXPR nameIndex "${indexPlus1} - 1")
        math(EXPR driverValueIndex "${nameIndex} + 1")
        if(PARGS_ENABLE_MASK_MODE)
            math(EXPR abstractValueIndex "${driverValueIndex} + 1")
        endif()

        list(GET PARGS_VALUES ${nameIndex} name)
        list(GET PARGS_VALUES ${driverValueIndex} driverValue)
        if(PARGS_ENABLE_MASK_MODE)
            list(GET PARGS_VALUES ${abstractValueIndex} abstractValue)
        endif()

        math(EXPR nameIndexDivStep "${nameIndex} / ${step}")

        if(${nameIndexDivStep} EQUAL ${valueCountDivStepMinus1})
            set(safeComma "")
            set(safeSemicolon "")
        else()
            set(safeComma ",")
            set(safeSemicolon ";")
        endif()

        if(${NRHI_DRIVER_MULTIPLE})
            if(${nameIndexDivStep} GREATER_EQUAL ${maxValueCountDivStep})
                set(
                    hppFileContent
                    "${hppFileContent}
                    static NRHI_API ${PARGS_NAME} ${name};
                    "
                )
            endif()
        else()
            set(
                hppFileContent
                "${hppFileContent}
                    ${name} = ${driverValue} ${safeComma}
                "
            )
        endif()
    endforeach()



    # Define values
    foreach(indexPlus1 RANGE 1 ${valueCount} ${step})
        math(EXPR nameIndex "${indexPlus1} - 1")
        math(EXPR driverValueIndex "${nameIndex} + 1")
        if(PARGS_ENABLE_MASK_MODE)
            math(EXPR abstractValueIndex "${driverValueIndex} + 1")
        endif()

        list(GET PARGS_VALUES ${nameIndex} name)
        list(GET PARGS_VALUES ${driverValueIndex} driverValue)
        if(PARGS_ENABLE_MASK_MODE)
            list(GET PARGS_VALUES ${abstractValueIndex} abstractValue)
        endif()

        math(EXPR nameIndexDivStep "${nameIndex} / ${step}")

        if(${nameIndexDivStep} EQUAL ${valueCountDivStepMinus1})
            set(safeComma "")
            set(safeSemicolon "")
        else()
            set(safeComma ",")
            set(safeSemicolon ";")
        endif()

        if(${NRHI_DRIVER_MULTIPLE})
            if(${nameIndexDivStep} GREATER_EQUAL ${maxValueCountDivStep})
                set(
                    cppFileContent
                    "${cppFileContent}
                    ${PARGS_NAME} ${PARGS_NAME}::${name} = ${PARGS_NAME}{(${PARGS_TYPE})${driverValue}};
                    "
                )
            endif()
        else()
        endif()
    endforeach()



    # Update maxValueCountDivStep
    if(${valueCountDivStep} GREATER ${maxValueCountDivStep})
        set(maxValueCountDivStep ${valueCountDivStep})
    endif()



    # Define update map mode
    if(${NRHI_DRIVER_MULTIPLE})
        if(${driverIndex} EQUAL 0)
            set(
                updateMapBodyContent
                "${updateMapBodyContent}
                if(nrhi::driver_index() == ${driverIndex}) {
                "
            )
        else()
            set(
                updateMapBodyContent
                "${updateMapBodyContent}
                else if(nrhi::driver_index() == ${driverIndex}) {
                "
            )
        endif()

        foreach(indexPlus1 RANGE 1 ${valueCount} ${step})
            math(EXPR nameIndex "${indexPlus1} - 1")
            math(EXPR driverValueIndex "${nameIndex} + 1")
            if(PARGS_ENABLE_MASK_MODE)
                math(EXPR abstractValueIndex "${driverValueIndex} + 1")
            endif()

            list(GET PARGS_VALUES ${nameIndex} name)
            list(GET PARGS_VALUES ${driverValueIndex} driverValue)
            if(PARGS_ENABLE_MASK_MODE)
                list(GET PARGS_VALUES ${abstractValueIndex} abstractValue)
            endif()

            math(EXPR nameIndexDivStep "${nameIndex} / ${step}")

            if(${nameIndexDivStep} EQUAL ${valueCountDivStepMinus1})
                set(safeComma "")
                set(safeSemicolon "")
            else()
                set(safeComma ",")
                set(safeSemicolon ";")
            endif()

            if(${NRHI_DRIVER_MULTIPLE})
                set(
                    updateMapBodyContent
                    "${updateMapBodyContent}
                    *((${PARGS_NAME}*)(&(${PARGS_NAME}::${name}))) = ${PARGS_NAME}{(${PARGS_TYPE})${driverValue}};
                    "
                )
            else()
            endif()
        endforeach()

        set(
            updateMapBodyContent
            "${updateMapBodyContent}
            }
            "
        )
    endif()



    # Add file footers
    if(${driverIndex} EQUAL ${NRHI_DRIVER_LAST_INDEX})
        if(${NRHI_DRIVER_MULTIPLE})
            set(
                cppFileContent
                "${cppFileContent}
                    void ${PARGS_NAME}::update_map() {
                        ncpp::i32 driver_index = nrhi::driver_index();
                        #include \"${TARGET_UPDATE_MAP_BODY_FILE_PATH}\"
                    }
                "
            )
        else()
            set(
                cppFileContent
                "${cppFileContent}
                "
            )
        endif()

        set(
            hppFileContent
            "${hppFileContent}
                    };
            "
        )

        if(${NRHI_DRIVER_MULTIPLE})
            set(
                hppFileContent
                "${hppFileContent}
                    namespace ${PARGS_NAME}___nrhi_enum_internal {
                        inline void try_update_map(){
                            ${PARGS_NAME}::update_map();
                        }
                    }
                "
            )
        else()
            set(
                hppFileContent
                "${hppFileContent}
                    namespace ${PARGS_NAME}___nrhi_enum_internal {
                        inline void try_update_map(){
                        }
                    }
                "
            )
        endif()

        set(
            hppFileContent
            "${hppFileContent}
                }
            "
        )
        set(
            cppFileContent
            "${cppFileContent}
                }
            "
        )
    endif()



    # Save files
    file(WRITE "${PARGS_TARGET_HPP_FILE_PATH}" "${hppFileContent}")
    file(WRITE "${PARGS_TARGET_CPP_FILE_PATH}" "${cppFileContent}")
    file(WRITE "${TARGET_UPDATE_MAP_BODY_FILE_PATH}" "${updateMapBodyContent}")
    file(WRITE "${TARGET_MAX_VALUE_COUNT_DIV_STEP_FILE_PATH}" "${maxValueCountDivStep}")

endfunction()
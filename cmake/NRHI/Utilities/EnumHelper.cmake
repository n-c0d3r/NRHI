
include(NCPP/Utilities/SetGlobal)
include(NCPP/Utilities/ApplyGlobal)

function(NRHI_EnumHelper_CreateEnum)

    cmake_parse_arguments(
        PARGS
        ""
        "NAMESPACE;NAME;DRIVER_UPPER_CASE_NAME;TARGET_HPP_FILE_PATH;TARGET_CPP_FILE_PATH;TYPE"
        "VALUES;INCLUDES;ADDITIONAL_CODE"
        ${ARGN}
    )

    string(REPLACE "\\" "/" targetHPPFilePathParsed "${PARGS_TARGET_HPP_FILE_PATH}")
    string(REPLACE "\\" "/" targetCPPFilePathParsed "${PARGS_TARGET_CPP_FILE_PATH}")

    set(driverIndex ${NRHI_DRIVER_INDEX_${PARGS_DRIVER_UPPER_CASE_NAME}})

    set(TARGET_UPDATE_MAP_BODY_FILE_PATH "${targetCPPFilePathParsed}.update_map_body")
    set(TARGET_MAX_VALUE_COUNT_DIV_STEP_FILE_PATH "${targetCPPFilePathParsed}.max_value_count_div_step")
    set(TARGET_INCLUDES_FILE_PATH "${targetCPPFilePathParsed}.includes")
    set(TARGET_PREPARED_VALUE_NAMES_FILE_PATH "${targetCPPFilePathParsed}.prepared_value_names")
    set(TARGET_LAST_DRIVER_INDEX_FILE_PATH "${targetCPPFilePathParsed}.last_driver_index")
    string(REPLACE "\\" "/" TARGET_LAST_DRIVER_INDEX_FILE_PATH "${TARGET_LAST_DRIVER_INDEX_FILE_PATH}")

    if(NOT NRHI_IMPLEMENTED_${PARGS_NAME})
        NCPP_SetGlobal(NRHI_IMPLEMENTED_${PARGS_NAME} "")
        set(isFirstImplementation ON)
    endif()



    # Append enum header file
    if(isFirstImplementation)
        file(READ "${NRHI_GENERATED_FILES_DIR}/enums.hpp" headerIncludes)
        file(WRITE "${NRHI_GENERATED_FILES_DIR}/enums.hpp" "${headerIncludes} #include \"${targetHPPFilePathParsed}\" \n")

        file(READ "${NRHI_GENERATED_FILES_DIR}/enums.try_update_maps" try_update_maps)
        file(WRITE "${NRHI_GENERATED_FILES_DIR}/enums.try_update_maps" "${try_update_maps} NRHI_ENUM_TRY_UPDATE_MAP(${PARGS_NAMESPACE}::${PARGS_NAME}); \n")
    endif()



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
        set(PARGS_TYPE "ncpp::u32")
    endif()



    # Prepare or read files
    if(isFirstImplementation)
        set(hppFileContent "#pragma once \n")
        set(cppFileContent "#include \"${targetHPPFilePathParsed}\" \n")
        set(updateMapBodyContent "")
        set(maxValueCountDivStep "0")
        set(includesFileContent "#pragma once \n #include \"${TARGET_LAST_DRIVER_INDEX_FILE_PATH}\" \n")
        set(prepared_value_names "")
    else()
        file(READ "${PARGS_TARGET_HPP_FILE_PATH}" hppFileContent)
        file(READ "${PARGS_TARGET_CPP_FILE_PATH}" cppFileContent)
        file(READ "${TARGET_UPDATE_MAP_BODY_FILE_PATH}" updateMapBodyContent)
        file(READ "${TARGET_MAX_VALUE_COUNT_DIV_STEP_FILE_PATH}" maxValueCountDivStep)
        file(READ "${TARGET_INCLUDES_FILE_PATH}" includesFileContent)
        file(READ "${TARGET_PREPARED_VALUE_NAMES_FILE_PATH}" prepared_value_names)
    endif()



    # Load prepared value names from file content
    foreach(pvn ${prepared_value_names})
        set(prepared_value_names_${pvn} ON)
    endforeach()



    # Generate includes file
    foreach(includePath ${PARGS_INCLUDES})
        set(includesFileContent
            "${includesFileContent}
            #include ${includePath}
            "
        )
    endforeach()
    foreach(code "${PARGS_ADDITIONAL_CODE}")
        set(includesFileContent
            "${includesFileContent}
            ${code}
            "
        )
    endforeach()



    # Add file headers
    if(isFirstImplementation)
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

                        ${PARGS_TYPE} value___nrhi_internal___ = 0;

                        NCPP_FORCE_INLINE operator ${PARGS_TYPE} () const noexcept {

                            return value___nrhi_internal___;
                        }
                        NCPP_FORCE_INLINE operator bool () const noexcept {

                            return (value___nrhi_internal___ != 0);
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
    if(${valueCount})
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
                if(NOT prepared_value_names_${name})
                    set(prepared_value_names "${prepared_value_names};${name}")
    #           if(${nameIndexDivStep} GREATER_EQUAL ${maxValueCountDivStep})
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
    endif()



    # Define values
    if(${valueCount})
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
                if(NOT prepared_value_names_${name})
    #                set(prepared_value_names "${prepared_value_names};${name}")
    #           if(${nameIndexDivStep} GREATER_EQUAL ${maxValueCountDivStep})
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
    endif()



    # Update maxValueCountDivStep
    if(${valueCountDivStep} GREATER ${maxValueCountDivStep})
        set(maxValueCountDivStep ${valueCountDivStep})
    endif()



    # Define update map mode
    if(${NRHI_DRIVER_MULTIPLE})
        if(isFirstImplementation)
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
#    if(${driverIndex} EQUAL ${NRHI_DRIVER_LAST_INDEX})
        set(
            hppFileContent
            "${hppFileContent}
                #if NRHI_IMPLEMENTATION_LAST_DRIVER_INDEX_${PARGS_NAME} == ${driverIndex}
            "
        )
        set(
            cppFileContent
            "${cppFileContent}
                #if NRHI_IMPLEMENTATION_LAST_DRIVER_INDEX_${PARGS_NAME} == ${driverIndex}
            "
        )

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
                #endif
            "
        )
        set(
            cppFileContent
            "${cppFileContent}
                }
                #endif
            "
        )
#    endif()



    # Save files
    file(WRITE "${PARGS_TARGET_HPP_FILE_PATH}" "${hppFileContent}")
    file(WRITE "${PARGS_TARGET_CPP_FILE_PATH}" "${cppFileContent}")
    file(WRITE "${TARGET_UPDATE_MAP_BODY_FILE_PATH}" "${updateMapBodyContent}")
    file(WRITE "${TARGET_MAX_VALUE_COUNT_DIV_STEP_FILE_PATH}" "${maxValueCountDivStep}")
    file(WRITE "${TARGET_INCLUDES_FILE_PATH}" "${includesFileContent}")
    file(WRITE "${TARGET_PREPARED_VALUE_NAMES_FILE_PATH}" "${prepared_value_names}")
    file(WRITE "${TARGET_LAST_DRIVER_INDEX_FILE_PATH}" "#define NRHI_IMPLEMENTATION_LAST_DRIVER_INDEX_${PARGS_NAME} ${driverIndex}")

endfunction()
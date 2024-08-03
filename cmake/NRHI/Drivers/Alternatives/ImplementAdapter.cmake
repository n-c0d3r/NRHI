
message(STATUS "<NRHI::Drivers::Alternatives> Start implementing adapter")

include(NRHI/Utilities/AlternativeHelper)



NCPP_SetGlobal(NRHI_ADAPTER_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/adapter.hpp")
NCPP_SetGlobal(NRHI_ADAPTER_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/adapter.cpp")



NRHI_AlternativeHelper_CreateAlternative(
    NAMESPACE "nrhi"
    NAME "H_adapter"
    MAIN_HPP_FILE_PATH "${NRHI_ADAPTER_HPP_FILE_PATH}"
    MAIN_CPP_FILE_PATH "${NRHI_ADAPTER_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/adapter_base.hpp>"
    VALUES
        "adapter_p_vector" "const TG_vector<TK_valid<A_adapter>>&()"
)

message(STATUS "<NRHI::Drivers::Alternatives> Implement adapter done")
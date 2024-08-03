
message(STATUS "<NRHI::Drivers::Alternatives> Start implementing sampler state")

include(NRHI/Utilities/AlternativeHelper)



NCPP_SetGlobal(NRHI_SAMPLER_STATE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/sampler_state.hpp")
NCPP_SetGlobal(NRHI_SAMPLER_STATE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/sampler_state.cpp")



NRHI_AlternativeHelper_CreateAlternative(
    NAMESPACE "nrhi"
    NAME "H_sampler_state"
    MAIN_HPP_FILE_PATH "${NRHI_SAMPLER_STATE_HPP_FILE_PATH}"
    MAIN_CPP_FILE_PATH "${NRHI_SAMPLER_STATE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/sampler_state_base.hpp>"
    ADDITIONAL_CODE
        ""
    VALUES
        "create" "TU<A_sampler_state>(
            TKPA_valid<A_device> device_p,
            const F_sampler_state_desc& desc
        )"
)

message(STATUS "<NRHI::Drivers::Alternatives> Implement sampler state done")
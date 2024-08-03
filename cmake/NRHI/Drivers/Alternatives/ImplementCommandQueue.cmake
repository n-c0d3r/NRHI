
message(STATUS "<NRHI::Drivers::Alternatives> Start implementing command_queue")

include(NRHI/Utilities/AlternativeHelper)



NCPP_SetGlobal(NRHI_COMMAND_QUEUE_HPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_queue.hpp")
NCPP_SetGlobal(NRHI_COMMAND_QUEUE_CPP_FILE_PATH "${NRHI_GENERATED_FILES_DIR}/command_queue.cpp")



NRHI_AlternativeHelper_CreateAlternative(
    NAMESPACE "nrhi"
    NAME "H_command_queue"
    MAIN_HPP_FILE_PATH "${NRHI_COMMAND_QUEUE_HPP_FILE_PATH}"
    MAIN_CPP_FILE_PATH "${NRHI_COMMAND_QUEUE_CPP_FILE_PATH}"
    INCLUDES
        "<nrhi/command_queue_base.hpp>"
    ADDITIONAL_CODE
        "namespace nrhi {
            class A_device;
        }"
    VALUES
        "create" "TU<A_command_queue>(TKPA_valid<A_device> device_p, const F_command_queue_desc& desc)"
        "execute_command_lists" "void(
            TKPA_valid<A_command_queue> command_queue_p,
            TG_span<TK_valid<A_command_list>> command_list_p_span
        )"
        "execute_command_list" "void(
			TKPA_valid<A_command_queue> command_queue_p,
			TKPA_valid<A_command_list> command_list_p
		)"
)

message(STATUS "<NRHI::Drivers::Alternatives> Implement command_queue done")
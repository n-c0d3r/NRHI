
#include <nrhi/.hpp>

using namespace nrhi;



NCPP_ENTRY_POINT() {

    nrhi::initialize_system();
    NCPP_DEFER(nrhi::release_system());

    const auto& adapter_p_vector = H_adapter::adapter_p_vector();
    NCPP_ASSERT(adapter_p_vector.size());

    auto device_p = H_device::create(adapter_p_vector[0]);

    auto command_list_type = E_command_list_type::DIRECT;

    auto command_queue_p = H_command_queue::create(
        NCPP_FOREF_VALID(device_p),
        F_command_queue_desc {
            command_list_type
        }
    );
    auto command_list_p = H_command_list::create(
        NCPP_FOREF_VALID(device_p),
        F_command_list_desc {
            command_list_type
        }
    );

    H_command_queue::execute_command_lists(
        NCPP_FOREF_VALID(command_queue_p),
        NCPP_INIL_SPAN(
            NCPP_FOREF_VALID(command_list_p)
        )
    );

    NCPP_INFO()
        << "Hello "
        << E_log_color::V_FOREGROUND_BRIGHT_MAGNETA
        << "NRHI "
        << E_log_color::V_FOREGROUND_BRIGHT_BLUE
        << NRHI_VERSION_STR
        << E_log_color::RESET;

    ncpp::pause_console();

	return 0;
}

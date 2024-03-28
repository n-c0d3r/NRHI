
#include <nrhi/.hpp>

using namespace nrhi;



NCPP_ENTRY_POINT() {

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

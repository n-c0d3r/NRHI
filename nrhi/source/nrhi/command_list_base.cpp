#include <nrhi/command_list_base.hpp>



namespace nrhi {

    A_command_list::A_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc) :
        A_device_child(device_p),
        desc_(desc),
		supports_graphics_(
			(desc.type == ED_command_list_type::DIRECT)
		),
		supports_compute_(
			(desc.type == ED_command_list_type::DIRECT)
			|| (desc.type == ED_command_list_type::COMPUTE)
		),
		supports_blit_(
			(desc.type == ED_command_list_type::DIRECT)
			|| (desc.type == ED_command_list_type::BLIT)
		)
    {

    }
    A_command_list::~A_command_list(){

    }

}
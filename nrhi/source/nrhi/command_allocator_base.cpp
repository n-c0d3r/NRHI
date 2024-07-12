#include <nrhi/command_allocator_base.hpp>



namespace nrhi {

	A_command_allocator::A_command_allocator(TKPA_valid<A_device> device_p, const F_command_allocator_desc& desc) :
		device_p_(device_p),
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
	A_command_allocator::~A_command_allocator(){

	}

}
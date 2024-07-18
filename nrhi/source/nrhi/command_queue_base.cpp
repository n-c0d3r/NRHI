#include <nrhi/command_queue_base.hpp>
#include <nrhi/command_list_base.hpp>



namespace nrhi {

    A_command_queue::A_command_queue(TKPA_valid<A_device> device_p, const F_command_queue_desc& desc) :
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
    A_command_queue::~A_command_queue(){
    }



	b8 A_command_queue::is_compatible(TKPA_valid<A_command_list> command_list_p) const {

		b8 is_command_list_for_graphics = command_list_p->supports_graphics();
		b8 is_command_list_for_compute = command_list_p->supports_compute();
		b8 is_command_list_for_blit = command_list_p->supports_blit();

		return (
			(
				supports_graphics_
				|| (!supports_graphics_ && !is_command_list_for_graphics)
			)
			&& (
				supports_compute_
				|| (!supports_compute_ && !is_command_list_for_compute)
			)
			&& (
				supports_blit_
				|| (!supports_blit_ && !is_command_list_for_blit)
			)
		);
	}

}
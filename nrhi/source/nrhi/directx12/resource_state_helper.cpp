#include <nrhi/directx12/resource_state_helper.hpp>



namespace nrhi
{
	b8 HD_directx12_resource_state::is_writable(ED_resource_state resource_state)
	{
		if(flag_is_has(resource_state, ED_resource_state::RENDER_TARGET))
			return true;
		if(flag_is_has(resource_state, ED_resource_state::UNORDERED_ACCESS))
			return true;
		if(flag_is_has(resource_state, ED_resource_state::DEPTH_WRITE))
			return true;
		if(flag_is_has(resource_state, ED_resource_state::COPY_DEST))
			return true;
		if(flag_is_has(resource_state, ED_resource_state::VIDEO_DECODE_WRITE))
			return true;
		if(flag_is_has(resource_state, ED_resource_state::VIDEO_PROCESS_WRITE))
			return true;
		if(flag_is_has(resource_state, ED_resource_state::VIDEO_ENCODE_WRITE))
			return true;

		return false;
	}
}
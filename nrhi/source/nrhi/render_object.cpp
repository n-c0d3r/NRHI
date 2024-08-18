#include <nrhi/render_object.hpp>



namespace nrhi
{
#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
    void A_render_object::set_debug_name(const F_debug_name& value)
    {
        debug_name_ = value;
    }
#endif



    A_render_object::A_render_object()
    {
    }
    A_render_object::~A_render_object()
    {
    }
}
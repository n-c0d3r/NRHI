#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/state_object_flag.hpp>



namespace nrhi
{
    struct F_state_object_config
    {
        ED_state_object_flag flags = ED_state_object_flag::NONE;
    };
}
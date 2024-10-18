#pragma once

#include <nrhi/prerequisites.hpp>



namespace nrhi
{
    struct F_gpu_memory_info
    {
        u64 budget = 0;
        u64 current_usage = 0;
    };
}
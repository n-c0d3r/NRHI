#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/work_graph_flag.hpp>



namespace nrhi
{
    struct F_work_graph_desc
    {
        ED_work_graph_flag flags = ED_work_graph_flag::NONE;
    };
}
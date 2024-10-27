#pragma once

#include <nrhi/device_child.hpp>



namespace nrhi
{
    class NRHI_API A_state_object : public A_device_child
    {
    protected:
        A_state_object(TKPA_valid<A_device> device_p);

    public:
        virtual ~A_state_object() override = default;
    };
}
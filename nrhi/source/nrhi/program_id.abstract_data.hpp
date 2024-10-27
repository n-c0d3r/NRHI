#pragma once

#include <nrhi/prerequisites.hpp>
#include <nrhi/utilities/abstract_data.hpp>
#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
#include <nrhi/program_id.hpp>
#endif



#ifdef NRHI_DRIVER_SUPPORT_STATE_OBJECT
namespace nrhi
{
#ifdef NRHI_DRIVER_DIRECTX_12
    #define NRHI_DRIVER_DIRECTX_12_PROGRAM_ID_TYPE D3D12_PROGRAM_IDENTIFIER
#else
    #define NRHI_DRIVER_DIRECTX_12_PROGRAM_ID_TYPE u8
#endif



    NRHI_DEFINE_ABSTRACT_DATA(
        F_program_id,
        H_program_id,
        NRHI_DRIVER_DIRECTX_12_PROGRAM_ID_TYPE
    );
}
#endif
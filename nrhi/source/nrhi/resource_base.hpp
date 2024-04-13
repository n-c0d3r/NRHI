#pragma once

/** @file nrhi/resource_base.hpp
*
*   Implement resource base class.
*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#pragma region Includes

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/prerequisites.hpp>

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#include <nrhi/format.hpp>
#include <nrhi/resource_heap_type.hpp>
#include <nrhi/resource_bind_flag.hpp>
#include <nrhi/resource_misc_flag.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_resource;



    struct F_sample_desc {

        u32 count = 1;
        u32 quality = 0;

    };

    struct F_initial_resource_data {

        void* system_mem_p = 0;
        u32 system_mem_pitch = 0;
        u32 system_mem_slice_pitch = 0;

    };

    struct F_resource_desc {

        u32 width;
        u32 height;

        u32 mip_level_count = 1;

        union {
            u32 array_size = 1;
            u32 depth;
        };

        union {
            E_format format;
            u32 stride;
        };

        F_sample_desc sample_desc;

        E_resource_heap_type heap_type = E_resource_heap_type::GREAD_GWRITE;
        E_resource_bind_flag bind_flags = E_resource_bind_flag::NONE;
        E_resource_misc_flag misc_flags = E_resource_misc_flag::NONE;

    };



    struct F_vertex_buffer_view {

        struct F_diver_non_advanced {

            TK<A_resource> managed_resource_p;
            u32 stride;
            u32 offset;

        };

        struct F_driver_advanced {

            u64 gpu_virtual_address;
            u32 stride;
            u32 size_in_bytes;

        };

        union {

            F_driver_non_advanced driver_non_advanced;
            F_driver_advanced driver_advanced;

        };

    };

    struct F_index_buffer_view {

        struct F_driver_non_advanced {

            TK<A_resource> managed_resource_p;
            E_format format;
            u32 offset;

        };

        struct F_driver_advanced {

            u64 gpu_virtual_address;
            E_format format;
            u32 size_in_bytes;

        };

        union {

            F_driver_non_advanced driver_non_advanced;
            F_driver_advanced driver_advanced;

        };

    };



    class NRHI_API A_resource {

    private:
        TK_valid<A_device> device_p_;
        F_resource_desc desc_;

    public:
        NCPP_FORCE_INLINE TK_valid<A_device> device_p() noexcept { return device_p_; }
        NCPP_FORCE_INLINE const F_resource_desc& desc() const noexcept { return desc_; }



    protected:
        A_resource(TK_valid<A_device> device_p, const F_resource_desc& desc);

    public:
        virtual ~A_resource();

    };

}

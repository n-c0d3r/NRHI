#pragma once

/** @file nrhi/directx11/resource.hpp
*
*   Implement directx11 resource.
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

#include <nrhi/resource_base.hpp>
#include <nrhi/buffer_handle.hpp>
#include <nrhi/texture_handle.hpp>

#pragma endregion



namespace nrhi {

    class A_device;



    class NRHI_API F_directx11_resource : public A_resource {

    protected:
        ID3D11Resource* d3d11_resource_p_ = 0;

    public:
        NCPP_FORCE_INLINE ID3D11Resource* d3d11_resource_p() noexcept { return d3d11_resource_p_; }
        NCPP_FORCE_INLINE void set_d3d11_resource_p_unsafe(ID3D11Resource* value) noexcept {
            d3d11_resource_p_ = value;
        }

    public:
        F_directx11_resource(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type
        );
        F_directx11_resource(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type,
            ID3D11Resource* d3d11_resource_p
        );
        virtual ~F_directx11_resource();

    };



    class NRHI_API HD_directx11_resource {

    public:
        static TU<A_resource> create(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );

    public:
        static U_buffer_handle create_buffer(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );

    public:
        static U_texture_1d_handle create_texture_1d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );
        static U_texture_2d_handle create_texture_2d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );
        static U_texture_3d_handle create_texture_3d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_resource_data,
            const F_resource_desc& desc
        );
		static U_texture_2d_array_handle create_texture_2d_array(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_resource_data,
			const F_resource_desc& desc
		);

	public:
		static F_mapped_subresource map(
			TKPA_valid<A_resource> resource_p,
			u32 subresource_index
		);
		static void unmap(
			TKPA_valid<A_resource> resource_p,
			u32 subresource_index
		);

    public:
        static sz first_pitch(u32 element_stride, u32 count);
        static sz second_pitch(sz first_pitch, u32 count);
        static sz third_pitch(sz second_pitch, u32 count);

    public:
        static u32 subresource_index(
            u32 array_slice,
            u32 mip_level,
            u32 mip_level_count
        );
        static u32 subresource_index_with_plane_slice(
            u32 array_slice,
            u32 mip_level,
            u32 mip_level_count,
            u32 array_size,
            u32 plane_slice
        );

    public:
        static F_vector3_u32 mip_divisor(
            ED_resource_type type
        );

    };

}

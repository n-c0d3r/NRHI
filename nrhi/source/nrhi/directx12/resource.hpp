#pragma once

/** @file nrhi/directx12/resource.hpp
*
*   Implement directx12 resource.
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
#include <nrhi/buffer.hpp>
#include <nrhi/texture.hpp>

#pragma endregion



namespace nrhi {

	class NRHI_API F_directx12_resource : public A_resource {

	protected:
		ID3D12Resource* d3d12_resource_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12Resource* d3d12_resource_p() noexcept { return d3d12_resource_p_; }
		NCPP_FORCE_INLINE void set_d3d12_resource_p_unsafe(ID3D12Resource* value) noexcept {
			d3d12_resource_p_ = value;
		}

	public:
		F_directx12_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			ED_resource_type overrided_type
		);
		F_directx12_resource(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			ED_resource_type overrided_type,
			ID3D12Resource* d3d12_resource_p
		);
		virtual ~F_directx12_resource();

	};



	class NRHI_API HD_directx12_resource {

	public:
		static TU<A_resource> create_committed(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);

	public:
		static U_buffer_handle create_committed_buffer(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);

	public:
		static U_texture_1d_handle create_committed_texture_1d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);
		static U_texture_2d_handle create_committed_texture_2d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);
		static U_texture_3d_handle create_committed_texture_3d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);
		static U_texture_2d_array_handle create_committed_texture_2d_array(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);

	public:
		static TU<A_resource> create_placed(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset
		);

	public:
		static U_buffer_handle create_placed_buffer(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset
		);

	public:
		static U_texture_1d_handle create_placed_texture_1d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset
		);
		static U_texture_2d_handle create_placed_texture_2d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset
		);
		static U_texture_3d_handle create_placed_texture_3d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset
		);
		static U_texture_2d_array_handle create_placed_texture_2d_array(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc,
			TKPA_valid<A_resource_heap> heap_p,
			u64 heap_offset
		);

	public:
		static TU<A_resource> create_reserved(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);

	public:
		static U_buffer_handle create_reserved_buffer(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);

	public:
		static U_texture_1d_handle create_reserved_texture_1d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);
		static U_texture_2d_handle create_reserved_texture_2d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);
		static U_texture_3d_handle create_reserved_texture_3d(
			TKPA_valid<A_device> device_p,
			const F_resource_desc& desc
		);
		static U_texture_2d_array_handle create_reserved_texture_2d_array(
			TKPA_valid<A_device> device_p,
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
		static F_resource_gpu_virtual_address gpu_virtual_address(
			TKPA_valid<A_resource> resource_p
		);



#pragma region Alternative Functions
#ifdef NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
	public:
		static TU<A_resource> create(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& inital_data,
			const F_resource_desc& desc
		);

	public:
		static U_buffer_handle create_buffer(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& inital_data,
			const F_resource_desc& desc
		);

	public:
		static U_texture_1d_handle create_texture_1d(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& inital_data,
			const F_resource_desc& desc
		);
		static U_texture_2d_handle create_texture_2d(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& inital_data,
			const F_resource_desc& desc
		);
		static U_texture_3d_handle create_texture_3d(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& inital_data,
			const F_resource_desc& desc
		);
		static U_texture_2d_array_handle create_texture_2d_array(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& inital_data,
			const F_resource_desc& desc
		);
#endif // NRHI_DRIVER_ENABLE_INTERFACE_ONLY_SUPPORTS
#pragma endregion

	};

}

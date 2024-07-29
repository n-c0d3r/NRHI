#pragma once

/** @file nrhi/directx12/command_list.hpp
*
*   Implement directx12 command_list.
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

#include <nrhi/command_list_base.hpp>
#include <nrhi/pipeline_state_base.hpp>
#include <nrhi/graphics_pipeline_state.hpp>
#include <nrhi/compute_pipeline_state.hpp>
#include <nrhi/buffer_handle.hpp>
#include <nrhi/clear_flag.hpp>
#include <nrhi/resource_view_handle.hpp>
#include <nrhi/resource_barrier.hpp>

#pragma endregion



namespace nrhi {

    class A_device;



	struct F_directx12_command_list_create_with_command_allocator_flag {};



    class NRHI_API F_directx12_command_list : public A_command_list {

	public:
		friend class HD_directx12_command_list;



	private:
		ID3D12GraphicsCommandList* d3d12_command_list_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12GraphicsCommandList* d3d12_command_list_p() const noexcept { return d3d12_command_list_p_; }
		NCPP_FORCE_INLINE void set_d3d12_command_list_p_unsafe(ID3D12GraphicsCommandList* value) noexcept { d3d12_command_list_p_ = value; }



    public:
        F_directx12_command_list(
			TKPA_valid<A_device> device_p,
			const F_command_list_desc& desc,
			F_directx12_command_list_create_with_command_allocator_flag
		);
        ~F_directx12_command_list();

    };



    class NRHI_API HD_directx12_command_list {

    public:
        static TU<A_command_list> create_with_command_allocator(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);

	public:
		static void async_begin(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_command_allocator> command_allocator_p
		);
		static void async_end(TKPA_valid<A_command_list> command_list_p);

	public:
		static void async_resource_barrier(
			TKPA_valid<A_command_list> command_list_p,
			const F_resource_barrier& resource_barrier
		);
		static void async_resource_barriers(
			TKPA_valid<A_command_list> command_list_p,
			const TG_span<F_resource_barrier>& resource_barriers
		);

	public:
		static void async_clear_state(
			TKPA_valid<A_command_list> command_list_p
		);
		static void async_clear_rtv(
			TKPA_valid<A_command_list> command_list_p,
			K_valid_rtv_handle rtv_p,
			PA_vector4_f32 color
		);
		static void async_clear_dsv(
			TKPA_valid<A_command_list> command_list_p,
			K_valid_dsv_handle dsv_p,
			ED_clear_flag flag,
			f32 depth,
			u8 stencil
		);
		static void async_clear_rtv_with_descriptor(
			TKPA_valid<A_command_list> command_list_p,
			F_descriptor_cpu_address rtv_cpu_address,
			PA_vector4_f32 color
		);
		static void async_clear_dsv_with_descriptor(
			TKPA_valid<A_command_list> command_list_p,
			F_descriptor_cpu_address dsv_cpu_address,
			ED_clear_flag flag,
			f32 depth,
			u8 stencil
		);

	public:
		static void async_copy_resource(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_resource> dst_resource_p,
			TKPA_valid<A_resource> src_resource_p
		);
		static void async_copy_buffer_region(
			TKPA_valid<A_command_list> command_list_p,
			TKPA_valid<A_resource> dst_resource_p,
			TKPA_valid<A_resource> src_resource_p,
			u64 dst_offset,
			u64 src_offset,
			u64 size
		);

    };

}

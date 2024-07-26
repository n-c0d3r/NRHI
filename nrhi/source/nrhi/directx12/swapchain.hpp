#pragma once

/** @file nrhi/directx12/swapchain.hpp
*
*   Implement directx12 swapchain.
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

#include <nrhi/swapchain_base.hpp>
#include <nrhi/texture.hpp>
#include <nrhi/resource_view_base.hpp>

#pragma endregion



namespace nrhi {

	class A_command_queue;
	class A_command_list;
	class A_resource;
	class A_resource_view;
	class A_descriptor_heap;



	class NRHI_API F_directx12_swapchain : public A_swapchain {

	public:
		friend class HD_directx12_swapchain;



	private:
		IDXGISwapChain3* dxgi_swapchain_p_ = 0;
		typename F_event::F_listener_handle surface_resize_handle_;

		TG_vector<U_texture_2d_handle> buffer_p_vector_;

		TU<A_descriptor_heap> rtv_descriptor_heap_p_;
		TG_vector<U_rtv_handle> rtv_p_vector_;
		U_rtv_handle back_rtv_p_;
		U_texture_2d_handle back_buffer_p_;

	public:
		NCPP_FORCE_INLINE IDXGISwapChain3* dxgi_swapchain_p() noexcept { return dxgi_swapchain_p_; }
		NCPP_FORCE_INLINE void set_dxgi_swapchain_p_unsafe(IDXGISwapChain3* value) noexcept { dxgi_swapchain_p_ = value; }



	public:
		F_directx12_swapchain(
			TKPA_valid<A_command_queue> command_queue_p,
			TKPA_valid<F_surface> surface_p,
			const F_swapchain_desc& desc
		);
		~F_directx12_swapchain();

	private:
		void update_d3d12_object_for_buffer_rtvs();

	};



	class NRHI_API HD_directx12_swapchain {

	public:
		static TU<A_swapchain> create(
			TKPA_valid<A_command_queue> command_queue_p,
			TKPA_valid<F_surface> surface_p,
			const F_swapchain_desc& desc
		);

	public:
		static u8 current_rtv_index(TKPA_valid<A_swapchain>);
		static K_valid_rtv_handle rtv_p(TKPA_valid<A_swapchain>, u8 index);
		static void update_back_rtv(TKPA_valid<A_swapchain>);
		static K_valid_rtv_handle back_rtv_p(TKPA_valid<A_swapchain>);
		static K_valid_texture_2d_handle back_buffer_p(TKPA_valid<A_swapchain>);

	public:
		static void async_present(TKPA_valid<A_swapchain>);

	};

}
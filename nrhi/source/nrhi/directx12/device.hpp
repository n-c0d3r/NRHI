#pragma once

/** @file nrhi/directx12/device.hpp
*
*   Implement directx12 device.
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

#include <nrhi/device_base.hpp>
#include <nrhi/resource_heap_tier.hpp>

#pragma endregion



namespace nrhi {

    class A_adapter;



    class NRHI_API F_directx12_device : public A_device {

	private:
		ID3D12Device* d3d12_device_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12Device* d3d12_device_p() noexcept { return d3d12_device_p_; }
    	NCPP_FORCE_INLINE void set_d3d12_device_p_unsafe(ID3D12Device* value) noexcept { d3d12_device_p_ = value; }

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
    	virtual void set_debug_name(const F_debug_name& value) override;
#endif



    public:
        F_directx12_device(TK_valid<A_adapter> adapter_p);
        ~F_directx12_device();

    };



    class NRHI_API HD_directx12_device {

    public:
        static TU<A_device> create(TKPA_valid<A_adapter> adapter_p);

	public:
		static u64 descriptor_increment_size(
			TKPA_valid<A_device> device_p,
			ED_descriptor_heap_type descriptor_heap_type
		);

	public:
		static ED_resource_heap_tier resource_heap_tier(
			TKPA_valid<A_device> device_p
		);
		static ED_mesh_shader_tier mesh_shader_tier(
			TKPA_valid<A_device> device_p
		);
    	static ED_work_graphs_tier work_graphs_tier(
			TKPA_valid<A_device> device_p
		);

    public:
    	static eastl::pair<u32, u32> hlsl_highest_shader_model(
    		TKPA_valid<A_device> device_p
    	);

    public:
    	static u32 min_wave_size(
    		TKPA_valid<A_device> device_p
    	);
    	static u32 max_wave_size(
			TKPA_valid<A_device> device_p
		);
    	static u32 total_lane_count(
			TKPA_valid<A_device> device_p
		);
    };
}

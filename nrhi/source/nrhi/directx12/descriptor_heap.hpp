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

#include <nrhi/descriptor_heap_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx12_descriptor_heap : public A_descriptor_heap {

	protected:
		ID3D12DescriptorHeap* d3d12_descriptor_heap_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12DescriptorHeap* d3d12_descriptor_heap_p() noexcept { return d3d12_descriptor_heap_p_; }
		NCPP_FORCE_INLINE void set_d3d12_descriptor_heap_p_unsafe(ID3D12DescriptorHeap* value) noexcept {
			d3d12_descriptor_heap_p_ = value;
		}

	public:
		F_directx12_descriptor_heap(
			TKPA_valid<A_device> device_p,
			const F_descriptor_heap_desc& desc
		);
		F_directx12_descriptor_heap(
			TKPA_valid<A_device> device_p,
			const F_descriptor_heap_desc& desc,
			ID3D12DescriptorHeap* d3d12_descriptor_heap_p
		);
		virtual ~F_directx12_descriptor_heap();

	};



	class NRHI_API HD_directx12_descriptor_heap {

	public:
		static TU<A_descriptor_heap> create(
			TKPA_valid<A_device> device_p,
			const F_descriptor_heap_desc& desc
		);

	};

}

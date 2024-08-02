#pragma once

/** @file nrhi/directx12/resource_heap.hpp
*
*   Implement directx12 resource_heap.
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

#include <nrhi/resource_heap_base.hpp>
#include <nrhi/buffer.hpp>
#include <nrhi/texture.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx12_resource_heap : public A_resource_heap {

	protected:
		ID3D12Heap* d3d12_resource_heap_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D12Heap* d3d12_resource_heap_p() noexcept { return d3d12_resource_heap_p_; }
		NCPP_FORCE_INLINE void set_d3d12_resource_heap_p_unsafe(ID3D12Heap* value) noexcept {
			d3d12_resource_heap_p_ = value;
		}

	public:
		F_directx12_resource_heap(
			TKPA_valid<A_device> device_p,
			const F_resource_heap_desc& desc
		);
		F_directx12_resource_heap(
			TKPA_valid<A_device> device_p,
			const F_resource_heap_desc& desc,
			ID3D12Heap* d3d12_resource_heap_p
		);
		virtual ~F_directx12_resource_heap();

	private:
		static ID3D12Heap* create_d3d12_resource_heap(
			TKPA_valid<A_device> device_p,
			const F_resource_heap_desc& desc
		);

	public:
		virtual void rebuild(
			const F_resource_heap_desc& desc
		) override;

	};



	class NRHI_API HD_directx12_resource_heap {

	public:
		static TU<A_resource_heap> create(
			TKPA_valid<A_device> device_p,
			const F_resource_heap_desc& desc
		);

	};

}

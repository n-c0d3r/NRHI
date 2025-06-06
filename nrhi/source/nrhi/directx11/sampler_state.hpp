#pragma once

/** @file nrhi/directx11/sampler_state.hpp
*
*   Implement directx11 sampler state.
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

#include <nrhi/sampler_state_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx11_sampler_state : public A_sampler_state {

	private:
		ID3D11SamplerState* d3d11_sampler_state_p_ = 0;

	public:
		NCPP_FORCE_INLINE ID3D11SamplerState* d3d11_sampler_state_p() noexcept { return d3d11_sampler_state_p_; }
		NCPP_FORCE_INLINE void set_d3d11_sampler_state_p_unsafe(ID3D11SamplerState* value) noexcept { d3d11_sampler_state_p_ = value; }

#ifdef NRHI_ENABLE_DRIVER_DEBUGGER
        virtual void set_debug_name(const F_debug_name& value) override;
#endif



	public:
		F_directx11_sampler_state(
			TKPA_valid<A_device> device_p,
			const F_sampler_state_desc& desc
		);
		F_directx11_sampler_state(
			TKPA_valid<A_device> device_p,
			const F_sampler_state_desc& desc,
			ID3D11SamplerState* d3d11_sampler_state_p
		);
		virtual ~F_directx11_sampler_state();

	public:
		NCPP_OBJECT(F_directx11_sampler_state);



	private:
		static ID3D11SamplerState* create_d3d11_sampler_state(
			TKPA_valid<A_device> device_p,
			const F_sampler_state_desc& desc
		);

	public:
		virtual void rebuild(
			const F_sampler_state_desc& desc
		) override;

	public:
		virtual void release_driver_specific_implementation() override;

	};



	class NRHI_API HD_directx11_sampler_state {

	public:
		static TU<A_sampler_state> create(
			TKPA_valid<A_device> device_p,
			const F_sampler_state_desc& desc
		);

	};

}

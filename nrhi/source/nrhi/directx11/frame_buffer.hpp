#pragma once

/** @file nrhi/directx11/frame_buffer.hpp
*
*   Implement directx11 frame_buffer.
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

#include <nrhi/frame_buffer_base.hpp>

#pragma endregion



namespace nrhi {

	class A_device;



	class NRHI_API F_directx11_frame_buffer : public A_frame_buffer {

	public:
		friend class HD_directx11_frame_buffer;



	public:
		F_directx11_frame_buffer(
			TKPA_valid<A_device> device_p,
			const F_frame_buffer_desc& desc
		);
		virtual ~F_directx11_frame_buffer();

	public:
		virtual void rebuild() override;

	};



	class NRHI_API HD_directx11_frame_buffer {

	public:
		static TU<A_frame_buffer> create(
			TKPA_valid<A_device> device_p,
			const F_frame_buffer_desc& desc
		);
	};

}

#pragma once

/** @file nrhi/directx11/buffer.hpp
*
*   Implement directx11 buffer.
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

#include <nrhi/directx11/resource.hpp>

#pragma endregion



namespace nrhi {

    class A_device;



    class NRHI_API F_directx11_buffer : public F_directx11_resource {

    public:
        F_directx11_buffer(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            E_resource_type overrided_type = E_resource_type::BUFFER
        );
        F_directx11_buffer(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            E_resource_type overrided_type,
            ID3D11Buffer* d3d11_buffer_p
        );
        virtual ~F_directx11_buffer();

    private:
        static ID3D11Buffer* create_d3d11_buffer(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc
        );

	public:
		virtual void rebuild(
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc
		) override;

    };



    class NRHI_API F_directx11_structured_buffer : public F_directx11_buffer {

    public:
        F_directx11_structured_buffer(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            E_resource_type overrided_type = E_resource_type::STRUCTURED_BUFFER
        );
        F_directx11_structured_buffer(
			TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            E_resource_type overrided_type,
            ID3D11Buffer* d3d11_buffer_p
        );
        virtual ~F_directx11_structured_buffer();

    private:
        static ID3D11Buffer* create_d3d11_structured_buffer(
			TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc
        );

	public:
		virtual void rebuild(
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc
		) override;

    };



	class NRHI_API F_directx11_indirect_buffer : public F_directx11_buffer {

	public:
		F_directx11_indirect_buffer(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc,
			E_resource_type overrided_type = E_resource_type::INDIRECT_BUFFER
		);
		F_directx11_indirect_buffer(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc,
			E_resource_type overrided_type,
			ID3D11Buffer* d3d11_buffer_p
		);
		virtual ~F_directx11_indirect_buffer();

	private:
		static ID3D11Buffer* create_d3d11_indirect_buffer(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc
		);

	public:
		virtual void rebuild(
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc
		) override;

	};

}

#pragma once

/** @file nrhi/directx11/texture.hpp
*
*   Implement directx11 texture.
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



    class NRHI_API F_directx11_texture_1d : public F_directx11_resource {

    public:
        F_directx11_texture_1d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type = ED_resource_type::TEXTURE_1D
        );
        F_directx11_texture_1d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type,
            ID3D11Texture1D* d3d11_texture_1d_p
        );
        virtual ~F_directx11_texture_1d();

    private:
        static ID3D11Texture1D* create_d3d11_texture_1d(
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



    class NRHI_API F_directx11_texture_2d : public F_directx11_resource {

    public:
        F_directx11_texture_2d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type = ED_resource_type::TEXTURE_2D
        );
        F_directx11_texture_2d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type,
            ID3D11Texture2D* d3d11_texture_2d_p
        );
        virtual ~F_directx11_texture_2d();

    private:
        static ID3D11Texture2D* create_d3d11_texture_2d(
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



    class NRHI_API F_directx11_texture_3d : public F_directx11_resource {

    public:
        F_directx11_texture_3d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type = ED_resource_type::TEXTURE_3D
        );
        F_directx11_texture_3d(
            TKPA_valid<A_device> device_p,
            const F_initial_resource_data& initial_data,
            const F_resource_desc& desc,
            ED_resource_type overrided_type,
            ID3D11Texture3D* d3d11_texture_3d_p
        );
        virtual ~F_directx11_texture_3d();

    private:
        static ID3D11Texture3D* create_d3d11_texture_3d(
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



	class NRHI_API F_directx11_texture_2d_array : public F_directx11_resource {

	public:
		F_directx11_texture_2d_array(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc,
			ED_resource_type overrided_type = ED_resource_type::TEXTURE_2D_ARRAY
		);
		F_directx11_texture_2d_array(
			TKPA_valid<A_device> device_p,
			const F_initial_resource_data& initial_data,
			const F_resource_desc& desc,
			ED_resource_type overrided_type,
			ID3D11Texture2D* d3d11_texture_2d_array_p
		);
		virtual ~F_directx11_texture_2d_array();

	private:
		static ID3D11Texture2D* create_d3d11_texture_2d_array(
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

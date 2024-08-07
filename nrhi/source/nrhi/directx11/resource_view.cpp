#include <nrhi/directx11/resource_view.hpp>
#include <nrhi/directx11/resource.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/shader_resource_view.hpp>
#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/directx11/depth_stencil_view.hpp>
#include <nrhi/directx11/unordered_access_view.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

	F_directx11_resource_view::F_directx11_resource_view(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc,
		ED_resource_view_type overrided_type
	) :
		A_resource_view(device_p, desc, overrided_type)
	{

	}
    F_directx11_resource_view::F_directx11_resource_view(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc,
        ED_resource_view_type overrided_type,
        ID3D11View* d3d11_view_p
    ) :
        A_resource_view(device_p, desc, overrided_type),
        d3d11_view_p_(d3d11_view_p)
    {

    }
    F_directx11_resource_view::~F_directx11_resource_view() {

        if(d3d11_view_p_)
            d3d11_view_p_->Release();
    }



	TU<A_resource_view> HD_directx11_resource_view::create(
		TKPA_valid<A_device> device_p,
		const F_resource_view_desc& desc
	){
		NRHI_ENUM_SWITCH(
			desc.type,
			NRHI_ENUM_CASE(
				ED_resource_view_type::SHADER_RESOURCE,
				return { TU<F_directx11_shader_resource_view>()(device_p, desc) };
            )
			NRHI_ENUM_CASE(
				ED_resource_view_type::UNORDERED_ACCESS,
				return { TU<F_directx11_unordered_access_view>()(device_p, desc) };
            )
			NRHI_ENUM_CASE(
				ED_resource_view_type::RENDER_TARGET,
				return { TU<F_directx11_render_target_view>()(device_p, desc) };
            )
			NRHI_ENUM_CASE(
				ED_resource_view_type::DEPTH_STENCIL,
				return { TU<F_directx11_depth_stencil_view>()(device_p, desc) };
            )
		);
	}

    U_srv_handle HD_directx11_resource_view::create_srv(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

        return { TU<F_directx11_shader_resource_view>()(device_p, desc) };
    }
    U_uav_handle HD_directx11_resource_view::create_uav(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

		return { TU<F_directx11_unordered_access_view>()(device_p, desc) };
    }
    U_rtv_handle HD_directx11_resource_view::create_rtv(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

        return { TU<F_directx11_render_target_view>()(device_p, desc) };
    }
    U_dsv_handle HD_directx11_resource_view::create_dsv(
        TKPA_valid<A_device> device_p,
        const F_resource_view_desc& desc
    ){

		return { TU<F_directx11_depth_stencil_view>()(device_p, desc) };
    }

	U_srv_handle HD_directx11_resource_view::create_default_srv(
		TKPA_valid<A_resource> resource_p
	) {
		return {
			TU<F_directx11_shader_resource_view>()(
				resource_p->device_p(),
				F_resource_view_desc {
					.resource_p = resource_p
				}
			)
		};
	}
	U_uav_handle HD_directx11_resource_view::create_default_uav(
		TKPA_valid<A_resource> resource_p
	) {
		return {
			TU<F_directx11_unordered_access_view>()(
				resource_p->device_p(),
				F_resource_view_desc {
					.resource_p = resource_p
				}
			)
		};
	}
	U_rtv_handle HD_directx11_resource_view::create_default_rtv(
		TKPA_valid<A_resource> resource_p
	) {
		return {
			TU<F_directx11_render_target_view>()(
				resource_p->device_p(),
				F_resource_view_desc {
					.resource_p = resource_p
				}
			)
		};
	}
	U_dsv_handle HD_directx11_resource_view::create_default_dsv(
		TKPA_valid<A_resource> resource_p
	) {
		return {
			TU<F_directx11_depth_stencil_view>()(
				resource_p->device_p(),
				F_resource_view_desc {
					.resource_p = resource_p
				}
			)
		};
	}

}
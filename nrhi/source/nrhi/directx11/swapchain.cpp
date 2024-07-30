#include <nrhi/directx11/swapchain.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/directx11/command_queue.hpp>
#include <nrhi/directx11/texture.hpp>
#include <nrhi/directx11/render_target_view.hpp>
#include <nrhi/resource_desc.hpp>
#include <nrhi/directx11/factory.hpp>



namespace nrhi {

    F_directx11_swapchain::F_directx11_swapchain(
        TKPA_valid<A_command_queue> command_queue_p,
        TKPA_valid<F_surface> surface_p,
        const F_swapchain_desc& desc
    ) :
        A_swapchain(
            command_queue_p,
            surface_p,
            desc
        )
    {
        auto device_p = command_queue_p->device_p();

        // create d3d11 swapchain
        DXGI_SWAP_CHAIN_DESC dxgi_swapchain_desc;
        ZeroMemory(&dxgi_swapchain_desc, sizeof(dxgi_swapchain_desc));
        dxgi_swapchain_desc.BufferCount = 1;
        dxgi_swapchain_desc.BufferDesc.Width = surface_p->client_size().x;
        dxgi_swapchain_desc.BufferDesc.Height = surface_p->client_size().y;
        dxgi_swapchain_desc.BufferDesc.Format = (DXGI_FORMAT)(desc.format);
        dxgi_swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
        dxgi_swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
        dxgi_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxgi_swapchain_desc.OutputWindow = surface_p.T_cast<F_windows_surface>()->handle();
        dxgi_swapchain_desc.SampleDesc.Count = desc.sample_desc.count;
        dxgi_swapchain_desc.SampleDesc.Quality = desc.sample_desc.quality;
        dxgi_swapchain_desc.Windowed = TRUE;
        F_directx11_factory_helper::factory_p()->CreateSwapChain(
            device_p.T_cast<F_directx11_device>()->d3d11_device_p(),
            &dxgi_swapchain_desc,
            &dxgi_swapchain_p_
        );

        // create back texture 2d resource
        F_resource_desc buffer_desc = H_resource_desc::create_texture_2d_desc(
			surface_p->client_size().x,
			surface_p->client_size().y,
            desc.format,
            1,
            desc.sample_desc,
            ED_resource_bind_flag::RENDER_TARGET,
            ED_resource_heap_type::GREAD_GWRITE
        );
		buffer_desc.can_create_view = false;

		buffer_p_ = {
			TU<F_directx11_texture_2d>()(
				device_p,
				F_initial_resource_data {},
				buffer_desc,
				ED_resource_type::TEXTURE_2D,
				(ID3D11Texture2D*)0
			)
		};

        // create back texture 2d rtv
		F_resource_view_desc back_rtv_desc = {
			.resource_p = NCPP_AOH_VALID(buffer_p_)
		};
		back_rtv_p_ = {
			TU<F_directx11_render_target_view>()(
				device_p,
				back_rtv_desc,
				ED_resource_view_type::RENDER_TARGET,
				(ID3D11RenderTargetView*)0
			)
		};

        // back rtv has no d3d11 object now
        // so we need to update d3d11 object for it
        update_d3d11_object_for_buffer_rtv();

        // also update_d3d11_object_for_buffer_rtvs when surface is resized
        surface_resize_handle_ = surface_p->T_get_event<F_surface_resize_event>().T_push_back_listener([this](auto& event){

			auto d3d11_buffer_p = NCPP_FOH_VALID(buffer_p_).T_cast<F_directx11_texture_2d>();
			auto d3d11_back_rtv_p = NCPP_FOH_VALID(back_rtv_p_).T_cast<F_directx11_render_target_view>();

			if (d3d11_back_rtv_p->d3d11_view_p())
				d3d11_back_rtv_p->d3d11_view_p()->Release();

			auto surface_size = this->surface_p()->client_size();

			auto& back_texture_2d_desc = inject_resource_desc(NCPP_FHANDLE_VALID_AS_OREF(d3d11_buffer_p));
			back_texture_2d_desc.width = surface_size.x;
			back_texture_2d_desc.height = surface_size.y;

			HRESULT hr = dxgi_swapchain_p_->ResizeBuffers(
				0,
				surface_size.x,
				surface_size.y,
				(DXGI_FORMAT)(this->desc().format),
				DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
			);
			NCPP_ASSERT(!FAILED(hr)) << "resize d3d11 swapchain failed";

			update_d3d11_object_for_buffer_rtv();
        });

    }
    F_directx11_swapchain::~F_directx11_swapchain(){

        NCPP_ASSERT(surface_p().is_valid()) << "surface is destroyed";

        surface_p()->T_get_event<F_surface_resize_event>().remove_listener(surface_resize_handle_);

		buffer_p_.reset();

        if(dxgi_swapchain_p_)
            dxgi_swapchain_p_->Release();
    }

    void F_directx11_swapchain::update_d3d11_object_for_buffer_rtv(){

		auto d3d11_buffer_p = NCPP_FOH_VALID(buffer_p_).T_cast<F_directx11_texture_2d>();
		auto d3d11_buffer_rtv_p = NCPP_FOH_VALID(back_rtv_p_).T_cast<F_directx11_render_target_view>();

		auto device_p = command_queue_p()->device_p();

		ID3D11Texture2D* main_rtbuffer_p = 0;
		ID3D11RenderTargetView* main_rtview_p = 0;

		dxgi_swapchain_p_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&main_rtbuffer_p);

		HRESULT hr = device_p.T_cast<F_directx11_device>()->d3d11_device_p()->CreateRenderTargetView(
			main_rtbuffer_p,
			0,
			&main_rtview_p
		);
		NCPP_ASSERT(!FAILED(hr)) << "update d3d11 object for back rtv failed";

		main_rtbuffer_p->Release();

		d3d11_buffer_rtv_p->set_d3d11_view_p_unsafe(main_rtview_p);

		++inject_resource_generation(
			NCPP_FOH_VALID(d3d11_buffer_p.T_cast<A_resource>())
		);
		++inject_resource_view_generation(
			NCPP_FOH_VALID(d3d11_buffer_rtv_p.T_cast<A_resource_view>())
		);
    }



    TU<A_swapchain> HD_directx11_swapchain::create(
        TKPA_valid<A_command_queue> command_queue_p,
        TKPA_valid<F_surface> surface_p,
        const F_swapchain_desc& desc
    ){
        return TU<F_directx11_swapchain>()(
            command_queue_p,
            surface_p,
            desc
        );
    }

	K_valid_rtv_handle HD_directx11_swapchain::back_rtv_p(TKPA_valid<A_swapchain> swapchain_p) {

		auto dx11_swapchain_p = swapchain_p.T_cast<F_directx11_swapchain>();

		return NCPP_FOH_VALID(dx11_swapchain_p->back_rtv_p_);
	}
	K_valid_texture_2d_handle HD_directx11_swapchain::back_buffer_p(TKPA_valid<A_swapchain> swapchain_p) {

		auto dx11_swapchain_p = swapchain_p.T_cast<F_directx11_swapchain>();

		return NCPP_FOH_VALID(dx11_swapchain_p->buffer_p_);
	}

	void HD_directx11_swapchain::present(TKPA_valid<A_swapchain> swapchain_p){

		auto dx11_swapchain_p = swapchain_p.T_cast<F_directx11_swapchain>();

		dx11_swapchain_p->dxgi_swapchain_p()->Present(0, 0);
	}

}
#include <nrhi/directx12/swapchain.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/command_queue.hpp>
//#include <nrhi/directx12/texture.hpp>
//#include <nrhi/directx12/render_target_view.hpp>
#include <nrhi/resource_desc.hpp>
#include <nrhi/directx12/factory.hpp>



namespace nrhi {

	F_directx12_swapchain::F_directx12_swapchain(
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

		// create d3d12 swapchain
		DXGI_SWAP_CHAIN_DESC1 dxgi_swapchain_desc;
		ZeroMemory(&dxgi_swapchain_desc, sizeof(dxgi_swapchain_desc));
		dxgi_swapchain_desc.BufferCount = desc.back_rtv_count + 1;
		dxgi_swapchain_desc.Width = surface_p->client_size().x;
		dxgi_swapchain_desc.Height = surface_p->client_size().y;
		dxgi_swapchain_desc.Format = (DXGI_FORMAT)(desc.format);
		dxgi_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//		dxgi_swapchain_desc.OutputWindow = surface_p.T_cast<F_windows_surface>()->handle();
		dxgi_swapchain_desc.SampleDesc.Count = desc.sample_desc.count;
		dxgi_swapchain_desc.SampleDesc.Quality = desc.sample_desc.quality;
		dxgi_swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		((IDXGIFactory4*)F_directx12_factory_helper::factory_p())->CreateSwapChainForHwnd(
			command_queue_p.T_cast<F_directx12_command_queue>()->d3d12_command_queue_p(),
			surface_p.T_cast<F_windows_surface>()->handle(),
			&dxgi_swapchain_desc,
			0,
			0,
			(IDXGISwapChain1**)&dxgi_swapchain_p_
		);

		NCPP_ASSERT(dxgi_swapchain_p_) << "can't create swapchain";



		// create back texture 2d resource
		F_resource_desc buffer_desc = H_resource_desc::create_texture_2d_desc(
			surface_p->client_size().x,
			surface_p->client_size().y,
			desc.format,
			1,
			desc.sample_desc,
			ED_resource_bind_flag::RTV,
			ED_resource_heap_type::GREAD_GWRITE
		);
		buffer_desc.can_create_view = false;

//		for(u32 i = 0; i < desc.back_rtv_count; ++i) {
//
//			buffer_p_vector_.push_back({
//				TU<F_directx12_texture_2d>()(
//					device_p,
//					F_initial_resource_data {},
//					buffer_desc,
//					ED_resource_type::TEXTURE_2D,
//					(ID3D12Texture2D*)0
//				)
//			});
//		}

		// create back texture 2d rtv
//		for(u32 i = 0; i < desc.back_rtv_count; ++i) {
//
//			F_resource_view_desc back_rtv_desc = {
//				.resource_p = NCPP_FHANDLE_VALID_AS_OREF(buffer_p_vector_[i])
//			};
//			back_rtv_p_vector_.push_back({
//				TU<F_directx12_render_target_view>()(
//					device_p,
//					back_rtv_desc,
//					ED_resource_view_type::RTV,
//					(ID3D12RenderTargetView*)0
//				)
//			});
//		}



		// back rtv has no d3d12 object now
		// so we need to update d3d12 object for it
		update_d3d12_object_for_buffer_rtvs();



		// also update_d3d12_object_for_buffer_rtvs when surface is resized
		surface_resize_handle_ = surface_p->T_get_event<F_surface_resize_event>().T_push_back_listener([this](auto& event){

//		  	u32 back_rtv_count = this->desc().back_rtv_count;
//		  	for(u32 i = 0; i < back_rtv_count; ++i)
//		  	{
//			  	auto d3d12_buffer_p = NCPP_FOH_VALID(buffer_p_vector_[i]).T_cast<F_directx12_texture_2d>();
//			  	auto d3d12_back_rtv_p = NCPP_FOH_VALID(back_rtv_p_vector_[i]).T_cast<F_directx12_render_target_view>();
//
//			  	if (d3d12_back_rtv_p->d3d12_view_p())
//				  	d3d12_back_rtv_p->d3d12_view_p()->Release();
//
//			  	auto surface_size = this->surface_p()->client_size();
//
//			  	auto& back_texture_2d_desc = inject_resource_desc(NCPP_FHANDLE_VALID_AS_OREF(d3d12_buffer_p));
//			  	back_texture_2d_desc.width = surface_size.x;
//			  	back_texture_2d_desc.height = surface_size.y;
//
//			  	HRESULT hr = dxgi_swapchain_p_->ResizeBuffers(
//				  	0,
//				  	surface_size.x,
//				  	surface_size.y,
//				  	(DXGI_FORMAT)(this->desc().format),
//				  	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
//			  	);
//			 	NCPP_ASSERT(!FAILED(hr)) << "resize d3d12 swapchain failed";
//
//			  	update_d3d12_object_for_buffer_rtvs();
//		  	}
		});

	}
	F_directx12_swapchain::~F_directx12_swapchain(){

		NCPP_ASSERT(surface_p().is_valid()) << "surface is destroyed";

		surface_p()->T_get_event<F_surface_resize_event>().remove_listener(surface_resize_handle_);

		if(dxgi_swapchain_p_)
			dxgi_swapchain_p_->Release();
	}

	void F_directx12_swapchain::update_d3d12_object_for_buffer_rtvs(){

//		u32 back_rtv_count = desc().back_rtv_count;
//		for(u32 i = 0; i < back_rtv_count; ++i)
//		{
//			auto d3d12_buffer_p = NCPP_FOH_VALID(buffer_p_vector_[i]).T_cast<F_directx12_texture_2d>();
//			auto d3d12_buffer_rtv_p = NCPP_FOH_VALID(back_rtv_p_vector_[i]).T_cast<F_directx12_render_target_view>();
//
//			auto device_p = command_queue_p()->device_p();
//
//			ID3D12Texture2D* main_rtbuffer_p = 0;
//			ID3D12RenderTargetView* main_rtview_p = 0;
//
//			dxgi_swapchain_p_->GetBuffer(i, __uuidof(ID3D12Texture2D), (void**)&main_rtbuffer_p);
//
//			HRESULT hr = device_p.T_cast<F_directx12_device>()->d3d12_device_p()->CreateRenderTargetView(
//				main_rtbuffer_p,
//				0,
//				&main_rtview_p
//			);
//			NCPP_ASSERT(!FAILED(hr)) << "update d3d12 object for back rtv failed";
//
//			main_rtbuffer_p->Release();
//
//			d3d12_buffer_rtv_p->set_d3d12_view_p(main_rtview_p);
//
//			++inject_resource_generation(
//				NCPP_FOH_VALID(d3d12_buffer_p.T_cast<A_resource>())
//			);
//			++inject_resource_view_generation(
//				NCPP_FOH_VALID(d3d12_buffer_rtv_p.T_cast<A_resource_view>())
//			);
//		}
	}



	TU<A_swapchain> HD_directx12_swapchain::create(
		TKPA_valid<A_command_queue> command_queue_p,
		TKPA_valid<F_surface> surface_p,
		const F_swapchain_desc& desc
	){
		return TU<F_directx12_swapchain>()(
			command_queue_p,
			surface_p,
			desc
		);
	}

	u8 HD_directx12_swapchain::current_back_rtv_index(TKPA_valid<A_swapchain> swapchain_p) {

		auto dx12_swapchain_p = swapchain_p.T_cast<F_directx12_swapchain>();

		return dx12_swapchain_p->dxgi_swapchain_p_->GetCurrentBackBufferIndex();
	}

	void HD_directx12_swapchain::async_present(TKPA_valid<A_swapchain> swapchain_p){

		auto dx12_swapchain_p = swapchain_p.T_cast<F_directx12_swapchain>();

		dx12_swapchain_p->dxgi_swapchain_p()->Present(0, 0);
	}

}
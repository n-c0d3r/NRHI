#include <nrhi/directx12/swapchain.hpp>
#include <nrhi/directx12/device.hpp>
#include <nrhi/directx12/command_queue.hpp>
#include <nrhi/directx12/resource_view.hpp>
#include <nrhi/directx12/committed_resource.hpp>
#include <nrhi/resource_desc.hpp>
#include <nrhi/directx12/factory.hpp>
#include <nrhi/descriptor_heap.hpp>
#include <nrhi/device.hpp>



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
		auto rtv_descriptor_increment_size = HD_directx12_device::descriptor_increment_size(device_p, ED_descriptor_heap_type::RENDER_TARGET);

		if(desc.refresh_rate == 0) {

			((u32&)(desc.refresh_rate)) = 10000;
		}

		// create d3d12 swapchain
		DXGI_SWAP_CHAIN_DESC dxgi_swapchain_desc;
		dxgi_swapchain_desc.BufferDesc.Width = surface_p->client_size().x;
		dxgi_swapchain_desc.BufferDesc.Height = surface_p->client_size().y;
		dxgi_swapchain_desc.BufferDesc.RefreshRate.Numerator = desc.refresh_rate;
		dxgi_swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
		dxgi_swapchain_desc.BufferDesc.Format = (DXGI_FORMAT)(desc.format);
		dxgi_swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		dxgi_swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		dxgi_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgi_swapchain_desc.BufferCount = desc.rtv_count;
		dxgi_swapchain_desc.OutputWindow = surface_p.T_cast<F_windows_surface>()->handle();
		dxgi_swapchain_desc.SampleDesc.Count = desc.sample_desc.count;
		dxgi_swapchain_desc.SampleDesc.Quality = desc.sample_desc.quality;
		dxgi_swapchain_desc.Windowed = true;
		dxgi_swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		dxgi_swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		((IDXGIFactory4*)F_directx12_factory_helper::factory_p())->CreateSwapChain(
			command_queue_p.T_cast<F_directx12_command_queue>()->d3d12_command_queue_p(),
			&dxgi_swapchain_desc,
			(IDXGISwapChain**)&dxgi_swapchain_p_
		);

		NCPP_ASSERT(dxgi_swapchain_p_) << "can't create swapchain";



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
		for(u32 i = 0; i < desc.rtv_count; ++i) {

			buffer_p_vector_.push_back({
				TU<F_directx12_committed_resource>()(
					device_p,
					buffer_desc,
					ED_resource_type::TEXTURE_2D,
					(ID3D12Resource*)0
				)
			});
		}

		back_buffer_p_ = {
			TU<F_directx12_committed_resource>()(
				device_p,
				buffer_desc,
				ED_resource_type::TEXTURE_2D,
				(ID3D12Resource*)0
			)
		};

		// create rtv descriptor heap
		rtv_descriptor_heap_p_ = H_descriptor_heap::create(
			device_p,
			{
				.type = ED_descriptor_heap_type::RENDER_TARGET,
				.descriptor_count = desc.rtv_count
			}
		);
		F_descriptor_cpu_address rtv_descriptor_heap_base_cpu_address = HD_directx12_descriptor_heap::base_cpu_address(
			NCPP_FOH_VALID(rtv_descriptor_heap_p_)
		);

		// create texture 2d rtv
		for(u32 i = 0; i < desc.rtv_count; ++i) {

			F_resource_view_desc rtv_desc = {
				.resource_p = NCPP_AOH_VALID(buffer_p_vector_[i])
			};
			rtv_p_vector_.push_back({
				TU<F_directx12_resource_view>()(
					device_p,
					rtv_desc,
					F_descriptor {
						.handle = {
							.cpu_address = rtv_descriptor_heap_base_cpu_address + i * rtv_descriptor_increment_size
						},
						.heap_p = rtv_descriptor_heap_p_
					},
					ED_resource_view_type::RENDER_TARGET
				)
			});
		}

		F_resource_view_desc rtv_desc = {
			.resource_p = NCPP_AOH_VALID(buffer_p_vector_[0])
		};
		back_rtv_p_ ={
			TU<F_directx12_resource_view>()(
				device_p,
				rtv_desc,
				F_descriptor {
					.handle = {
						.cpu_address = rtv_descriptor_heap_base_cpu_address + 0 * rtv_descriptor_increment_size
					},
					.heap_p = rtv_descriptor_heap_p_
				},
				ED_resource_view_type::RENDER_TARGET
			)
		};

		// rtv has no d3d12 object now
		// so we need to update d3d12 object for it
		update_d3d12_object_for_buffer_rtvs();

		HD_directx12_swapchain::update_back_rtv(
			NCPP_KTHIS()
		);

		// also update_d3d12_object_for_buffer_rtvs when surface is resized
		surface_resize_handle_ = surface_p->T_get_event<F_surface_resize_event>().T_push_back_listener([this, surface_p](auto& event){

		  	u32 rtv_count = this->desc().rtv_count;
		  	for(u32 i = 0; i < rtv_count; ++i)
		  	{
			  	auto dx12_buffer_p = NCPP_FOH_VALID(buffer_p_vector_[i]).T_cast<F_directx12_committed_resource>();
			  	auto dx12_rtv_p = NCPP_FOH_VALID(rtv_p_vector_[i]).T_cast<F_directx12_resource_view>();

			  	if(dx12_buffer_p->d3d12_resource_p()) {

					dx12_buffer_p->d3d12_resource_p()->Release();
					dx12_buffer_p->set_d3d12_resource_p_unsafe(0);
			  	}

			  	auto surface_size = this->surface_p()->client_size();

			  	auto& texture_2d_desc = inject_resource_desc(NCPP_FHANDLE_VALID_AS_OREF(dx12_buffer_p));
			  	texture_2d_desc.width = surface_size.x;
			  	texture_2d_desc.height = surface_size.y;
		  	}

		  	HRESULT hr = dxgi_swapchain_p_->ResizeBuffers(
			  	this->desc().rtv_count,
				surface_p->desc().size.x,
				surface_p->desc().size.y,
			  	(DXGI_FORMAT)(this->desc().format),
			  	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		  	);
		  	NCPP_ASSERT(!FAILED(hr)) << "resize d3d12 swapchain failed";

		  	update_d3d12_object_for_buffer_rtvs();

		  	HD_directx12_swapchain::update_back_rtv(
			  	NCPP_KTHIS()
		  	);
		});

	}
	F_directx12_swapchain::~F_directx12_swapchain(){

		NCPP_ASSERT(surface_p().is_valid()) << "surface is destroyed";

		surface_p()->T_get_event<F_surface_resize_event>().remove_listener(surface_resize_handle_);

		NCPP_FOH_VALID(back_buffer_p_).T_cast<F_directx12_resource>()->set_d3d12_resource_p_unsafe(0);

		if(dxgi_swapchain_p_)
			dxgi_swapchain_p_->Release();
	}

	void F_directx12_swapchain::update_d3d12_object_for_buffer_rtvs(){

		u32 rtv_count = desc().rtv_count;
		for(u32 i = 0; i < rtv_count; ++i)
		{
			auto dx12_buffer_p = NCPP_FOH_VALID(buffer_p_vector_[i]).T_cast<F_directx12_committed_resource>();
			auto dx12_buffer_rtv_p = NCPP_FOH_VALID(rtv_p_vector_[i]).T_cast<F_directx12_resource_view>();

			auto device_p = command_queue_p()->device_p();
			auto d3d12_device_p = device_p.T_cast<F_directx12_device>()->d3d12_device_p();

			ID3D12Resource* main_rtbuffer_p = 0;
			const F_descriptor& main_rtview_descriptor = dx12_buffer_rtv_p->descriptor();

			dxgi_swapchain_p_->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&main_rtbuffer_p);

			d3d12_device_p->CreateRenderTargetView(
				main_rtbuffer_p,
				0,
				{ main_rtview_descriptor.handle.cpu_address }
			);
			NCPP_ASSERT(!FAILED(d3d12_device_p->GetDeviceRemovedReason())) << "update d3d12 object for rtv failed";

			dx12_buffer_p->set_d3d12_resource_p_unsafe(main_rtbuffer_p);

			++inject_resource_generation(
				NCPP_FOH_VALID(dx12_buffer_p.T_cast<A_resource>())
			);
			++inject_resource_view_generation(
				NCPP_FOH_VALID(dx12_buffer_rtv_p.T_cast<A_resource_view>())
			);
		}
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

	u8 HD_directx12_swapchain::current_rtv_index(TKPA_valid<A_swapchain> swapchain_p) {

		auto dx12_swapchain_p = swapchain_p.T_cast<F_directx12_swapchain>();

		return dx12_swapchain_p->dxgi_swapchain_p_->GetCurrentBackBufferIndex();
	}
	K_valid_rtv_handle HD_directx12_swapchain::rtv_p(TKPA_valid<A_swapchain> swapchain_p, u8 index) {

		auto dx12_swapchain_p = swapchain_p.T_cast<F_directx12_swapchain>();

		return NCPP_FOH_VALID(dx12_swapchain_p->rtv_p_vector_[index]);
	}
	void HD_directx12_swapchain::update_back_rtv(TKPA_valid<A_swapchain> swapchain_p) {

		auto dx12_swapchain_p = swapchain_p.T_cast<F_directx12_swapchain>();

		u8 current_rtv_index = HD_directx12_swapchain::current_rtv_index(dx12_swapchain_p);

		// update rtv
		{
			auto& current_rtv_p = dx12_swapchain_p->rtv_p_vector_[current_rtv_index];
			auto& back_rtv_p = dx12_swapchain_p->back_rtv_p_;

			auto& current_rtv_descriptor = (F_descriptor&)(current_rtv_p->descriptor());
			auto& back_rtv_descriptor = (F_descriptor&)(back_rtv_p->descriptor());
			back_rtv_descriptor.handle = current_rtv_descriptor.handle;

			auto& current_rtv_desc = (F_resource_view_desc&)(current_rtv_p->desc());
			auto& back_rtv_desc = (F_resource_view_desc&)(back_rtv_p->desc());
			back_rtv_desc = back_rtv_desc;

			back_rtv_p->set_generation_unsafe(
				current_rtv_p->generation()
			);
		}

		// update buffer
		{
			auto& current_buffer_p = dx12_swapchain_p->buffer_p_vector_[current_rtv_index];
			auto& back_buffer_p = dx12_swapchain_p->back_buffer_p_;

			NCPP_FOH_VALID(back_buffer_p).T_cast<F_directx12_resource>()->set_d3d12_resource_p_unsafe(
				NCPP_FOH_VALID(current_buffer_p).T_cast<F_directx12_resource>()->d3d12_resource_p()
			);

			auto& current_buffer_desc = (F_resource_desc&)(current_buffer_p->desc());
			auto& back_buffer_desc = (F_resource_desc&)(back_buffer_p->desc());
			back_buffer_desc = current_buffer_desc;

			back_buffer_p->set_generation_unsafe(
				current_buffer_p->generation()
			);
		}
	}
	K_valid_rtv_handle HD_directx12_swapchain::back_rtv_p(TKPA_valid<A_swapchain> swapchain_p) {

		auto dx12_swapchain_p = swapchain_p.T_cast<F_directx12_swapchain>();

		return NCPP_FOH_VALID(dx12_swapchain_p->back_rtv_p_);
	}
	K_valid_texture_2d_handle HD_directx12_swapchain::back_buffer_p(TKPA_valid<A_swapchain> swapchain_p) {

		auto dx12_swapchain_p = swapchain_p.T_cast<F_directx12_swapchain>();

		return NCPP_FOH_VALID(dx12_swapchain_p->back_buffer_p_);
	}
	void HD_directx12_swapchain::async_present(TKPA_valid<A_swapchain> swapchain_p){

		auto dx12_swapchain_p = swapchain_p.T_cast<F_directx12_swapchain>();

		dx12_swapchain_p->dxgi_swapchain_p()->Present(0, 0);
	}

}
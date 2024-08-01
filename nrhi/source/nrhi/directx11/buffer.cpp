#include <nrhi/directx11/buffer.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_buffer::F_directx11_buffer(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        ED_resource_type overrided_type
    ) :
        F_directx11_resource(
            device_p,
            initial_data,
            desc,
            overrided_type,
            create_d3d11_buffer(
                device_p,
                initial_data,
                desc
            )
        )
    {
    }
    F_directx11_buffer::F_directx11_buffer(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        ED_resource_type overrided_type,
        ID3D11Buffer* d3d11_buffer_p
    ) :
        F_directx11_resource(device_p, initial_data, desc, overrided_type, d3d11_buffer_p)
    {

    }
    F_directx11_buffer::~F_directx11_buffer() {
    }

    ID3D11Buffer* F_directx11_buffer::create_d3d11_buffer(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data_,
        const F_resource_desc& desc_
    ) {

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

        ID3D11Buffer* d3d11_buffer_p = 0;

        D3D11_BUFFER_DESC d3d11_buffer_desc;
        d3d11_buffer_desc.BindFlags = NRHI_DRIVER_DIRECTX_11_MAP___RESOURCE_FLAG___TO___RESOURCE_FLAG(desc_.flags);
        d3d11_buffer_desc.ByteWidth = desc_.size;
        NRHI_ENUM_SWITCH(
			desc_.heap_type,
            NRHI_ENUM_CASE(
                ED_resource_heap_type::GREAD_GWRITE,
                d3d11_buffer_desc.CPUAccessFlags = 0;
                d3d11_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
                NRHI_ENUM_BREAK;
            )
            NRHI_ENUM_CASE(
                ED_resource_heap_type::GREAD_CWRITE,
                d3d11_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                d3d11_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
                NRHI_ENUM_BREAK;
            )
            NRHI_ENUM_CASE(
                ED_resource_heap_type::CREAD_GWRITE,
                d3d11_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                d3d11_buffer_desc.Usage = D3D11_USAGE_STAGING;
                NRHI_ENUM_BREAK;
            )
		);
        d3d11_buffer_desc.StructureByteStride = desc_.stride;
        d3d11_buffer_desc.MiscFlags = NRHI_DRIVER_DIRECTX_11_MAP___RESOURCE_FLAG___TO___RESOURCE_MISC_FLAG(desc_.flags);

        D3D11_SUBRESOURCE_DATA d3d11_subresource_data;
        D3D11_SUBRESOURCE_DATA* d3d11_subresource_data_p = 0;
        if(initial_data_.size()) {

            d3d11_subresource_data.pSysMem = initial_data_[0].data_p;
            d3d11_subresource_data.SysMemPitch = 0;
            d3d11_subresource_data.SysMemSlicePitch = 0;
            d3d11_subresource_data_p = &d3d11_subresource_data;
        }

        d3d11_device_p->CreateBuffer(
            &d3d11_buffer_desc,
            d3d11_subresource_data_p,
            &d3d11_buffer_p
        );

        NCPP_ASSERT(d3d11_buffer_p) << "buffer creation failed";

        return d3d11_buffer_p;
    }

	void F_directx11_buffer::rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		if(d3d11_resource_p_)
			d3d11_resource_p_->Release();
		d3d11_resource_p_ = create_d3d11_buffer(
			device_p(),
			initial_data,
			desc
		);
		finalize_rebuild(
			initial_data,
			desc
		);
	}

}
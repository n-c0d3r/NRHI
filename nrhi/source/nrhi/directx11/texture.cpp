#include <nrhi/directx11/texture.hpp>
#include <nrhi/directx11/device.hpp>
#include <nrhi/format_helper.hpp>



namespace nrhi {

    F_directx11_texture_1d::F_directx11_texture_1d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        E_resource_type overrided_type
    ) :
        F_directx11_resource(
            device_p,
            initial_data,
            desc,
            overrided_type,
            create_d3d11_texture_1d(
                device_p,
                initial_data,
                desc
            )
        )
    {
    }
    F_directx11_texture_1d::F_directx11_texture_1d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        E_resource_type overrided_type,
        ID3D11Texture1D* d3d11_texture_1d_p
    ) :
        F_directx11_resource(device_p, initial_data, desc, overrided_type, d3d11_texture_1d_p)
    {

    }
    F_directx11_texture_1d::~F_directx11_texture_1d() {
    }

    ID3D11Texture1D* F_directx11_texture_1d::create_d3d11_texture_1d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data_,
        const F_resource_desc& desc_
    ) {

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

        ID3D11Texture1D* d3d11_texture_1d_p = 0;

        D3D11_TEXTURE1D_DESC d3d11_texture_1d_desc;
        d3d11_texture_1d_desc.Width = desc_.width;
        d3d11_texture_1d_desc.MipLevels = desc_.mip_level_count;
        d3d11_texture_1d_desc.ArraySize = 1;
        d3d11_texture_1d_desc.BindFlags = D3D11_BIND_FLAG(desc_.bind_flags);
        d3d11_texture_1d_desc.Format = DXGI_FORMAT(desc_.format);
        switch (desc_.heap_type) {
            case E_resource_heap_type::GREAD_GWRITE:
                d3d11_texture_1d_desc.CPUAccessFlags = 0;
                d3d11_texture_1d_desc.Usage = D3D11_USAGE_DEFAULT;
                break;
            case E_resource_heap_type::GREAD_CWRITE:
                d3d11_texture_1d_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                d3d11_texture_1d_desc.Usage = D3D11_USAGE_DYNAMIC;
                break;
            case E_resource_heap_type::CREAD_GWRITE:
                d3d11_texture_1d_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                d3d11_texture_1d_desc.Usage = D3D11_USAGE_STAGING;
                break;
        }
        d3d11_texture_1d_desc.MiscFlags = 0;

		if(desc_.is_mip_map_generatable)
		{
			d3d11_texture_1d_desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			d3d11_texture_1d_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			d3d11_texture_1d_desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}

		TG_vector<D3D11_SUBRESOURCE_DATA> d3d11_subresource_datas(initial_data_.size());
        if(initial_data_.size()) {

			for(u32 i = 0; i < initial_data_.size(); ++i) {

				auto& d3d11_subresource_data = d3d11_subresource_datas[i];
				d3d11_subresource_data.pSysMem = initial_data_[i].data_p;
				d3d11_subresource_data.SysMemPitch = 0;
				d3d11_subresource_data.SysMemSlicePitch = 0;
			}
        }

        d3d11_device_p->CreateTexture1D(
            &d3d11_texture_1d_desc,
			d3d11_subresource_datas.begin(),
            &d3d11_texture_1d_p
        );

        NCPP_ASSERT(d3d11_texture_1d_p) << "texture_1d creation failed";

        return d3d11_texture_1d_p;
    }

	void F_directx11_texture_1d::rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		if(d3d11_resource_p_)
			d3d11_resource_p_->Release();
		d3d11_resource_p_ = create_d3d11_texture_1d(
			device_p(),
			initial_data,
			desc
		);
		finalize_rebuild(
			initial_data,
			desc
		);
	}



    F_directx11_texture_2d::F_directx11_texture_2d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        E_resource_type overrided_type
    ) :
        F_directx11_resource(
            device_p,
            initial_data,
            desc,
            overrided_type,
            create_d3d11_texture_2d(
                device_p,
                initial_data,
                desc
            )
        )
    {
    }
    F_directx11_texture_2d::F_directx11_texture_2d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        E_resource_type overrided_type,
        ID3D11Texture2D* d3d11_texture_2d_p
    ) :
        F_directx11_resource(device_p, initial_data, desc, overrided_type, d3d11_texture_2d_p)
    {

    }
    F_directx11_texture_2d::~F_directx11_texture_2d() {
    }

    ID3D11Texture2D* F_directx11_texture_2d::create_d3d11_texture_2d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data_,
        const F_resource_desc& desc_
    ) {

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

        ID3D11Texture2D* d3d11_texture_2d_p = 0;

        D3D11_TEXTURE2D_DESC d3d11_texture_2d_desc;
        d3d11_texture_2d_desc.Width = desc_.width;
        d3d11_texture_2d_desc.Height = desc_.height;
        d3d11_texture_2d_desc.MipLevels = desc_.mip_level_count;
        d3d11_texture_2d_desc.ArraySize = 1;
        d3d11_texture_2d_desc.BindFlags = D3D11_BIND_FLAG(desc_.bind_flags);
        d3d11_texture_2d_desc.Format = DXGI_FORMAT(desc_.format);
        d3d11_texture_2d_desc.SampleDesc.Count = desc_.sample_desc.count;
        d3d11_texture_2d_desc.SampleDesc.Quality = desc_.sample_desc.quality;
        switch (desc_.heap_type) {
            case E_resource_heap_type::GREAD_GWRITE:
                d3d11_texture_2d_desc.CPUAccessFlags = 0;
                d3d11_texture_2d_desc.Usage = D3D11_USAGE_DEFAULT;
                break;
            case E_resource_heap_type::GREAD_CWRITE:
                d3d11_texture_2d_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                d3d11_texture_2d_desc.Usage = D3D11_USAGE_DYNAMIC;
                break;
            case E_resource_heap_type::CREAD_GWRITE:
                d3d11_texture_2d_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                d3d11_texture_2d_desc.Usage = D3D11_USAGE_STAGING;
                break;
        }
        d3d11_texture_2d_desc.MiscFlags = 0;

		if(desc_.is_mip_map_generatable)
		{
			d3d11_texture_2d_desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			d3d11_texture_2d_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			d3d11_texture_2d_desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}

		TG_vector<D3D11_SUBRESOURCE_DATA> d3d11_subresource_datas(initial_data_.size());
		if(initial_data_.size()) {

			for(u32 i = 0; i < initial_data_.size(); ++i) {

				auto& d3d11_subresource_data = d3d11_subresource_datas[i];
				d3d11_subresource_data.pSysMem = initial_data_[i].data_p;
				d3d11_subresource_data.SysMemPitch = desc_.width * desc_.stride;
				d3d11_subresource_data.SysMemSlicePitch = 0;
			}
		}

        d3d11_device_p->CreateTexture2D(
            &d3d11_texture_2d_desc,
			d3d11_subresource_datas.begin(),
            &d3d11_texture_2d_p
        );

        NCPP_ASSERT(d3d11_texture_2d_p) << "texture_2d creation failed";

        return d3d11_texture_2d_p;
    }

	void F_directx11_texture_2d::rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		if(d3d11_resource_p_)
			d3d11_resource_p_->Release();
		d3d11_resource_p_ = create_d3d11_texture_2d(
			device_p(),
			initial_data,
			desc
		);
		finalize_rebuild(
			initial_data,
			desc
		);
	}



    F_directx11_texture_3d::F_directx11_texture_3d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        E_resource_type overrided_type
    ) :
        F_directx11_resource(
            device_p,
            initial_data,
            desc,
            overrided_type,
            create_d3d11_texture_3d(
                device_p,
                initial_data,
                desc
            )
        )
    {
    }
    F_directx11_texture_3d::F_directx11_texture_3d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        const F_resource_desc& desc,
        E_resource_type overrided_type,
        ID3D11Texture3D* d3d11_texture_3d_p
    ) :
        F_directx11_resource(device_p, initial_data, desc, overrided_type, d3d11_texture_3d_p)
    {

    }
    F_directx11_texture_3d::~F_directx11_texture_3d() {
    }

    ID3D11Texture3D* F_directx11_texture_3d::create_d3d11_texture_3d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data_,
        const F_resource_desc& desc_
    ) {

        ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

        ID3D11Texture3D* d3d11_texture_3d_p = 0;

        D3D11_TEXTURE3D_DESC d3d11_texture_3d_desc;
        d3d11_texture_3d_desc.Width = desc_.width;
        d3d11_texture_3d_desc.Height = desc_.height;
        d3d11_texture_3d_desc.Depth = desc_.depth;
        d3d11_texture_3d_desc.MipLevels = desc_.mip_level_count;
        d3d11_texture_3d_desc.BindFlags = D3D11_BIND_FLAG(desc_.bind_flags);
        d3d11_texture_3d_desc.Format = DXGI_FORMAT(desc_.format);
        switch (desc_.heap_type) {
            case E_resource_heap_type::GREAD_GWRITE:
                d3d11_texture_3d_desc.CPUAccessFlags = 0;
                d3d11_texture_3d_desc.Usage = D3D11_USAGE_DEFAULT;
                break;
            case E_resource_heap_type::GREAD_CWRITE:
                d3d11_texture_3d_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                d3d11_texture_3d_desc.Usage = D3D11_USAGE_DYNAMIC;
                break;
            case E_resource_heap_type::CREAD_GWRITE:
                d3d11_texture_3d_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                d3d11_texture_3d_desc.Usage = D3D11_USAGE_STAGING;
                break;
        }
        d3d11_texture_3d_desc.MiscFlags = 0;

		if(desc_.is_mip_map_generatable)
		{
			d3d11_texture_3d_desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			d3d11_texture_3d_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			d3d11_texture_3d_desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}

		TG_vector<D3D11_SUBRESOURCE_DATA> d3d11_subresource_datas(initial_data_.size());
		if(initial_data_.size()) {

			for(u32 i = 0; i < initial_data_.size(); ++i) {

				auto& d3d11_subresource_data = d3d11_subresource_datas[i];
				d3d11_subresource_data.pSysMem = initial_data_[i].data_p;
				d3d11_subresource_data.SysMemPitch = desc_.width * desc_.stride;
				d3d11_subresource_data.SysMemSlicePitch = desc_.width * desc_.height * desc_.stride;
			}
		}
        d3d11_device_p->CreateTexture3D(
            &d3d11_texture_3d_desc,
			d3d11_subresource_datas.begin(),
            &d3d11_texture_3d_p
        );

        NCPP_ASSERT(d3d11_texture_3d_p) << "texture_3d creation failed";

        return d3d11_texture_3d_p;
    }

	void F_directx11_texture_3d::rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		if(d3d11_resource_p_)
			d3d11_resource_p_->Release();
		d3d11_resource_p_ = create_d3d11_texture_3d(
			device_p(),
			initial_data,
			desc
		);
		finalize_rebuild(
			initial_data,
			desc
		);
	}



	F_directx11_texture_2d_array::F_directx11_texture_2d_array(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc,
		E_resource_type overrided_type
	) :
		F_directx11_resource(
			device_p,
			initial_data,
			desc,
			overrided_type,
			create_d3d11_texture_2d_array(
				device_p,
				initial_data,
				desc
			)
		)
	{
	}
	F_directx11_texture_2d_array::F_directx11_texture_2d_array(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc,
		E_resource_type overrided_type,
		ID3D11Texture2D* d3d11_texture_2d_array_p
	) :
		F_directx11_resource(device_p, initial_data, desc, overrided_type, d3d11_texture_2d_array_p)
	{

	}
	F_directx11_texture_2d_array::~F_directx11_texture_2d_array() {
	}

	ID3D11Texture2D* F_directx11_texture_2d_array::create_d3d11_texture_2d_array(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data_,
		const F_resource_desc& desc_
	) {

		ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		ID3D11Texture2D* d3d11_texture_2d_array_p = 0;

		D3D11_TEXTURE2D_DESC d3d11_texture_2d_array_desc;
		d3d11_texture_2d_array_desc.Width = desc_.width;
		d3d11_texture_2d_array_desc.Height = desc_.height;
		d3d11_texture_2d_array_desc.MipLevels = desc_.mip_level_count;
		d3d11_texture_2d_array_desc.ArraySize = desc_.array_size;
		d3d11_texture_2d_array_desc.BindFlags = D3D11_BIND_FLAG(desc_.bind_flags);
		d3d11_texture_2d_array_desc.Format = DXGI_FORMAT(desc_.format);
		d3d11_texture_2d_array_desc.SampleDesc.Count = desc_.sample_desc.count;
		d3d11_texture_2d_array_desc.SampleDesc.Quality = desc_.sample_desc.quality;
		switch (desc_.heap_type) {
		case E_resource_heap_type::GREAD_GWRITE:
			d3d11_texture_2d_array_desc.CPUAccessFlags = 0;
			d3d11_texture_2d_array_desc.Usage = D3D11_USAGE_DEFAULT;
			break;
		case E_resource_heap_type::GREAD_CWRITE:
			d3d11_texture_2d_array_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			d3d11_texture_2d_array_desc.Usage = D3D11_USAGE_DYNAMIC;
			break;
		case E_resource_heap_type::CREAD_GWRITE:
			d3d11_texture_2d_array_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			d3d11_texture_2d_array_desc.Usage = D3D11_USAGE_STAGING;
			break;
		}
		d3d11_texture_2d_array_desc.MiscFlags = 0;

		if(desc_.is_mip_map_generatable)
		{
			d3d11_texture_2d_array_desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			d3d11_texture_2d_array_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			d3d11_texture_2d_array_desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}

		NCPP_ASSERT(desc_.array_size) << "texture 2d array size can't be zero";

		TG_vector<D3D11_SUBRESOURCE_DATA> d3d11_subresource_datas(initial_data_.size());
		if(initial_data_.size()) {

			for(u32 i = 0; i < initial_data_.size(); ++i) {

				auto& d3d11_subresource_data = d3d11_subresource_datas[i];
				d3d11_subresource_data.pSysMem = initial_data_[i].data_p;
				d3d11_subresource_data.SysMemPitch = desc_.width * desc_.stride;
				d3d11_subresource_data.SysMemSlicePitch = desc_.width * desc_.height * desc_.stride;
			}
		}
		d3d11_device_p->CreateTexture2D(
			&d3d11_texture_2d_array_desc,
			d3d11_subresource_datas.begin(),
			&d3d11_texture_2d_array_p
		);

		NCPP_ASSERT(d3d11_texture_2d_array_p) << "texture 2d array creation failed";

		return d3d11_texture_2d_array_p;
	}

	void F_directx11_texture_2d_array::rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		if(d3d11_resource_p_)
			d3d11_resource_p_->Release();
		d3d11_resource_p_ = create_d3d11_texture_2d_array(
			device_p(),
			initial_data,
			desc
		);
		finalize_rebuild(
			initial_data,
			desc
		);
	}



	F_directx11_texture_cube::F_directx11_texture_cube(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc,
		E_resource_type overrided_type
	) :
		F_directx11_texture_2d_array(
			device_p,
			initial_data,
			desc,
			overrided_type,
			create_d3d11_texture_cube(
				device_p,
				initial_data,
				desc
			)
		)
	{
	}
	F_directx11_texture_cube::F_directx11_texture_cube(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc,
		E_resource_type overrided_type,
		ID3D11Texture2D* d3d11_texture_cube_p
	) :
		F_directx11_texture_2d_array(device_p, initial_data, desc, overrided_type, d3d11_texture_cube_p)
	{

	}
	F_directx11_texture_cube::~F_directx11_texture_cube() {
	}

	ID3D11Texture2D* F_directx11_texture_cube::create_d3d11_texture_cube(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data_,
		const F_resource_desc& desc_
	) {

		ID3D11Device* d3d11_device_p = device_p.T_cast<F_directx11_device>()->d3d11_device_p();

		ID3D11Texture2D* d3d11_texture_cube_p = 0;

		D3D11_TEXTURE2D_DESC d3d11_texture_cube_desc;
		d3d11_texture_cube_desc.Width = desc_.width;
		d3d11_texture_cube_desc.Height = desc_.height;
		d3d11_texture_cube_desc.MipLevels = desc_.mip_level_count;
		d3d11_texture_cube_desc.ArraySize = 6;
		d3d11_texture_cube_desc.BindFlags = D3D11_BIND_FLAG(desc_.bind_flags);
		d3d11_texture_cube_desc.Format = DXGI_FORMAT(desc_.format);
		d3d11_texture_cube_desc.SampleDesc.Count = desc_.sample_desc.count;
		d3d11_texture_cube_desc.SampleDesc.Quality = desc_.sample_desc.quality;
		switch (desc_.heap_type) {
		case E_resource_heap_type::GREAD_GWRITE:
			d3d11_texture_cube_desc.CPUAccessFlags = 0;
			d3d11_texture_cube_desc.Usage = D3D11_USAGE_DEFAULT;
			break;
		case E_resource_heap_type::GREAD_CWRITE:
			d3d11_texture_cube_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			d3d11_texture_cube_desc.Usage = D3D11_USAGE_DYNAMIC;
			break;
		case E_resource_heap_type::CREAD_GWRITE:
			d3d11_texture_cube_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			d3d11_texture_cube_desc.Usage = D3D11_USAGE_STAGING;
			break;
		}
		d3d11_texture_cube_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		if(desc_.is_mip_map_generatable)
		{
			d3d11_texture_cube_desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			d3d11_texture_cube_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			d3d11_texture_cube_desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}

		TG_vector<D3D11_SUBRESOURCE_DATA> d3d11_subresource_datas(initial_data_.size());
		if(initial_data_.size()) {

			for(u32 i = 0; i < initial_data_.size(); ++i) {

				auto& d3d11_subresource_data = d3d11_subresource_datas[i];
				d3d11_subresource_data.pSysMem = initial_data_[i].data_p;
				d3d11_subresource_data.SysMemPitch = desc_.width * desc_.stride;
				d3d11_subresource_data.SysMemSlicePitch = desc_.width * desc_.height * desc_.stride;
			}
		}
		d3d11_device_p->CreateTexture2D(
			&d3d11_texture_cube_desc,
			d3d11_subresource_datas.begin(),
			&d3d11_texture_cube_p
		);

		NCPP_ASSERT(d3d11_texture_cube_p) << "texture cube creation failed";

		return d3d11_texture_cube_p;
	}

	void F_directx11_texture_cube::rebuild(
		const F_initial_resource_data& initial_data,
		const F_resource_desc& desc
	) {
		if(d3d11_resource_p_)
			d3d11_resource_p_->Release();
		d3d11_resource_p_ = create_d3d11_texture_cube(
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
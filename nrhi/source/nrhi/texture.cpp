#include <nrhi/texture.hpp>
#include <nrhi/resource.hpp>



namespace nrhi {

    U_texture_1d_handle H_texture::create_1d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 width,
        E_format format,
        u32 mip_level_count,
        ED_resource_bind_flag bind_flags,
        ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
    ) {

        return H_resource::create_texture_1d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_1d_desc(
                width,
                format,
                mip_level_count,
                bind_flags,
                heap_type,
				is_mip_map_generatable
            )
        );
    }

    U_texture_2d_handle H_texture::create_2d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 width,
        u32 height,
        E_format format,
        u32 mip_level_count,
        F_sample_desc sample_desc,
        ED_resource_bind_flag bind_flags,
        ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
    ) {

        return H_resource::create_texture_2d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_2d_desc(
                width,
                height,
                format,
                mip_level_count,
                sample_desc,
                bind_flags,
                heap_type,
				is_mip_map_generatable
            )
        );
    }

    U_texture_3d_handle H_texture::create_3d(
        TKPA_valid<A_device> device_p,
        const F_initial_resource_data& initial_data,
        u32 width,
        u32 height,
        u32 depth,
        E_format format,
        u32 mip_level_count,
        ED_resource_bind_flag bind_flags,
        ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
    ) {

        return H_resource::create_texture_3d(
            device_p,
            initial_data,
            H_resource_desc::create_texture_3d_desc(
                width,
                height,
                depth,
                format,
                mip_level_count,
                bind_flags,
                heap_type,
				is_mip_map_generatable
            )
        );
    }

	U_texture_2d_array_handle H_texture::create_2d_array(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		u32 height,
		u32 count,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		return H_resource::create_texture_2d_array(
			device_p,
			initial_data,
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				count,
				format,
				mip_level_count,
				sample_desc,
				bind_flags,
				heap_type,
				is_mip_map_generatable
			)
		);
	}

	U_texture_cube_handle H_texture::create_cube(
		TKPA_valid<A_device> device_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		return H_resource::create_texture_cube(
			device_p,
			initial_data,
			H_resource_desc::create_texture_cube_desc(
				width,
				format,
				mip_level_count,
				sample_desc,
				bind_flags,
				heap_type,
				is_mip_map_generatable
			)
		);
	}



	void H_texture::rebuild_1d(
		KPA_valid_texture_1d_handle texture_1d_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		E_format format,
		u32 mip_level_count,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		texture_1d_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_1d_desc(
				width,
				format,
				mip_level_count,
				bind_flags,
				heap_type,
				is_mip_map_generatable
			)
		);
	}

	void H_texture::rebuild_2d(
		KPA_valid_texture_2d_handle texture_2d_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		u32 height,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		texture_2d_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_2d_desc(
				width,
				height,
				format,
				mip_level_count,
				sample_desc,
				bind_flags,
				heap_type,
				is_mip_map_generatable
			)
		);
	}

	void H_texture::rebuild_3d(
		KPA_valid_texture_3d_handle texture_3d_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		u32 height,
		u32 depth,
		E_format format,
		u32 mip_level_count,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		texture_3d_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_3d_desc(
				width,
				height,
				depth,
				format,
				mip_level_count,
				bind_flags,
				heap_type,
				is_mip_map_generatable
			)
		);
	}

	void H_texture::rebuild_2d_array(
		KPA_valid_texture_2d_array_handle texture_2d_array_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		u32 height,
		u32 count,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		texture_2d_array_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_2d_array_desc(
				width,
				height,
				count,
				format,
				mip_level_count,
				sample_desc,
				bind_flags,
				heap_type,
				is_mip_map_generatable
			)
		);
	}

	void H_texture::rebuild_cube(
		KPA_valid_texture_cube_handle texture_cube_p,
		const F_initial_resource_data& initial_data,
		u32 width,
		E_format format,
		u32 mip_level_count,
		F_sample_desc sample_desc,
		ED_resource_bind_flag bind_flags,
		ED_resource_heap_type heap_type,
		b8 is_mip_map_generatable
	) {

		texture_cube_p->rebuild(
			initial_data,
			H_resource_desc::create_texture_cube_desc(
				width,
				format,
				mip_level_count,
				sample_desc,
				bind_flags,
				heap_type,
				is_mip_map_generatable
			)
		);
	}

}
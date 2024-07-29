#pragma once

/** @file nrhi/command_list_base.hpp
*
*   Implement command list base class.
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

#include <nrhi/device_child.hpp>
#include <nrhi/command_list_type.hpp>
#include <nrhi/graphics_pipeline_state_handle.hpp>
#include <nrhi/compute_pipeline_state_handle.hpp>
#include <nrhi/resource_view_handle.hpp>
#include <nrhi/buffer_handle.hpp>
#include <nrhi/clear_flag.hpp>

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
#include <nrhi/descriptor_base.hpp>
#include <nrhi/resource_barrier.hpp>
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

#pragma endregion



namespace nrhi {

    class A_device;
    class A_frame_buffer;
    class A_sampler_state;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
    class A_command_allocator;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION



    struct F_command_list_desc {

        ED_command_list_type type = ED_command_list_type::DIRECT;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
		TK<A_command_allocator> command_allocator_p;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

    };



    class NRHI_API A_command_list : public A_device_child {

    private:
        F_command_list_desc desc_;
		b8 supports_graphics_ = false;
		b8 supports_compute_ = false;
		b8 supports_blit_ = false;

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	protected:
		TK<A_command_allocator> command_allocator_p_;
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

    public:
        NCPP_FORCE_INLINE const F_command_list_desc& desc() const noexcept { return desc_; }
        NCPP_FORCE_INLINE b8 supports_graphics() const noexcept { return supports_graphics_; }
        NCPP_FORCE_INLINE b8 supports_compute() const noexcept { return supports_compute_; }
        NCPP_FORCE_INLINE b8 supports_blit() const noexcept { return supports_blit_; }

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
        NCPP_FORCE_INLINE TK<A_command_allocator> command_allocator_p() const noexcept { return command_allocator_p_; }
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION



    protected:
        A_command_list(TKPA_valid<A_device> device_p, const F_command_list_desc& desc);

    public:
        virtual ~A_command_list();

	public:
		NCPP_OBJECT(A_command_list);



	public:
		void clear_state();
		void clear_rtv(KPA_valid_rtv_handle rtv_p, PA_vector4 color);
		void clear_dsv(
			KPA_valid_dsv_handle dsv_p,
			ED_clear_flag flag,
			f32 depth,
			u8 stencil
		);

	public:
		void set_pipeline_state(TKPA_valid<A_pipeline_state> pipeline_state_p);
		void set_graphics_pipeline_state(KPA_valid_graphics_pipeline_state_handle graphics_pipeline_state_p);
		void set_compute_pipeline_state(KPA_valid_compute_pipeline_state_handle compute_pipeline_state_p);

	public:
		void ZIA_bind_index_buffer(
			KPA_valid_buffer_handle index_buffer_p,
			u32 offset
		);
		void ZIA_bind_vertex_buffers(
			const TG_span<K_valid_buffer_handle>& vertex_buffer_p_span,
			const TG_span<u32>& offset_span,
			u32 base_slot_index
		);
		void ZIA_bind_vertex_buffer(
			KPA_valid_buffer_handle vertex_buffer_p,
			u32 offset,
			u32 slot_index
		);
		void ZIA_bind_instance_buffers(
			const TG_span<K_valid_buffer_handle>& instance_buffer_p_span,
			const TG_span<u32>& offset_span,
			u32 base_slot_index
		);
		void ZIA_bind_instance_buffer(
			KPA_valid_buffer_handle instance_buffer_p,
			u32 offset,
			u32 slot_index
		);

	public:
		void ZVS_bind_constant_buffers(
			const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
			u32 base_slot_index
		);
		void ZVS_bind_constant_buffer(
			KPA_valid_buffer_handle constant_buffer_p,
			u32 slot_index
		);
		void ZVS_bind_srvs(
			const TG_span<K_valid_srv_handle>& srv_p_span,
			u32 base_slot_index
		);
		void ZVS_bind_srv(
			KPA_valid_srv_handle srv_p,
			u32 slot_index
		);
		void ZVS_bind_sampler_states(
			const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
			u32 base_slot_index
		);
		void ZVS_bind_sampler_state(
			TKPA_valid<A_sampler_state> sampler_state_p,
			u32 slot_index
		);

	public:
		void ZPS_bind_constant_buffers(
			const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
			u32 base_slot_index
		);
		void ZPS_bind_constant_buffer(
			KPA_valid_buffer_handle constant_buffer_p,
			u32 slot_index
		);
		void ZPS_bind_srvs(
			const TG_span<K_valid_srv_handle>& srv_p_span,
			u32 base_slot_index
		);
		void ZPS_bind_srv(
			KPA_valid_srv_handle srv_p,
			u32 slot_index
		);
		void ZPS_bind_sampler_states(
			const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
			u32 base_slot_index
		);
		void ZPS_bind_sampler_state(
			TKPA_valid<A_sampler_state> sampler_state_p,
			u32 slot_index
		);

	public:
		void ZOM_bind_frame_buffer(TKPA_valid<A_frame_buffer> frame_buffer_p);

	public:
		void ZCS_bind_constant_buffers(
			const TG_span<K_valid_buffer_handle>& constant_buffer_p_span,
			u32 base_slot_index
		);
		void ZCS_bind_constant_buffer(
			KPA_valid_buffer_handle constant_buffer_p,
			u32 slot_index
		);
		void ZCS_bind_srvs(
			const TG_span<K_valid_srv_handle>& srv_p_span,
			u32 base_slot_index
		);
		void ZCS_bind_srv(
			KPA_valid_srv_handle srv_p,
			u32 slot_index
		);
		void ZCS_bind_uavs(
			const TG_span<K_valid_uav_handle>& uav_p_span,
			u32 base_slot_index
		);
		void ZCS_bind_uav(
			KPA_valid_uav_handle uav_p,
			u32 slot_index
		);
		void ZCS_bind_sampler_states(
			const TG_span<TK_valid<A_sampler_state>>& sampler_state_p_span,
			u32 base_slot_index
		);
		void ZCS_bind_sampler_state(
			TKPA_valid<A_sampler_state> sampler_state_p,
			u32 slot_index
		);

#ifdef NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION
	public:
		void draw(
			u32 vertex_count,
			u32 base_vertex_location
		);
		void draw_instanced(
			u32 vertex_count_per_instance,
			u32 instance_count,
			u32 base_vertex_location,
			u32 base_instance_location
		);
		void draw_indexed(
			u32 index_count,
			u32 base_index_locatio,
			u32 base_vertex_locationn
		);
		void draw_indexed_instanced(
			u32 index_count_per_instance,
			u32 instance_count,
			u32 base_index_location,
			u32 base_vertex_location,
			u32 base_instance_location
		);

	public:
		void draw_instanced_indirect(
			KPA_indirect_buffer_handle indirect_buffer_p,
			u32 indirect_buffer_offset
		);
		void draw_indexed_instanced_indirect(
			KPA_indirect_buffer_handle indirect_buffer_p,
			u32 indirect_buffer_offset
		);

	public:
		void dispatch_indirect(
			KPA_indirect_buffer_handle indirect_buffer_p,
			u32 indirect_buffer_offset
		);

	public:
		void dispatch(PA_vector3_u32 thread_group_count_3d);
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION

#if defined(NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT) && defined(NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING) && defined(NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION)
	public:
		void update_resource_data(
			TKPA_valid<A_resource> resource_p,
			void* data_p,
			u32 data_size,
			u32 src_data_offset = 0,
			u32 dst_data_offset = 0
		);

	public:
		void generate_mips(
			KPA_valid_srv_handle srv_p
		);
#endif // NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_MANAGEMENT && NRHI_DRIVER_SUPPORT_SIMPLE_RESOURCE_BINDING && NRHI_DRIVER_SUPPORT_SIMPLE_WORK_SUBMISSION

#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	public:
		void async_begin(
			TKPA_valid<A_command_allocator> command_allocator_p
		);
		void async_end();

	public:
		void async_resource_barrier(
			const F_resource_barrier& resource_barrier
		);
		void async_resource_barriers(
			const TG_span<F_resource_barrier>& resource_barriers
		);

	public:
		void async_clear_state();
		void async_clear_rtv(
			K_valid_rtv_handle rtv_p,
			PA_vector4_f32 color
		);
		void async_clear_dsv(
			K_valid_dsv_handle dsv_p,
			ED_clear_flag flag,
			f32 depth,
			u8 stencil
		);
		void async_clear_rtv_with_descriptor(
			F_descriptor_cpu_address rtv_cpu_address,
			PA_vector4_f32 color
		);
		void async_clear_dsv_with_descriptor(
			F_descriptor_cpu_address dsv_cpu_address,
			ED_clear_flag flag,
			f32 depth,
			u8 stencil
		);

	public:
		void async_copy_resource(
			TKPA_valid<A_resource> dst_resource_p,
			TKPA_valid<A_resource> src_resource_p
		);
		void async_copy_buffer_region(
			TKPA_valid<A_resource> dst_resource_p,
			TKPA_valid<A_resource> src_resource_p,
			u64 dst_offset,
			u64 src_offset,
			u64 size
		);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

    };



#ifdef NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION
	class F_command_list_async_scope {

	private:
		TK_valid<A_command_list> command_list_p_;

	public:
		NCPP_FORCE_INLINE TKPA_valid<A_command_list> command_list_p() const noexcept { return command_list_p_; }



	public:
		NCPP_FORCE_INLINE F_command_list_async_scope(TKPA_valid<A_command_list> command_list_p, TKPA_valid<A_command_allocator> command_allocator_p) noexcept :
			command_list_p_(command_list_p)
		{
			command_list_p->async_begin(command_allocator_p);
		}
		NCPP_FORCE_INLINE ~F_command_list_async_scope() noexcept {

			command_list_p_->async_end();
		}

	};

#define NRHI_COMMAND_LIST_ASYNC_SCOPE(...) nrhi::F_command_list_async_scope NCPP_GLUE(___nrhi_command_list_async_scope_, NCPP_LINE)( \
            	__VA_ARGS__                                                                                                                   \
			);
#endif // NRHI_DRIVER_SUPPORT_ADVANCED_WORK_SUBMISSION

}

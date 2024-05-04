#pragma once

/** @file nrhi/command_queue_base.hpp
*
*   Implement command queue base class.
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

#include <nrhi/command_list_type.hpp>

#pragma endregion



namespace nrhi {

    class A_device;
    class A_command_list;



    struct F_command_queue_desc {

        E_command_list_type type;

    };



    class NRHI_API A_command_queue {

    private:
        TK_valid<A_device> device_p_;
        F_command_queue_desc desc_;

    public:
        NCPP_FORCE_INLINE TK_valid<A_device> device_p() noexcept { return device_p_; }
        NCPP_FORCE_INLINE const F_command_queue_desc& desc() const noexcept { return desc_; }



    protected:
        A_command_queue(TKPA_valid<A_device> device_p, const F_command_queue_desc& desc);

    public:
        virtual ~A_command_queue();



	public:
		void execute_command_lists(TG_span<TK_valid<A_command_list>> command_list_p_span);
		void execute_command_list(TKPA_valid<A_command_list> command_list_p);

    };

}

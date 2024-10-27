#pragma once

#include <nrhi/prerequisites.hpp>



namespace nrhi
{
    namespace internal
    {
        template<typename... Fs_specific_data__>
        struct TF_abstract_data_payload_size_helper;
        template<>
        struct TF_abstract_data_payload_size_helper<>
        {
            static constexpr sz value = 1;
        };
        template<typename F_first_specific_data__, typename... Fs_rest_specific_data__>
        struct TF_abstract_data_payload_size_helper<F_first_specific_data__, Fs_rest_specific_data__...>
        {
            static constexpr sz first_value = sizeof(F_first_specific_data__);
            static constexpr sz rest_value = TF_abstract_data_payload_size_helper<Fs_rest_specific_data__...>::value;
            static constexpr sz value = (
                (first_value > rest_value)
                ? first_value
                : rest_value
            );
        };

        template<typename... Fs_specific_data__>
        struct TF_abstract_data_alignment_helper;
        template<>
        struct TF_abstract_data_alignment_helper<>
        {
            static constexpr sz value = 1;
        };
        template<typename F_first_specific_data__, typename... Fs_rest_specific_data__>
        struct TF_abstract_data_alignment_helper<F_first_specific_data__, Fs_rest_specific_data__...>
        {
            static constexpr sz first_value = NCPP_ALIGNOF(F_first_specific_data__);
            static constexpr sz rest_value = TF_abstract_data_alignment_helper<Fs_rest_specific_data__...>::value;
            static constexpr sz value = (
                (first_value > rest_value)
                ? first_value
                : rest_value
            );
        };
    }



    template<class F_helper_class__, typename... Fs_specific_data__>
    struct NCPP_ALIGN(
        internal::TF_abstract_data_alignment_helper<Fs_specific_data__...>::value
    ) TF_abstract_data
    {
    public:
        using F_helper_class = F_helper_class__;
        using F_specific_data_targ_list = TF_template_targ_list<Fs_specific_data__...>;

        static constexpr sz payload_size = internal::TF_abstract_data_payload_size_helper<Fs_specific_data__...>::value;

        using F_payload = u8[payload_size];



    private:
        F_payload payload_;

    public:
        NCPP_FORCE_INLINE void* payload_p() const noexcept { return this; }



    public:
        NCPP_FORCE_INLINE TF_abstract_data() noexcept
        {
            F_helper_class::construct_default(this);
        }
        NCPP_FORCE_INLINE ~TF_abstract_data() noexcept
        {
            F_helper_class::destruct(this);
        }

        NCPP_FORCE_INLINE TF_abstract_data(const TF_abstract_data& x) noexcept
        {
            F_helper_class::construct_copy(this, &x);
        }
        NCPP_FORCE_INLINE TF_abstract_data& operator = (const TF_abstract_data& x) noexcept
        {
            F_helper_class::operator_copy(this, &x);
            return *this;
        }

        NCPP_FORCE_INLINE TF_abstract_data(TF_abstract_data&& x) noexcept
        {
            F_helper_class::construct_move(this, &x);
        }
        NCPP_FORCE_INLINE TF_abstract_data& operator = (TF_abstract_data&& x) noexcept
        {
            F_helper_class::operator_move(this, &x);
            return *this;
        }



    public:
        template<typename F__>
        NCPP_FORCE_INLINE F__& T_as() noexcept
        {
            return *((F__*)payload_);
        }
        template<typename F__>
        NCPP_FORCE_INLINE const F__& T_as() const noexcept
        {
            return *((const F__*)payload_);
        }
    };



    namespace internal
    {
        template<class F_helper_class__, typename, typename... Fs_specific_data__>
        using TF_to_abstract_data = TF_abstract_data<
            F_helper_class__,
            Fs_specific_data__...
        >;
    }
}



#define NRHI_DEFINE_ABSTRACT_DATA(Name, HelperClass, ...) using Name = nrhi::internal::TF_to_abstract_data<HelperClass, void __VA_OPT__(,) __VA_ARGS__>;
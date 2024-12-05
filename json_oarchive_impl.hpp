/***************************************************************************
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * Use, modification and distribution is subject to the Boost Software
 * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt).
 ***************************************************************************/

#pragma once
#include "json_archive.hpp"

namespace boost {
namespace archive {
class json_oarchive_impl :
    public detail::common_oarchive<json_oarchive_impl>,
    public json_archive_interface
{
public:
    using DataItem = std::pair<std::string, bool>;
    using DataVector = std::vector<DataItem>;
    using base_class = detail::common_oarchive<json_oarchive_impl>;

public:
    json_oarchive_impl();

    Json& get_json_data();

private:
    void write_data(Json& data, const Json& input);

private:
    friend class save_access;
    friend class detail::interface_oarchive<json_oarchive_impl>;

    // direct
    static const char* convert_type(const char* v);
    static const std::string& convert_type(const std::string& v);
    static bool convert_type(bool v);
    static std::nullptr_t convert_type(std::nullptr_t v);
    static long convert_type(boost::serialization::item_version_type v);
    // float
    template <class T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
    static double convert_type(T v);
    // int
    template <class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    static long convert_type(T v);
    // long
    template <class T,
              typename std::enable_if<!std::is_integral<T>::value, int>::type = 0,
              typename std::enable_if<!std::is_floating_point<T>::value, int>::type = 0,
              typename std::enable_if<std::is_convertible<T, long>::value, int>::type = 0>
    static long convert_type(T v);
    // error
    struct unknown {};
    static unknown convert_type(...);

    template <class T>
    void save(const T& t)
    {
        save_impl(static_cast< decltype(convert_type(t)) >(t));
    }
    // save impl
    template <class T>
    void save_impl(const T& v);
    void save_impl(const char* v);
    void save_impl(const std::string& v);
    void save_impl(double v);
    void save_impl(long v);
    void save_impl(bool v);
    void save_impl(std::nullptr_t v);
    void save_impl(const boost::serialization::item_version_type& v);

    void save_start(const char* name);
    void save_end(const char* name);
    void array_item(int number);
    void end_preamble();
    void save_binary(void* address, size_t count);

    //
    void save_override(char* const t)
    {
        save(t);
    }
    // write primitive
    template <class T>
    void save_override(T& t)
    {
        base_class::save_override(t);
    }
    // NVP(class)
    template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
    void save_override(const boost::serialization::nvp<T>& t)
    {
        if(class_id_ && class_id_type_ != ClassIDType::Reference)
        {
            if(class_map_.count(*class_id_) == 0)
            {
                class_map_[*class_id_] = class_info_;
            }
            class_info_.clear();
        }

        if(!t.name() && class_id_ && class_id_type_ != ClassIDType::Optional)
        {
            if(object_id_)
            {
                if(object_id_type_ == ObjectIDType::Normal)
                {
                    archive_.push_obj_repository(*class_id_, *object_id_);
                    base_class::save_override(t.const_value());
                    archive_.pop_to_mark();
                }
            }
            else
            {
                base_class::save_override(t.const_value());
            }

            clear();
            return;
        }

        save_start(t.name());
        base_class::save_override(t.const_value());
        save_end(t.name());

        clear();
    }
    // NVP(non-class)
    template <class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
    void save_override(const boost::serialization::nvp<T>& t)
    {
        save_start(t.name());
        base_class::save_override<const T&>(t.const_value());
        save_end(t.name());
    }
    // ARRAY
    template <class T>
    void save_override(const boost::serialization::array_wrapper<T>& t)
    {
        const size_t sz = t.count();
        for(size_t i = 0; i < sz; i++)
        {
            array_item(i);
            base_class::save_override(t.address()[i]);
            array_end();
        }
    }

    void save_override(const class_name_type& t);
    void save_override(const version_type& t);
    void save_override(const object_id_type& t);
    void save_override(const object_reference_type& t);
    void save_override(const class_id_type& t);
    void save_override(const class_id_optional_type& t);
    void save_override(const class_id_reference_type& t);
    void save_override(const tracking_type& t);

private:
    DataVector array_stack_;
    ClassInfo class_info_;
};
} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::json_oarchive_impl)

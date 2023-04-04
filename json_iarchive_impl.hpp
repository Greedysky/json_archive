/***************************************************************************
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * Use, modification and distribution is subject to the Boost Software
 * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt).
 ***************************************************************************/
#pragma once
#include "json_archive.hpp"

namespace boost {
namespace archive {
class json_iarchive_impl :
  public detail::common_iarchive<json_iarchive_impl>,
  public json_archive_interface
{
public:
  struct DataItem {
    std::string key_;
    int index_;
    bool pop_flag_;
  };
  using DataVector = std::vector<DataItem>;
  using base_class = detail::common_iarchive<json_iarchive_impl>;

public:
  json_iarchive_impl();

  void set_data(Json& data);

private:
  void   load_data();
  Json & get_data();

private:
  friend class load_access;
  friend class detail::interface_iarchive<json_iarchive_impl>;

  // direct
  void load(std::string& s);
  void load(boost::serialization::item_version_type& v);
  // float
  template <class T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
  void load(T& t)
  {
    t = get_data();
  }
  // int
  template <class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
  void load(T& t)
  {
    t = get_data();
  }
  // long
  template <class T,
            typename std::enable_if<!std::is_integral<T>::value, int>::type         = 0,
            typename std::enable_if<!std::is_floating_point<T>::value, int>::type   = 0,
            typename std::enable_if<std::is_convertible<T, long>::value, int>::type = 0>
  void load(T& t)
  {
    t = get_data();
  }
  void load_start(const char* name);
  void load_end(const char* name);
  void load_preamble();
  void array_item(int number);

  //
  void load_override(boost::serialization::collection_size_type& t);
  void load_override(const boost::serialization::nvp<boost::serialization::collection_size_type>& t);
  // load primitive
  template <class T>
  void load_override(T& t);
  template <class T>
  void load_override(T* t)
  {
    base_class::load_override(t);
  }
  // NVP(class)
  template <class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
  void load_override(const boost::serialization::nvp<T>& t)
  {
    if(!t.name())
    {
      if(class_id_ && class_id_type_ != ClassIDType::Optional)
      {
        if(object_id_)
        {
          archive_.push_obj_repository(*class_id_, *object_id_);
          base_class::load_override(t.value());
          archive_.pop_to_mark();
          clear();
          return;
        }
        else
        {
          base_class::load_override(t.value());
          clear();
          return;
        }
      }
      base_class::load_override(t.value());
      clear();
    }

    load_start(t.name());
    if(!archive_.get_data().is_null())
    {
      base_class::load_override(t.value());
    }
    load_end(t.name());

    clear();
  }
  // NVP(non-class)
  template <class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
  void load_override(const boost::serialization::nvp<T>& t)
  {
    load_start(t.name());
    if(!archive_.get_data().is_null())
    {
      base_class::load_override(t.value());
    }
    load_end(t.name());
  }
  // ARRAY
  template <class T>
  void load_override(const boost::serialization::array_wrapper<T>& t)
  {
    const size_t sz = t.count();
    for(size_t i = 0; i<sz; i++)
    {
      array_item(i);
      base_class::load_override(t.address()[i]);
      array_end();
    }
  }

  void load_override(class_name_type& t);
  void load_override(version_type& t);
  void load_override(object_id_type& t);
  void load_override(object_reference_type& t);
  void load_override(class_id_type& t);
  void load_override(class_id_optional_type& t);
  void load_override(class_id_reference_type& t);
  void load_override(tracking_type& t);

private:
  DataVector array_stack_;
};
} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::json_iarchive_impl)

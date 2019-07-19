/*********************************************************************
*
* Software License Agreement ()
*
*  Copyright (c) 2019, Greedysky Studio, Inc.
*  All rights reserved.
*
* Author: Greedysky
* Create Date: July 6, 2019
*********************************************************************/
#include "json_iarchive_impl.hpp"

namespace boost {
namespace archive {
json_iarchive_impl::json_iarchive_impl()
  : detail::common_iarchive<json_iarchive_impl>()
  , json_archive_interface()
{
}

void json_iarchive_impl::set_data(Json& data)
{
  root_.swap(data);
  load_data();
}

void json_iarchive_impl::load_data()
{
  archive_.push_class_repository();
  Json& data = archive_.get_data();
  for(auto itr = data.begin(); itr != data.end(); itr++)
  {
    auto& value = *itr;
    ClassInfo& dst = class_map_[std::stoi(itr.key())];
    dst.name_    = value[param::ClassNameType];
    dst.track_   = value[param::TrackingType];
    dst.version_ = value[param::VersionType];
  }
  archive_.pop_data();
}

Json & json_iarchive_impl::get_data()
{
  Json&      data     = archive_.get_data();
  const int  idx      = array_stack_.back().index_;
  const bool is_array = data.is_array();

  if(idx == 0)
  {
    if(is_array)
    {
      return data[0];
    }
  }
  else
  {
    if(is_array)
    {
      return data[idx];
    }
  }
  return data;
}

void json_iarchive_impl::load(std::string& s)
{
  s = get_data();
}

void json_iarchive_impl::load(boost::serialization::item_version_type& v)
{
  uint32_t val;
  load(val);
  v = boost::serialization::item_version_type(val);
}

void json_iarchive_impl::load_start(const char* name)
{
  if(!name)
  {
    return;
  }
  //
  archive_.push_data(name);

  const size_t sz = array_stack_.size();
  if(sz < ++array_level_)
  {
    array_stack_.emplace_back(DataItem {name, 0, false});
  }
  else
  {
    const bool is_array = archive_.get_data().is_array();
    DataItem&  item     = array_stack_.back();
    if(item.key_ == name)
    {
      ++item.index_;
      item.pop_flag_ = true;
      archive_.push_data(item.index_);
    }
    else
    {
      item.index_    = 0;
      item.key_      = name;
      item.pop_flag_ = false;

      if(is_array)
      {
        item.pop_flag_ = true;
        archive_.push_data(item.index_);
      }
    }
  }
}

void json_iarchive_impl::load_end(const char* name)
{
  if(!name)
  {
    return;
  }
  //
  archive_.pop_data();
  if(array_stack_[--array_level_].pop_flag_)
  {
    archive_.pop_data();
  }
  array_stack_.resize(array_level_ + 1);
}

void json_iarchive_impl::load_preamble()
{
}

void json_iarchive_impl::array_item(int number)
{
  archive_.push_data(number);
  array_stack_.resize(array_level_);
}

void json_iarchive_impl::load_override(const boost::serialization::nvp<boost::serialization::collection_size_type>& t)
{
  load_override(t.value());
}

void json_iarchive_impl::load_override(class_name_type& t)
{
  if(!class_id_)
  {
    return;
  }

  auto itr = class_map_.find(*class_id_);
  if(itr == class_map_.end())
  {
    return;
  }
  std::strcpy((char *)t, itr->second.name_.c_str());
}

void json_iarchive_impl::load_override(version_type& t)
{
  if(!class_id_)
  {
    return;
  }

  uint32_t version = 0;
  auto itr = class_map_.find(*class_id_);
  if(itr != class_map_.end())
  {
    version = itr->second.version_;
  }
  t = version_type(version);
}

void json_iarchive_impl::load_override(boost::serialization::collection_size_type& t)
{
  Json&    data = archive_.get_data();
  uint32_t value;
  if(data.is_array())
  {
    value = data.size();
  }
  else
  {
    value = data["count"];
  }
  t = boost::serialization::collection_size_type(value);
}

void json_iarchive_impl::load_override(object_id_type& t)
{
  Json& data = archive_.get_data()[param::ObjectIdType];
  if(!data.is_number())
  {
    object_reference_type r(object_id_type(0));
    load_override(r);
    t = object_id_type(r);
    return;
  }

  object_id_      = data;
  object_id_type_ = ObjectIDType::Normal;
  t               = object_id_type(*object_id_);
}

void json_iarchive_impl::load_override(object_reference_type& t)
{
  Json& data = archive_.get_data()[param::ObjectReferenceType];
  if(!data.is_number())
  {
    return;
  }

  object_id_      = data;
  object_id_type_ = ObjectIDType::Reference;
  t               = object_reference_type(object_id_type(int(*class_id_)));
}

void json_iarchive_impl::load_override(class_id_type& t)
{
  Json& data = archive_.get_data()[param::ClassIdType];
  if(!data.is_number())
  {
    class_id_reference_type r(class_id_type(0));
    load_override(r);
    t = class_id_type(int(r));
    return;
  }

  class_id_      = data;
  class_id_type_ = ClassIDType::Normal;
  object_id_     = boost::none;
  t              = class_id_type(int(*class_id_));
}

void json_iarchive_impl::load_override(class_id_optional_type& t)
{
  Json& data = get_data()[param::ClassIdOptionalType];
  if(!data.is_number())
  {
    return;
  }

  class_id_      = data;
  class_id_type_ = ClassIDType::Optional;
  object_id_     = boost::none;
  t              = class_id_optional_type(class_id_type(int(*class_id_)));
}

void json_iarchive_impl::load_override(class_id_reference_type& t)
{
  Json& data = archive_.get_data()[param::ClassIdReferenceType];
  if(!data.is_number())
  {
    return;
  }

  class_id_      = data;
  class_id_type_ = ClassIDType::Reference;
  object_id_     = boost::none;
  t              = class_id_reference_type(class_id_type(int(*class_id_)));
}

void json_iarchive_impl::load_override(tracking_type& t)
{
  if(!class_id_)
  {
    return;
  }

  uint32_t track = 0;
  auto itr = class_map_.find(*class_id_);
  if(itr != class_map_.end())
  {
    track = itr->second.track_;
  }
  t = tracking_type(track);
}
}
}

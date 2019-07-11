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
#include "json_oarchive_impl.hpp"

namespace boost {
namespace archive {
json_oarchive_impl::json_oarchive_impl()
  : detail::common_oarchive<json_oarchive_impl>()
  , json_archive_interface()
{
}

Json & json_oarchive_impl::get_json_data()
{
  archive_.push_data(param::ClassRepository.c_str());
  for(auto& item : class_map_)
  {
    archive_.push_data(std::to_string(item.first).c_str());
    Json& data = archive_.get_data();
    data[param::ClassNameType] = item.second.name_;
    data[param::VersionType]   = item.second.version_;
    data[param::TrackingType]  = item.second.track_;
    archive_.pop_data();
  }
  archive_.pop_data();
  return root_;
}

void json_oarchive_impl::write_data(Json& data, const Json& input)
{
  if(data.is_null())
  {
    data = input;
  }
  else if(data.is_array())
  {
    data.push_back(input);
  }
  else
  {
    Json temp;
    temp.swap(data);
    data.push_back(temp);
    data.push_back(input);
  }
}

void json_oarchive_impl::save_impl(const char* v)
{
  write_data(archive_.get_data(), Json(v));
}

void json_oarchive_impl::save_impl(double v)
{
  write_data(archive_.get_data(), Json(v));
}

void json_oarchive_impl::save_impl(long v)
{
  write_data(archive_.get_data(), Json(v));
}

void json_oarchive_impl::save_impl(bool v)
{
  write_data(archive_.get_data(), Json(v));
}

void json_oarchive_impl::save_impl(std::nullptr_t v)
{
  write_data(archive_.get_data(), Json());
}

void json_oarchive_impl::save_impl(const std::string& v)
{
  write_data(archive_.get_data(), Json(v));
}

void json_oarchive_impl::save_start(const char* name)
{
  if(!name)
  {
    return;
  }
  //
  archive_.push_data(name);

  Json&        data = archive_.get_data();
  const size_t sz   = array_stack_.size();
  if(sz < ++array_level_)
  {
    array_stack_.emplace_back(std::make_pair(name, false));
    data = Json();
  }
  else
  {
    if(array_stack_.back().first != name)
    {
      array_stack_.back().first = name;
      data = Json();
    }
    else
    {
      if(!data.is_array())
      {
        array_stack_.back().second = true;

        Json temp;
        temp.swap(data);
        data.push_back(temp);
      }
      archive_.push_data(data.size());
    }
  }
}

void json_oarchive_impl::save_end(const char* name)
{
  if(!name)
  {
    return;
  }
  //
  archive_.pop_data();
  if(array_stack_[--array_level_].second)
  {
    archive_.pop_data();
  }
  array_stack_.resize(array_level_ + 1);
}

void json_oarchive_impl::array_item(int number)
{
  archive_.push_data(number);
  array_stack_.resize(array_level_);
}

void json_oarchive_impl::end_preamble()
{
}

void json_oarchive_impl::save_binary(void* address, size_t count)
{
}

void json_oarchive_impl::save_override(const class_name_type& t)
{
  std::string name(t.t);
  class_info_.name_ = name;
}

void json_oarchive_impl::save_override(const version_type& t)
{
  class_info_.version_ = uint32_t(t);
}

void json_oarchive_impl::save_override(const object_id_type& t)
{
  uint32_t value(t);
  object_id_      = value;
  object_id_type_ = ObjectIDType::Normal;
  save_override(boost::serialization::make_nvp(param::ObjectIdType.c_str(), value));
}

void json_oarchive_impl::save_override(const object_reference_type& t)
{
  uint32_t value(t);
  object_id_      = value;
  object_id_type_ = ObjectIDType::Reference;
  save_override(boost::serialization::make_nvp(param::ObjectReferenceType.c_str(), value));
}

void json_oarchive_impl::save_override(const class_id_type& t)
{
  uint32_t value(t);
  class_id_      = value;
  object_id_     = boost::none;
  class_id_type_ = ClassIDType::Normal;
  save_override(boost::serialization::make_nvp(param::ClassIdType.c_str(), value));
}

void json_oarchive_impl::save_override(const class_id_optional_type& t)
{
  uint32_t value(t);
  class_id_      = value;
  class_id_type_ = ClassIDType::Optional;
  save_override(boost::serialization::make_nvp(param::ClassIdOptionalType.c_str(), value));
}

void json_oarchive_impl::save_override(const class_id_reference_type& t)
{
  uint32_t value(t);
  class_id_      = value;
  class_id_type_ = ClassIDType::Reference;
  save_override(boost::serialization::make_nvp(param::ClassIdReferenceType.c_str(), value));
}

void json_oarchive_impl::save_override(const tracking_type& t)
{
  class_info_.track_ = uint32_t(t);
}
}
}

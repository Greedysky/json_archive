/***************************************************************************
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * Use, modification and distribution is subject to the Boost Software
 * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt).
 ***************************************************************************/
#pragma once
#include "json.hpp"
#include <unordered_map>
#include <boost/optional.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/impl/archive_serializer_map.ipp>

namespace boost {
namespace archive {
using Json = nlohmann::json;

namespace param {
const std::string ClassNameType("class_name");
const std::string VersionType("version");
const std::string ItemVersionType("item_version");
const std::string ObjectIdType("object_id");
const std::string ObjectReferenceType("object_id_ref");
const std::string ClassIdType("class_id");
const std::string ClassIdOptionalType("class_id_opt");
const std::string ClassIdReferenceType("class_id_ref");
const std::string TrackingType("tracking_level");
const std::string ObjRepository("__obj_repository__");
const std::string ClassRepository("__class_repository__");
}

struct ClassInfo
{
  std::string name_;
  int track_;
  int version_;

  void clear()
  {
    name_.clear();
    track_   = 0;
    version_ = 0;
  }
};

using DataStack = std::vector<Json *>;
using DataMark  = std::vector<int>;
using ClassMap  = std::unordered_map<uint32_t, ClassInfo>;
using DataID    = boost::optional<uint32_t>;

enum class ClassIDType {
  Normal,
  Optional,
  Reference
};

enum class ObjectIDType {
  Normal,
  Reference
};

class json_archive
{
public:
  json_archive(Json& repository);
  void push_class_repository();
  void push_obj_repository(const uint32_t class_id, const uint32_t obj_id);
  void pop_to_mark();

  void         push_data(const char* name);
  void         push_data(const int number);
  void         pop_data();
  const Json & get_data() const;
  Json &       get_data();

private:
  DataStack data_stack_;
  DataMark  data_mark_;
  Json*     obj_repository_;
};


class json_archive_interface
{
public:
  json_archive_interface();

protected:
  void clear();
  void array_end();

protected:
  Json         root_;
  json_archive archive_;
  DataID       class_id_;
  DataID       object_id_;
  ClassIDType  class_id_type_;
  ObjectIDType object_id_type_;

  int      array_level_;
  ClassMap class_map_;
};
}
}

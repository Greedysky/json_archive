#include "json_archive.hpp"

namespace boost {
namespace archive {
json_archive::json_archive(Json& repository)
    : obj_repository_(&repository[param::ObjRepository])
{
    data_stack_.push_back(&repository);
}

void json_archive::push_class_repository()
{
    data_mark_.push_back(data_stack_.size());
    push_data(param::ClassRepository.c_str());
}

void json_archive::push_obj_repository(const uint32_t class_id, const uint32_t obj_id)
{
    data_mark_.push_back(data_stack_.size());
    data_stack_.push_back(obj_repository_);

    push_data(std::to_string(class_id).c_str());
    push_data(std::to_string(obj_id).c_str());
}

void json_archive::pop_to_mark()
{
    data_stack_.resize(data_mark_.back());
    data_mark_.pop_back();
}

void json_archive::push_data(const char* name)
{
    data_stack_.push_back(&get_data()[name]);
}

void json_archive::push_data(const int number)
{
    Json& target = get_data();
    if(!target.is_array())
    {
        target = Json();
    }
    data_stack_.push_back(&target[number]);
}

void json_archive::pop_data()
{
    data_stack_.pop_back();
}

const Json& json_archive::get_data() const
{
    return *data_stack_.back();
}

Json & json_archive::get_data()
{
    return *data_stack_.back();
}


json_archive_interface::json_archive_interface()
    : archive_(root_)
    , array_level_(0)
{
    clear();
}

void json_archive_interface::clear()
{
    class_id_ = boost::none;
    object_id_ = boost::none;
}

void json_archive_interface::array_end()
{
    archive_.pop_data();
}
}
}

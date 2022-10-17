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
#include "json_oarchive.hpp"
#include "json_iarchive.hpp"

#include <sstream>
#include <iostream>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

class Pos
{
public:
  Pos()
    : x_(0)
    , y_(0)
  {
  }

  Pos(int x, int y)
    : x_(x)
    , y_(y)
  {
  }

private:
  int x_;
  int y_;

private:
  friend class boost::serialization::access;
  template <typename Archive>
  inline void serialize(Archive& ar, unsigned)
  {
    ar
    & BOOST_SERIALIZATION_NVP(x_)
    & BOOST_SERIALIZATION_NVP(y_);
  }
};


class Test
{
public:
  Test()
    : struct_(3, 4)
    , int_(1)
    , bool_(true)
    , double_(3.14)
    , string_("Test")
    , int_array_({1, 1, 1})
    , struct_array_({Pos(0, 0), Pos(1, 1), Pos(2, 2)})
  {
  }

private:
  friend class boost::serialization::access;
  template <typename Archive>
  inline void serialize(Archive& ar, unsigned)
  {
    ar
    & BOOST_SERIALIZATION_NVP(struct_)
    & BOOST_SERIALIZATION_NVP(int_)
    & BOOST_SERIALIZATION_NVP(bool_)
    & BOOST_SERIALIZATION_NVP(double_)
    & BOOST_SERIALIZATION_NVP(string_)
    & BOOST_SERIALIZATION_NVP(int_array_)
    & BOOST_SERIALIZATION_NVP(struct_array_);
  }

  Pos         struct_;
  int         int_;
  bool        bool_;
  double      double_;
  std::string string_;

  std::vector<int> int_array_;
  std::vector<Pos> struct_array_;
};

class TestA : public Test
{
public:
  TestA()
    : Test()
    , hdr_(boost::make_shared<Pos>(6, 6))
  {
  }

private:
  friend class boost::serialization::access;
  template <typename Archive>
  inline void serialize(Archive& ar, unsigned)
  {
    ar
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Test)
    & BOOST_SERIALIZATION_NVP(hdr_);
  }

  boost::shared_ptr<Pos> hdr_;
};


int main()
{
  std::stringstream stream;
  {
    {
      TestA test;
      boost::archive::json_oarchive oa(stream);
      oa << BOOST_SERIALIZATION_NVP(test);
      std::cout << stream.str() << std::endl;
    }

    {
      TestA test;
      boost::archive::json_iarchive ia(stream);
      ia >> BOOST_SERIALIZATION_NVP(test);
    }
  }
}

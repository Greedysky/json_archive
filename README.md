Json Archive
===========
Json archive(input/output) for boost::serialization.

## Build Dependencies
* boost 1.64.0 or newer
* json
* g++ (c++11)

## Build
```bash
  $ cmake .
  $ make
```

## Usage
```c++
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

    Pos struct_;
    int int_;
    bool bool_;
    double double_;
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
```

## License
BSL-1.0 License

## Copyright
 * This file is part of the Json Archive project.
 * Copyright (C) 2015 - 2025 Greedysky Studio.
 * Mail: greedysky@163.com.

## How To Contribute
 * Fork this project on github and make a branch. Commit in that branch, and push, then create a pull request to be reviewed and merged.
 * Create an issue if you have any problem when using project or you find a bug, etc.
 * What you can do: translation, write document, wiki, find or fix bugs, give your idea for this project etc.
 * If you want to join the project developed together, please send e-mail to me.

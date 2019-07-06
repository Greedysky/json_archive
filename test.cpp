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

class Poss
{
public:
  Poss()
  {
  }

  Poss(int x, int y)
  {
    this->x = x;
    this->y = y;
  }
  int x;
  int y;
private:
  friend class boost::serialization::access;

  template <typename Archive> void serialize(Archive& ar, unsigned)
  {
    ar & BOOST_SERIALIZATION_NVP(x)
    & BOOST_SERIALIZATION_NVP(y);
  }
};

class Animal {
public:
  Animal()
  {
    legs      = 0;
    is_mammal = false;
  }
  virtual void Sleep() = 0;
  void set_leg(int l)
  {
    legs = l;
  }
  void set_name(std::string s)
  {
    name = s;
  }
  void set_ismammal(bool b)
  {
    is_mammal = b;
  }
  void set_myposs(int x, int y)
  {
    my_poss.x = x;
    my_poss.y = y;
  }
  void set_array(int x, int y, int z)
  {
    array.push_back(x);
    array.push_back(y);
    array.push_back(z);
  }
  void set_poss_array(int x, int y)
  {
    poss_array.push_back(Poss(x, y));
    poss_array.push_back(Poss(x, y));
    poss_array.push_back(Poss(x, y));
  }

  void print();

private:
  friend class boost::serialization::access;

  template <typename Archive> void serialize(Archive& ar, unsigned)
  {
    ar & BOOST_SERIALIZATION_NVP(my_poss)
    & BOOST_SERIALIZATION_NVP(legs)
    & BOOST_SERIALIZATION_NVP(is_mammal)
    & BOOST_SERIALIZATION_NVP(array)
    & BOOST_SERIALIZATION_NVP(poss_array)
    & BOOST_SERIALIZATION_NVP(name);
  }

  Poss              my_poss;
  int               legs;
  bool              is_mammal;
  std::vector<int>  array;
  std::vector<Poss> poss_array;
  std::string       name;
};



class Dog : public Animal
{
public:

  Dog()
  {
    poss = boost::make_shared<Poss>(0, 0);
    run  = false;
  }

  void set_run(bool b)
  {
    run = b;
  }

  void set_poss(int x, int y)
  {
    poss->x = x;
    poss->y = y;
  }

  void pposs()
  {
    std::cout << run << poss->x << poss->y;
  }

  virtual void Sleep() override
  {
  }

private:
  friend class boost::serialization::access;

  template <typename Archive> void serialize(Archive& ar, unsigned)
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Animal)
    & BOOST_SERIALIZATION_NVP(run)
    & BOOST_SERIALIZATION_NVP(poss);
  }
  bool                    run;
  boost::shared_ptr<Poss> poss;
};

void Animal::print()
{
  std::cout << name << legs << is_mammal << my_poss.x << my_poss.y;
}

int main()
{
  std::stringstream stream;
  {
    Dog animal;
    animal.set_name("Horse");
    animal.set_leg(4);
    animal.set_ismammal(true);
    animal.set_run(true);
    animal.set_myposs(2, 2);
    animal.set_poss(11, 11);
    animal.set_array(9, 8, 7);
    animal.set_poss_array(9, 8);

    boost::archive::json_oarchive oa(stream);
    oa << BOOST_SERIALIZATION_NVP(animal);
    std::cout << stream.str() << std::endl;

    {
      Dog                           animal;
      boost::archive::json_iarchive ia(stream);
      ia >> BOOST_SERIALIZATION_NVP(animal);
      animal.print();
    }
  }
}

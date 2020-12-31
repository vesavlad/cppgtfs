// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_TRIP_H_
#define AD_CPPGTFS_GTFS_TRIP_H_

#include <cppgtfs/gtfs/Frequency.h>
#include <cppgtfs/gtfs/Route.h>
#include <cppgtfs/gtfs/Service.h>
#include <cppgtfs/gtfs/Shape.h>
#include <cppgtfs/gtfs/Stop.h>
#include <cppgtfs/gtfs/StopTime.h>
#include <cppgtfs/gtfs/flat/Trip.h>

#include <cstdint>
#include <algorithm>
#include <set>
#include <string>

namespace ad::cppgtfs::gtfs {

template <typename StopTimeT, typename ServiceT, typename RouteT, typename ShapeT>
class TripB {
  // typedef std::set<StopTimeT, StopTimeCompare<StopTimeT>> StopTimes;
  typedef std::vector<StopTimeT> StopTimes;
  typedef std::vector<Frequency> Frequencies;

 public:
  typedef TripB<StopTimeT, ServiceT, RouteT, ShapeT>* Ref;
  static std::string getId(Ref r) { return r->getId(); }

  typedef flat::Trip::WC_BIKE_ACCESSIBLE WC_BIKE_ACCESSIBLE;
  typedef flat::Trip::DIRECTION DIRECTION;

  TripB() {}
  TripB(const std::string& id, typename RouteT::Ref r, typename ServiceT::Ref s,
        const std::string& hs, const std::string& short_name, DIRECTION dir,
        const std::string& blockid, typename ShapeT::Ref shp,
        WC_BIKE_ACCESSIBLE wc, WC_BIKE_ACCESSIBLE ba);

  const std::string& getId() const;
  const typename RouteT::Ref getRoute() const;
  typename RouteT::Ref getRoute();
  typename ServiceT::Ref getService();
  const typename ServiceT::Ref getService() const;
  const std::string& getHeadsign() const;
  const std::string& getShortname() const;
  DIRECTION getDirection() const;
  const std::string& getBlockId() const;
  const typename ShapeT::Ref getShape() const;
  typename ShapeT::Ref getShape();
  void setShape(typename ShapeT::Ref shp);
  WC_BIKE_ACCESSIBLE getWheelchairAccessibility() const;
  WC_BIKE_ACCESSIBLE getBikesAllowed() const;
  const StopTimes& getStopTimes() const;
  StopTimes& getStopTimes();
  Frequencies& getFrequencies();
  const Frequencies& getFrequencies() const;
  bool addStopTime(const StopTimeT& t);
  void addFrequency(const Frequency& t);

  gtfs::flat::Trip getFlat() const {
    return gtfs::flat::Trip{
        _id,       RouteT::getId(_route), ServiceT::getId(_service),
        _headsign, _short_name,           _dir,
        _block_id, ShapeT::getId(_shape), _wc,
        _ba};
  };

 private:
  std::string _id;
  typename RouteT::Ref _route;
  typename ServiceT::Ref _service;
  std::string _headsign;
  std::string _short_name;
  DIRECTION _dir;
  std::string _block_id;
  typename ShapeT::Ref _shape;
  WC_BIKE_ACCESSIBLE _wc;
  WC_BIKE_ACCESSIBLE _ba;

  StopTimes _stoptimes;
  Frequencies _frequencies;
};

typedef TripB<StopTime<Stop>, Service, Route, Shape> Trip;

#include <cppgtfs/gtfs/Trip.tpp>

}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_TRIP_H_

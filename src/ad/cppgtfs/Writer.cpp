// Copyright 2018, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <boost/filesystem.hpp>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include "Writer.h"
#include "ad/util/CsvWriter.h"
#include "gtfs/Shape.h"
#include "gtfs/Trip.h"

using ad::util::CsvWriter;
using ad::cppgtfs::Writer;
using ad::cppgtfs::gtfs::Agency;
using ad::cppgtfs::gtfs::Stop;
using ad::cppgtfs::gtfs::Route;
using ad::cppgtfs::gtfs::Trip;
using ad::cppgtfs::gtfs::Service;
using ad::cppgtfs::gtfs::ServiceDate;
using ad::cppgtfs::gtfs::StopTime;
using ad::cppgtfs::gtfs::Shape;
using ad::cppgtfs::gtfs::ShapePoint;
using ad::cppgtfs::gtfs::Time;

// ____________________________________________________________________________
bool Writer::write(gtfs::Feed* sourceFeed, std::string path) const {
  std::ofstream fs;
  boost::filesystem::path gtfsPath(path);
  boost::filesystem::path curFile;

  curFile = gtfsPath / "shapes.txt";
  fs.open(curFile.c_str());
  writeShapes(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "trips.txt";
  fs.open(curFile.c_str());
  writeTrips(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "agencies.txt";
  fs.open(curFile.c_str());
  writeAgencies(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "stops.txt";
  fs.open(curFile.c_str());
  writeStops(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "stop_times.txt";
  fs.open(curFile.c_str());
  writeStopTimes(sourceFeed, &fs);
  fs.close();

  return true;
}

// ____________________________________________________________________________
bool Writer::writeAgencies(gtfs::Feed* sourceFeed, std::ostream* s) const {
  CsvWriter csvw(
      s, {"agency_id", "agency_name", "agency_url", "agency_timezone",
          "agency_lang", "agency_phone", "agency_fare_url", "agency_email"});

  for (const auto& t : sourceFeed->getAgencies()) {
    csvw.writeString(t.second->getId());
    csvw.writeString(t.second->getName());
    csvw.writeString(t.second->getUrl());
    csvw.writeString(t.second->getTimezone());
    csvw.writeString(t.second->getLang());
    csvw.writeString(t.second->getPhone());
    csvw.writeString(t.second->getFareUrl());
    csvw.writeString(t.second->getAgencyEmail());
    csvw.flushLine();
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeStops(gtfs::Feed* sourceFeed, std::ostream* s) const {
  CsvWriter csvw(
      s, {"stop_id", "stop_code", "stop_name", "stop_desc", "stop_lat",
          "stop_lon", "zone_id", "stop_url", "location_type", "parent_station",
          "stop_timezone", "wheelchair_boarding"});

  for (const auto& t : sourceFeed->getStops()) {
    csvw.writeString(t.second->getId());
    csvw.writeString(t.second->getCode());
    csvw.writeString(t.second->getName());
    csvw.writeString(t.second->getDesc());
    csvw.writeDouble(t.second->getLat());
    csvw.writeDouble(t.second->getLng());
    csvw.writeString(t.second->getZoneId());
    csvw.writeString(t.second->getStopUrl());
    csvw.writeInt(t.second->getLocationType());
    if (t.second->getParentStation()) csvw.writeString(t.second->getParentStation()->getId());
    else csvw.skip();
    csvw.writeString(t.second->getStopTimezone());
    csvw.writeInt(t.second->getWheelchairBoarding());
    csvw.flushLine();
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeTrips(gtfs::Feed* sourceFeed, std::ostream* s) const {
  CsvWriter csvw(s, {"route_id", "service_id", "trip_id", "trip_headsign",
                     "trip_short_name", "direction_id", "block_id", "shape_id",
                     "wheelchair_accessible", "bikes_allowed"});

  for (const auto& t : sourceFeed->getTrips()) {
    csvw.writeString(t.second->getRoute()->getId());
    csvw.writeString(t.second->getService()->getId());
    csvw.writeString(t.second->getId());
    csvw.writeString(t.second->getHeadsign());
    csvw.writeString(t.second->getShortname());
    if (t.second->getDirection() < 2) csvw.writeInt(t.second->getDirection());
    else csvw.skip();
    csvw.writeString(t.second->getBlockId());
    csvw.writeString(t.second->getShape()->getId());
    csvw.writeInt(t.second->getWheelchairAccessibility());
    csvw.writeInt(t.second->getBikesAllowed());
    csvw.flushLine();
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeStopTimes(gtfs::Feed* sourceFeed, std::ostream* s) const {
  CsvWriter csvw(s, {"trip_id", "arrival_time", "departure_time",
                     "stop_id", "stop_sequence", "stop_headsign",
                     "pickup_type", "drop_off_type", "shape_dist_traveled", "timepoint"});

  for (const auto& t : sourceFeed->getTrips()) {
    for (const auto& p : t.second->getStopTimes()) {
      csvw.writeString(t.second->getId());
      csvw.writeString(p.getArrivalTime().toString());
      csvw.writeString(p.getDepartureTime().toString());
      csvw.writeString(p.getStop()->getId());
      csvw.writeInt(p.getSeq());
      csvw.writeString(p.getHeadsign());
      csvw.writeInt(p.getPickupType());
      csvw.writeInt(p.getDropOffType());
      if (p.getShapeDistanceTravelled() > -.5) csvw.writeInt(p.getDropOffType());
      else csvw.skip();
      csvw.writeInt(p.isTimepoint());
      csvw.flushLine();
    }
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeShapes(gtfs::Feed* sourceFeed, std::ostream* s) const {
  CsvWriter csvw(s, {"shape_id", "shape_pt_lat", "shape_pt_lon",
                     "shape_pt_sequence", "shape_dist_traveled"});

  for (const auto& t : sourceFeed->getShapes()) {
    for (const auto& p : t.second->getPoints()) {
      csvw.writeString(t.second->getId());
      csvw.writeDouble(p.lat);
      csvw.writeDouble(p.lng);
      csvw.writeInt(p.seq);
      if (p.travelDist > -0.5) csvw.writeDouble(p.travelDist);
      else csvw.skip();
      csvw.flushLine();
    }
  }

  return true;
}

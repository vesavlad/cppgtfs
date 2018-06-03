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
  if (!fs.good()) cannotWrite(curFile);
  writeShapes(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "trips.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeTrips(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "agency.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeAgencies(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "stops.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeStops(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "stop_times.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeStopTimes(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "routes.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeRoutes(sourceFeed, &fs);
  fs.close();

  if (!sourceFeed->getPublisherUrl().empty() &&
      !sourceFeed->getPublisherName().empty()) {
    curFile = gtfsPath / "feed_info.txt";
    fs.open(curFile.c_str());
    if (!fs.good()) cannotWrite(curFile);
    writeFeedInfo(sourceFeed, &fs);
    fs.close();
  }

  curFile = gtfsPath / "transfers.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeTransfers(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "calendar.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeCalendar(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "calendar_dates.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeCalendarDates(sourceFeed, &fs);
  fs.close();

  curFile = gtfsPath / "frequencies.txt";
  fs.open(curFile.c_str());
  if (!fs.good()) cannotWrite(curFile);
  writeFrequencies(sourceFeed, &fs);
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
          "stop_timezone", "wheelchair_boarding", "platform_code"});

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
    if (t.second->getParentStation())
      csvw.writeString(t.second->getParentStation()->getId());
    else
      csvw.skip();
    csvw.writeString(t.second->getStopTimezone());
    csvw.writeInt(t.second->getWheelchairBoarding());
    csvw.writeString(t.second->getPlatformCode());
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
    if (t.second->getDirection() < 2)
      csvw.writeInt(t.second->getDirection());
    else
      csvw.skip();
    csvw.writeString(t.second->getBlockId());
    if (t.second->getShape())
      csvw.writeString(t.second->getShape()->getId());
    else
      csvw.skip();
    csvw.writeInt(t.second->getWheelchairAccessibility());
    csvw.writeInt(t.second->getBikesAllowed());
    csvw.flushLine();
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeStopTimes(gtfs::Feed* sourceFeed, std::ostream* s) const {
  CsvWriter csvw(s, {"trip_id", "arrival_time", "departure_time", "stop_id",
                     "stop_sequence", "stop_headsign", "pickup_type",
                     "drop_off_type", "shape_dist_traveled", "timepoint"});

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
      if (p.getShapeDistanceTravelled() > -.5)
        csvw.writeDouble(p.getShapeDistanceTravelled());
      else
        csvw.skip();
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
      if (p.travelDist > -0.5)
        csvw.writeDouble(p.travelDist);
      else
        csvw.skip();
      csvw.flushLine();
    }
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeRoutes(gtfs::Feed* sourceFeed, std::ostream* s) const {
  CsvWriter csvw(s, {"route_id", "agency_id", "route_short_name",
                     "route_long_name", "route_desc", "route_type", "route_url",
                     "route_color", "route_text_color"});

  for (const auto& r : sourceFeed->getRoutes()) {
    csvw.writeString(r.second->getId());
    if (r.second->getAgency())
      csvw.writeString(r.second->getAgency()->getId());
    else
      csvw.skip();
    csvw.writeString(r.second->getShortName());
    csvw.writeString(r.second->getLongName());
    csvw.writeString(r.second->getDesc());
    csvw.writeInt(r.second->getType());
    csvw.writeString(r.second->getUrl());
    csvw.writeString(r.second->getColorString());
    csvw.writeString(r.second->getTextColorString());
    csvw.flushLine();
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeFeedInfo(gtfs::Feed* f, std::ostream* os) const {
  CsvWriter csvw(os, {"feed_publisher_name", "feed_publisher_url", "feed_lang",
                      "feed_start_date", "feed_end_date", "feed_version"});

  csvw.writeString(f->getPublisherName());
  csvw.writeString(f->getPublisherUrl());
  csvw.writeString(f->getLang());
  if (!f->getStartDate().empty())
    csvw.writeInt(f->getStartDate().getYYYYMMDD());
  else
    csvw.skip();
  if (!f->getEndDate().empty())
    csvw.writeInt(f->getEndDate().getYYYYMMDD());
  else
    csvw.skip();
  csvw.writeString(f->getVersion());
  csvw.flushLine();

  return true;
}

// ____________________________________________________________________________
bool Writer::writeTransfers(gtfs::Feed* f, std::ostream* os) const {
  // TODO

  return true;
}

// ____________________________________________________________________________
bool Writer::writeCalendar(gtfs::Feed* f, std::ostream* os) const {
  CsvWriter csvw(
      os, {"service_id", "monday", "tuesday", "wednesday", "thursday", "friday",
           "saturday", "sunday", "start_date", "end_date"});

  for (const auto& r : f->getServices()) {
    if (!r.second->hasServiceDays()) continue;
    csvw.writeString(r.second->getId());
    csvw.writeInt((bool)(r.second->getServiceDates() & gtfs::Service::MONDAYS));
    csvw.writeInt(
        (bool)(r.second->getServiceDates() & gtfs::Service::TUESDAYS));
    csvw.writeInt(
        (bool)(r.second->getServiceDates() & gtfs::Service::WEDNESDAYS));
    csvw.writeInt(
        (bool)(r.second->getServiceDates() & gtfs::Service::THURSDAYS));
    csvw.writeInt((bool)(r.second->getServiceDates() & gtfs::Service::FRIDAYS));
    csvw.writeInt(
        (bool)(r.second->getServiceDates() & gtfs::Service::SATURDAYS));
    csvw.writeInt((bool)(r.second->getServiceDates() & gtfs::Service::SUNDAYS));
    csvw.writeInt(r.second->getBeginDate().getYYYYMMDD());
    csvw.writeInt(r.second->getEndDate().getYYYYMMDD());
    csvw.flushLine();
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeCalendarDates(gtfs::Feed* f, std::ostream* os) const {
  CsvWriter csvw(os, {"service_id", "date", "exception_type"});

  for (const auto& r : f->getServices()) {
    for (const auto& e : r.second->getExceptions()) {
      csvw.writeString(r.second->getId());
      csvw.writeInt(e.first.getYYYYMMDD());
      csvw.writeInt(e.second);
      csvw.flushLine();
    }
  }

  return true;
}

// ____________________________________________________________________________
bool Writer::writeFrequencies(gtfs::Feed* f, std::ostream* os) const {
  CsvWriter csvw(
      os, {"trip_id", "start_time", "end_time", "headway_secs", "exact_times"});

  for (const auto& t : f->getTrips()) {
    for (const auto& f : t.second->getFrequencies()) {
      csvw.writeString(t.second->getId());
      csvw.writeString(f.getStartTime().toString());
      csvw.writeString(f.getEndTime().toString());
      csvw.writeInt(f.getHeadwaySecs());
      csvw.writeInt(f.hasExactTimes());
      csvw.flushLine();
    }
  }
}

// ___________________________________________________________________________
void Writer::cannotWrite(boost::filesystem::path file) const {
  throw WriterException("Could not write to file.", std::string(file.c_str()));
}

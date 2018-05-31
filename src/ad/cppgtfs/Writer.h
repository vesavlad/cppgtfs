// Copyright 2018, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_WRITER_H_
#define AD_CPPGTFS_WRITER_H_

#include <stdint.h>
#include <boost/filesystem.hpp>
#include <exception>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "ad/util/CsvWriter.h"
#include "gtfs/Feed.h"

using std::string;
using ad::util::CsvWriter;

// A GTFS writer

namespace ad {
namespace cppgtfs {

class WriterException : public std::exception {
 public:
  WriterException(std::string msg, std::string file_name)
      : _msg(msg), _file_name(file_name) {}
  WriterException(std::string msg)
      : _msg(msg), _file_name("?") {}
  ~WriterException() throw() {}

  virtual const char* what() const throw() {
    std::stringstream ss;
    ss << _file_name << ":";
    ss << " " << _msg;
    _what_msg = ss.str();
    return _what_msg.c_str();
  }

 private:
  mutable std::string _what_msg;
  std::string _msg;
  std::string _file_name;
};

class Writer {
 public:
  // Default initialization.
  Writer() {}

  // write a GtfsFeed to a zip/folder
  bool write(gtfs::Feed* sourceFeed, std::string path) const;

 private:
  bool writeAgencies(gtfs::Feed* f, std::ostream* os) const;
  bool writeStops(gtfs::Feed* f, std::ostream* os) const;
  bool writeShapes(gtfs::Feed* f, std::ostream* os) const;
  bool writeTrips(gtfs::Feed* f, std::ostream* os) const;
  bool writeStopTimes(gtfs::Feed* f, std::ostream* os) const;
  bool writeRoutes(gtfs::Feed* f, std::ostream* os) const;
  bool writeFeedInfo(gtfs::Feed* f, std::ostream* os) const;
  bool writeTransfers(gtfs::Feed* f, std::ostream* os) const;
  bool writeCalendar(gtfs::Feed* f, std::ostream* os) const;
  bool writeCalendarDates(gtfs::Feed* f, std::ostream* os) const;
  bool writeFrequencies(gtfs::Feed* f, std::ostream* os) const;
  void cannotWrite(boost::filesystem::path file) const;
};
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_WRITER_H_

// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_TRANSFER_H_
#define AD_CPPGTFS_GTFS_TRANSFER_H_

#include <string>
#include "Stop.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

class Transfer {
 public:
  enum TYPE : uint8_t {
    RECOMMENDED = 0,
    TIMED = 1,
    MIN_TIME = 2,
    NO_TRANSFER = 3
  };
  Transfer() {}

  Transfer(Stop* fromStop, Stop* toStop, TYPE type, int32_t tTime)
      : _fromStop(fromStop), _toStop(toStop), _type(type), _tTime(tTime) {}

  Stop* getFromStop() const { return _fromStop; }

  Stop* getToStop() const { return _toStop; }

  TYPE getType() const { return _type; }

  int32_t getMinTransferTime() const { return _tTime; }

  // TODO(patrick): implement setters

 private:
  Stop* _fromStop;
  Stop* _toStop;
  TYPE _type;
  int32_t _tTime;
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_TRANSFER_H_

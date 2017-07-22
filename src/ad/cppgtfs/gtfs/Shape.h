// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_SHAPE_H_
#define AD_CPPGTFS_GTFS_SHAPE_H_

#include <set>
#include <string>
#include <vector>

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

struct ShapePoint {
  ShapePoint(double lat, double ln, double dist, uint32_t seq)
      : lat(lat), lng(ln), travelDist(dist), seq(seq) {}
  ShapePoint() : lat(0), lng(0), travelDist(-1), seq(0) {}
  double lat, lng, travelDist;
  uint32_t seq;
};

struct ShapePointCompare {
  bool operator()(const ShapePoint& lh, const ShapePoint& rh) const {
    return lh.seq < rh.seq;
  }
};

typedef std::set<ShapePoint, ShapePointCompare> ShapePoints;

class Shape {
 public:
  Shape() {}

  explicit Shape(const string& id) : _id(id) {}

  const std::string& getId() const { return _id; }

  const ShapePoints& getPoints() const { return _shapePoints; }

  bool addPoint(ShapePoint p) { return _shapePoints.insert(p).second; }

 private:
  string _id;
  ShapePoints _shapePoints;
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_SHAPE_H_

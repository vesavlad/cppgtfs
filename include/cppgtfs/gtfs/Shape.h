// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef CPPGTFS_GTFS_SHAPE_H
#define CPPGTFS_GTFS_SHAPE_H

#include <set>
#include <string>
#include <vector>

namespace ad::cppgtfs::gtfs
{

    struct ShapePoint
    {
        ShapePoint(float lat, float ln, float dist, uint32_t seq)
                : lat(lat), lng(ln), travelDist(dist), seq(seq) {}

        ShapePoint() : lat(0), lng(0), travelDist(-1), seq(0) {}

        float lat, lng;
        float travelDist;
        uint32_t seq;
    };

    struct ShapePointCompare
    {
        bool operator()(const ShapePoint &lh, const ShapePoint &rh) const
        {
            return lh.seq < rh.seq;
        }
    };

    using ShapePoints = std::vector<ShapePoint>;

    class Shape
    {
    public:
        using Ref = Shape *;

        static std::string getId(Ref r) { return r->getId(); }

        Shape() = default;

        explicit Shape(const string &id) : _id(id) {}

        const std::string &getId() const { return _id; }

        const ShapePoints &getPoints() const { return _shapePoints; }

        bool addPoint(const ShapePoint &p)
        {
            for (auto & _shapePoint : _shapePoints) {
                if (_shapePoint.seq == p.seq)
                    return false;
            }
            _shapePoints.reserve(_shapePoints.size() + 1);
            _shapePoints.push_back(p);
            auto cmp = ShapePointCompare();
            std::sort(_shapePoints.begin(), _shapePoints.end(), cmp);
            return true;
        }

    private:
        string _id;
        ShapePoints _shapePoints;
    };

}  // namespace ad

#endif // CPPGTFS_GTFS_SHAPE_H

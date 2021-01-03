// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef CPPGTFS_GTFS_ROUTE_H
#define CPPGTFS_GTFS_ROUTE_H

#include <cppgtfs/gtfs/Agency.h>
#include <cppgtfs/gtfs/flat/Route.h>

#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <set>
#include <sstream>
#include <string>
#include <utility>

namespace ad::cppgtfs::gtfs
{

    template<typename AgencyT>
    class RouteB
    {
    public:
        using Ref = RouteB<AgencyT> *;

        static std::string getId(Ref r) { return r->getId(); }

        using TYPE = flat::Route::TYPE;

        RouteB() = default;

        RouteB(std::string id, typename AgencyT::Ref agency,
               std::string short_name, std::string long_name, std::string desc,
               flat::Route::TYPE type, std::string url, uint32_t color,
               uint32_t text_color)
                : _id(std::move(id)),
                  _agency(agency),
                  _short_name(std::move(short_name)),
                  _long_name(std::move(long_name)),
                  _desc(std::move(desc)),
                  _type(type),
                  _url(std::move(url)),
                  _color(color),
                  _text_color(text_color) {}

        const std::string &getId() const { return _id; }

        const typename AgencyT::Ref getAgency() const { return _agency; }

        typename AgencyT::Ref getAgency() { return _agency; }

        const std::string &getShortName() const { return _short_name; }

        const std::string &getLongName() const { return _long_name; }

        const std::string &getDesc() const { return _desc; }

        flat::Route::TYPE getType() const { return _type; }

        const std::string &getUrl() const { return _url; }

        uint32_t getColor() const { return _color; }

        std::string getColorString() const
        {
            return flat::Route::getHexColorString(_color);
        }

        uint32_t getTextColor() const { return _text_color; }

        std::string getTextColorString() const
        {
            return flat::Route::getHexColorString(_text_color);
        }

        flat::Route getFlat() const
        {
            flat::Route r;
            r.id = _id;
            r.agency = _agency->getId();
            r.short_name = _short_name;
            r.long_name = _long_name;
            r.desc = _desc;
            r.type = _type;
            r.url = _url;
            r.color = _color;
            r.text_color = _text_color;
            return r;
        }

        // TODO(patrick): implement setters

    private:
        string _id;
        typename AgencyT::Ref _agency;
        string _short_name;
        string _long_name;
        string _desc;
        flat::Route::TYPE _type;
        string _url;
        uint32_t _color{};
        uint32_t _text_color{};
    };

    using Route = RouteB<Agency>;

}  // namespace ad

#endif // CPPGTFS_GTFS_ROUTE_H

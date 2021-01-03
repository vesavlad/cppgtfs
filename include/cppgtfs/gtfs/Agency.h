// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef CPPGTFS_GTFS_AGENCY_H
#define CPPGTFS_GTFS_AGENCY_H

#include <cppgtfs/gtfs/flat/Agency.h>

#include <string>
#include <utility>

namespace ad::cppgtfs::gtfs
{

    class Agency
    {
    public:
        using Ref = Agency *;

        static std::string getId(Ref r) { return r->getId(); }

        Agency() = default;

        Agency(std::string id, std::string name, std::string url,
               std::string timezone, std::string lang,
               std::string phone, std::string fare_url,
               std::string agency_email)
                : _id(std::move(id)),
                  _name(std::move(name)),
                  _url(std::move(url)),
                  _timezone(std::move(timezone)),
                  _lang(std::move(lang)),
                  _phone(std::move(phone)),
                  _fare_url(std::move(fare_url)),
                  _agency_email(std::move(agency_email)) {}

        Agency(const char *id, const char *name, const char *url,
               const char *timezone, const char *lang, const char *phone,
               const char *fare_url, const char *agency_email)
                : _id(id),
                  _name(name),
                  _url(url),
                  _timezone(timezone),
                  _lang(lang),
                  _phone(phone),
                  _fare_url(fare_url),
                  _agency_email(agency_email) {}

        const std::string &getId() const { return _id; }

        const std::string &getName() const { return _name; }

        const std::string &getUrl() const { return _url; }

        const std::string &getTimezone() const { return _timezone; }

        const std::string &getLang() const { return _lang; }

        const std::string &getPhone() const { return _phone; }

        const std::string &getFareUrl() const { return _fare_url; }

        const std::string &getAgencyEmail() const { return _agency_email; }

        flat::Agency getFlat() const
        {
            flat::Agency r;
            r.id = _id;
            r.name = _name;
            r.url = _url;
            r.timezone = _timezone;
            r.lang = _lang;
            r.phone = _phone;
            r.fare_url = _fare_url;
            r.agency_email = _agency_email;
            return r;
        }

        // TODO(patrick): implement setters

    private:
        std::string _id, _name, _url, _timezone, _lang, _phone, _fare_url,
                _agency_email;
    };

}  // namespace ad

#endif // CPPGTFS_GTFS_AGENCY_H

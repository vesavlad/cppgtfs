// Copyright 2013, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Hannah Bast <bast@informatik.uni-freiburg.de>,
//          Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include "cppgtfs/util/CsvParser.h"

using ad::util::CsvParser;
using std::remove;

// cached first 10 powers of 10
static int pow10[10] = {1,      10,      100,      1000,      10000,
                        100000, 1000000, 10000000, 100000000, 1000000000};

// _____________________________________________________________________________
CsvParser::CsvParser() {}

CsvParser::CsvParser(std::istream* stream) : _curLine(0), _stream(stream) {
  readNextLine();
  parseHeader();
}

// _____________________________________________________________________________
bool CsvParser::readNextLine() {
  if (!_stream->good()) return false;

  // TODO: error message if buffer was too small
  _stream->getline(_buff, 10000);
  size_t readN = _stream->gcount();
  _buff[readN] = 0;

  // Remove new line characters
  while (_buff[readN - 1] == '\r' || _buff[readN - 1] == '\n') {
    _buff[readN - 1] = 0;
    readN--;
  }

  _curLine++;

  if (readN == 0) return readNextLine();
  size_t pos = 0;
  size_t lastPos = pos;
  bool firstChar = false;
  bool esc = false;
  int esc_quotes_found = 0;
  _currentItems.clear();
  _currentModItems.clear();

  if (readN > 2 && static_cast<int>(_buff[0]) == -17 &&
      static_cast<int>(_buff[1]) == -69 && static_cast<int>(_buff[2]) == -65) {
    pos = 3;
    lastPos = pos;
  }

  while (pos < readN) {
    if (!firstChar && std::isspace(_buff[pos])) {
      pos++;
      lastPos = pos;
      continue;
    }

    if (_buff[pos] == '"' && !esc) {
      esc = true;
      pos++;
      lastPos = pos;
      continue;
    }

    firstChar = true;

    if (!esc) {
      const char* c = strchr(_buff + pos, ',');
      if (c)
        pos = c - _buff;
      else
        pos = readN - 1;
    } else {
      const char* c = strchr(_buff + pos, '"');
      if (c)
        pos = c - _buff;
      else
        pos = readN - 1;

      if (pos < readN - 1 && _buff[pos + 1] == '"') {
        pos += 2;
        esc_quotes_found++;
        continue;
      } else {
        // we end this field here, because of the closing quotes
        // see CSV spec at http://tools.ietf.org/html/rfc4180#page-2
        _buff[pos] = 0;
        esc = false;
        firstChar = false;

        const char* c = strchr(_buff + pos + 1, ',');
        if (c)
          pos = c - _buff;
        else
          pos = readN - 1;
      }
    }

    if (_buff[pos] == ',') {
      _buff[pos] = 0;
      firstChar = false;
    }

    if (!esc_quotes_found) {
      _currentItems.push_back(inlineRightTrim(_buff + lastPos));
    } else {
      size_t p = -1;

      // we have to modify to string (that is, we have to remove
      // characters. create a copy of this item
      // on the line-wise modified vector
      _currentModItems.push_back(_buff + lastPos);

      while (esc_quotes_found) {
        p = _currentModItems.back().find("\"\"", p + 1);
        if (p != std::string::npos) {
          _currentModItems.back().replace(p, 2, "\"");
        }

        esc_quotes_found--;
      }

      // pointers will point to strings on the modified string vector
      _currentItems.push_back(inlineRightTrim(_currentModItems.back().c_str()));
    }

    lastPos = ++pos;
  }
  return true;
}

// _____________________________________________________________________________
const char* CsvParser::getTString(const size_t i) const {
  return _currentItems[i];
}

// _____________________________________________________________________________
double CsvParser::getDouble(const size_t i) const {
  if (i >= _currentItems.size())
    throw CsvParserException("expected float number", i, getFieldName(i),
                             _curLine);

  bool fail = false;
  double ret = atof(_currentItems[i], 38, &fail);
  if (fail) {
    std::string a = "expected float number, found ";
    a += _currentItems[i];
    throw CsvParserException(a, i, getFieldName(i),
                             _curLine);
  }
  return ret;
}

// _____________________________________________________________________________
int32_t CsvParser::getLong(const size_t i) const {
  if (i >= _currentItems.size())
    throw CsvParserException("expected non-negative integer number", i,
                             getFieldName(i), _curLine);
  bool fail = false;
  uint32_t ret = atoi(_currentItems[i], &fail);
  if (fail)
    throw CsvParserException("expected non-negative integer number", i,
                             getFieldName(i), _curLine);
  return ret;
}

// _____________________________________________________________________________
bool CsvParser::hasItem(const std::string& fieldName) const {
  return _headerMap.find(fieldName) != _headerMap.end();
}

// _____________________________________________________________________________
bool CsvParser::fieldIsEmpty(const std::string& fieldName) const {
  return strlen(getTString(fieldName)) == 0;
}

// _____________________________________________________________________________
bool CsvParser::fieldIsEmpty(size_t field) const {
  return strlen(getTString(field)) == 0;
}

// _____________________________________________________________________________
const char* CsvParser::getTString(const std::string& fieldName) const {
  return getTString(getFieldIndex(fieldName));
}

// _____________________________________________________________________________
double CsvParser::getDouble(const std::string& fieldName) const {
  return getDouble(getFieldIndex(fieldName));
}

// _____________________________________________________________________________
int32_t CsvParser::getLong(const std::string& fieldName) const {
  return getDouble(getFieldIndex(fieldName));
}

// _____________________________________________________________________________
size_t CsvParser::getNumColumns() const { return _currentItems.size(); }

// _____________________________________________________________________________
size_t CsvParser::getFieldIndex(const string& fieldName) const {
  if (_headerMap.find(fieldName) == _headerMap.end())
    throw CsvParserException("field " + fieldName + " does not exist.", -1,
                             fieldName, _curLine);
  return _headerMap.find(fieldName)->second;
}

// _____________________________________________________________________________
size_t CsvParser::getOptFieldIndex(const string& fieldName) const {
  if (_headerMap.find(fieldName) == _headerMap.end())
    return std::numeric_limits<std::size_t>::max();
  return _headerMap.find(fieldName)->second;
}

// _____________________________________________________________________________
int32_t CsvParser::getCurLine() const { return _curLine; }

// _____________________________________________________________________________
const string CsvParser::getFieldName(size_t i) const {
  if (i < _headerVec.size()) return _headerVec[i].c_str();
  return "(no field name given)";
}

// _____________________________________________________________________________
void CsvParser::parseHeader() {
  _headerMap.clear();
  for (size_t i = 0; i < getNumColumns(); ++i) {
    string s = getTString(i);
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    _headerMap[s] = i;
    _headerVec.push_back(s);
  }
}

// _____________________________________________________________________________
const char* CsvParser::inlineRightTrim(const char* t) const {
  char* s = const_cast<char*>(t);
  char* end = s + std::strlen(s) - 1;
  while (std::isspace(*end)) *end-- = 0;
  return s;
}

// ___________________________________________________________________________
inline uint32_t CsvParser::atoi(const char* p, bool* fail) {
  uint32_t x = 0;
  while (*p >= '0' && *p <= '9') {
    x = (x * 10) + (*p - '0');
    ++p;
  }
  if (*p != 0 && *p != ' ') *fail = true;
  return x;
}

// ___________________________________________________________________________
inline double CsvParser::atof(const char* p, uint8_t mn, bool* fail) {
  // this atof implementation works only on "normal" float strings like
  // 56.445 or -345.00, but should be faster than std::atof
  double ret = 0.0;
  bool neg = false;
  bool decSep = false;
  if (*p == '-') {
    neg = true;
    p++;
  }

  while (*p >= '0' && *p <= '9') {
    ret = ret * 10.0 + (*p - '0');
    p++;
  }

  if (*p == '.') {
    if (decSep) {
      *fail = true;
      return 0;
    }
    decSep = true;
    p++;
    double f = 0;
    uint8_t n = 0;

    for (; n < mn && *p >= '0' && *p <= '9'; n++, p++) {
      f = f * 10.0 + (*p - '0');
    }

    if (n < 10)
      ret += f / pow10[n];
    else
      ret += f / std::pow(10, n);
  } else if (*p != 0 && *p != ' ') {
    *fail = true;
    return 0;
  }

  if (neg) return -ret;
  return ret;
}

"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _express = require("express");

var _moment = require("moment");

var _moment2 = _interopRequireDefault(_moment);

var _db = require("../db.js");

var _db2 = _interopRequireDefault(_db);

var _util = require("util");

var _util2 = _interopRequireDefault(_util);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var router = (0, _express.Router)();

var getWorkHoursAt = function getWorkHoursAt(date) {
  var fromDb = getWorkhoursFromDb(date);
  return fromDb.then(function (data) {
    console.log("fromDb data: " + _util2.default.inspect(data));
    if (data != null) {
      return data;
    } else {
      return getNewWorkHours(date);
    }
  });
};

var getWorkhoursFromDb = function getWorkhoursFromDb(date) {
  return _db2.default.oneOrNone("SELECT * FROM hours where start::date = '" + (0, _moment2.default)(date).format("YYYY-MM-DD") + "' ORDER BY start DESC LIMIT 1").then(function (data) {
    console.log("got one: " + _util2.default.inspect(data));
    if (data != null && data.duration === null) {
      var _date = (0, _moment2.default)(data.start);
      console.log("moment date: " + _util2.default.inspect(_date));
      return {
        startDate: _date.format("YYYY-MM-DD"),
        startTime: _date.format("HH:mm"),
        hours: 0
      };
    }
    return null;
  }).catch(function (error) {
    console.log("Error accurred getWorkhoursFromDb: " + error);
  });
};

var getNewWorkHours = function getNewWorkHours(date) {
  var day = (0, _moment2.default)(date).format("YYYY-MM-DD");
  var time = (0, _moment2.default)(date).format("HH:mm");

  var work = {
    startDate: day,
    startTime: time,
    hours: 0
  };
  return work;
};

router.get("/", function (req, res, next) {
  getWorkHoursAt(new Date()).then(function (data) {
    console.log("workhoursAt: " + _util2.default.inspect(data));
    res.render("index", data);
  });
});

exports.default = router;
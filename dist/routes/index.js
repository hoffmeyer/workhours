"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _express = require("express");

var _moment = require("moment");

var _moment2 = _interopRequireDefault(_moment);

var _db = require("../db.js");

var _db2 = _interopRequireDefault(_db);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var router = (0, _express.Router)();


var getWorkHoursAt = function getWorkHoursAt(date) {
  var fromDb = getWorkhoursFromDb(date);
  if (fromDb != null) {
    return fromDb;
  } else {
    return getNewWorkHours(date);
  }
};

var getWorkhoursFromDb = function getWorkhoursFromDb(date) {
  _db2.default.oneOrNone("SELECT * FROM hours where startDate = " + date.toString() + "ORDER BY startDate DESC LIMIT 1").then(function (data) {}).catch(function (error) {});
};

var getNewWorkHours = function getNewWorkHours(date) {
  var day = (0, _moment2.default)(date).format("YYYY-MM-DD");
  var time = (0, _moment2.default)(date).format("HH:mm");

  var work = {
    startDate: day,
    startTime: time,
    hours: 0,
    minutes: 0,
    lunch: 30
  };
  return work;
};

router.get("/", function (req, res, next) {
  res.render("index", getWorkHoursAt(new Date()));
});

exports.default = router;
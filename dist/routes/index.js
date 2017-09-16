"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _express = require("express");

var _moment = require("moment");

var _moment2 = _interopRequireDefault(_moment);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var router = (0, _express.Router)();

router.get("/", function (req, res, next) {
  var date = new Date();
  var today = (0, _moment2.default)().format("YYYY-MM-DD");
  var now = (0, _moment2.default)().format("HH:mm");

  var time = {
    startDate: today,
    startTime: now,
    hours: 0,
    minutes: 0,
    lunch: 30
  };

  res.render("index", time);
});

exports.default = router;
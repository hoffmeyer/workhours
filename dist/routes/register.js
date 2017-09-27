"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _express = require("express");

var _db = require("../db.js");

var _db2 = _interopRequireDefault(_db);

var _v = require("uuid/v4");

var _v2 = _interopRequireDefault(_v);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var router = (0, _express.Router)();


var combineDateAndTime = function combineDateAndTime(date, time) {
  return new Date(date + " " + time);
};

router.post("/", function (req, res, next) {
  var work = req.body;

  _db2.default.none("INSERT INTO hours(id, start, duration) VALUES($1, $2, $3)", [(0, _v2.default)(), new Date(work.startDate + " " + work.startTime), work.hours]).then(function () {
    console.log("Insert successfull");
    res.redirect("/?sucess=true");
  }).catch(function (error) {
    console.log("Inserting hours into db failed: " + error);
    console.log(error.message);
  });
});

exports.default = router;
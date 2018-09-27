// Generated by BUCKLESCRIPT VERSION 4.0.5, PLEASE EDIT WITH CARE
'use strict';

var Caml_obj = require("bs-platform/lib/js/caml_obj.js");

function addDays(date, days) {
  var newDate = new Date(date.valueOf());
  newDate.setDate(newDate.getDate() + days);
  return newDate;
}

function startOfWeek(date) {
  var dayOfWeek = date.getDay();
  var day = addDays(date, dayOfWeek * -1);
  day.setHours(0, 0, 0, 0);
  return day;
}

function endOfWeek(date) {
  var dayOfWeek = date.getDay();
  var day = addDays(date, 6 - dayOfWeek);
  day.setHours(13, 59, 59, 999);
  return day;
}

function isBefore(d1, d2) {
  return d1.valueOf() > d2.valueOf();
}

function isAfter(d1, d2) {
  return d1.valueOf() < d2.valueOf();
}

function isSameDay(d1, d2) {
  if (d1.getFullYear() === d2.getFullYear() && d1.getMonth() === d2.getMonth()) {
    return d1.getDay() === d2.getDay();
  } else {
    return false;
  }
}

function compareAsc(d1, d2) {
  if (Caml_obj.caml_greaterthan(d1, d2)) {
    return 1;
  } else if (Caml_obj.caml_lessthan(d1, d2)) {
    return -1;
  } else {
    return 0;
  }
}

function compareDesc(d1, d2) {
  return compareAsc(d2, d1);
}

exports.addDays = addDays;
exports.startOfWeek = startOfWeek;
exports.endOfWeek = endOfWeek;
exports.isBefore = isBefore;
exports.isAfter = isAfter;
exports.isSameDay = isSameDay;
exports.compareAsc = compareAsc;
exports.compareDesc = compareDesc;
/* No side effect */

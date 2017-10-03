'use strict';

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _util = require('util');

var _util2 = _interopRequireDefault(_util);

var _express = require('express');

var _v = require('uuid/v4');

var _v2 = _interopRequireDefault(_v);

var _db = require('../db.js');

var _db2 = _interopRequireDefault(_db);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var router = (0, _express.Router)();

var combineDateAndTime = function combineDateAndTime(date, time) {
  return new Date(date + ' ' + time);
};

var insertNew = function insertNew(res, work) {
  _db2.default.none('INSERT INTO hours(id, start, duration) VALUES($1, $2, $3);', [(0, _v2.default)(), new Date(work.startDate + ' ' + work.startTime), work.hours]).then(function () {
    console.log('Insert successfull');
    res.redirect('/?sucess=true');
  }).catch(function (error) {
    console.log('Inserting hours into db failed: ' + error);
    console.log(error.message);
  });
};

var updateExisting = function updateExisting(res, work) {
  _db2.default.none('UPDATE hours SET start=$2, duration=$3 WHERE id=$1;', [work.id, new Date(work.startDate + ' ' + work.startTime), work.hours]).then(function () {
    console.log('Update successfull');
    res.redirect('/?sucess=true');
  }).catch(function (error) {
    console.log('Updating hours with id ' + (work.id != null ? work.id : 'null') + ' failed: ' + error);
    console.log(error.message);
  });
};

router.post('/', function (req, res, next) {
  var work = req.body;
  console.log('saving to database: ' + _util2.default.inspect(work));

  if (work.id === null || work.id === '') {
    insertNew(res, work);
  } else {
    updateExisting(res, work);
  }
});

exports.default = router;
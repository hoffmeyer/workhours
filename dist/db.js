'use strict';

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _pgPromise = require('pg-promise');

var _pgPromise2 = _interopRequireDefault(_pgPromise);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var username = 'workhours';

var password = 'workhours';
var host = 'localhost';
var port = 'port';
var database = 'database';

exports.default = (0, _pgPromise2.default)('postgres://' + username + ':' + password + '@' + host + ':' + port + '/' + database);
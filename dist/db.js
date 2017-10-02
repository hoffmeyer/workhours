'use strict';

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _pgPromise = require('pg-promise');

var _pgPromise2 = _interopRequireDefault(_pgPromise);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var options = {};


var connection = {
  host: 'localhost',
  port: 5432,
  database: 'postgres',
  user: 'postgres',
  password: 'postgres'
};

exports.default = (0, _pgPromise2.default)(options)(connection);
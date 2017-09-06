'use strict';

var _express = require('express');

var router = (0, _express.Router)();

/* GET users listing. */

router.get('/', function (req, res, next) {
  res.send('respond with a resource');
});

module.exports = router;
'use strict';

Object.defineProperty(exports, "__esModule", {
        value: true
});

var _express = require('express');

var router = (0, _express.Router)();


router.post('/', function (req, res, next) {
        var work = req.body;
        console.log(work);
});

exports.default = router;
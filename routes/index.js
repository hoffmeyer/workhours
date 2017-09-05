let express = require('express');
let router = express.Router();
let moment = require('moment');

let date = new Date();
let today = moment().format('YYYY-MM-DD');
let now = moment().format('HH:mm');

let time = {
  startDate: today,
  startTime: now,
  endDate: today,
  endTime: now,
};

/* GET home page. */
router.get('/', function(req, res, next) {
  console.log(time);
  res.render('index', time);
});

module.exports = router;

// @flow
import {Router} from 'express';
import moment from 'moment';
let router = Router();

const date = new Date();
const today = moment().format('YYYY-MM-DD');
const now = moment().format('HH:mm');

const time = {
  startDate: today,
  startTime: now,
  endDate: today,
  endTime: now,
};

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', time);
});

export default router;

// @flow

import util from 'util';
import {groupBy} from 'ramda';

import {Router} from 'express';
import moment from 'moment';

import {getWorkFromDateToNow} from '../db';

let router = Router();

const weekToUiWeek = work => {
  return {
    ...work,
    description: moment(work.start).format('dddd DD/MM/YY, hh:mm'),
  };
};
const byWeek = list => groupBy(work => moment(work.start).isoWeek(), list);

router.get('/', (req, res, next) => {
  getWorkFromDateToNow(
    moment()
      .startOf('isoweek')
      .subtract(21, 'days')
      .utcOffset('+01:00')
      .toString(),
  ).then(works => {
    res.render('list', {weeks: byWeek(works.map(weekToUiWeek))});
  });
});

export default router;

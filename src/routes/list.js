// @flow

import util from 'util';
import R from 'ramda';

import {Router} from 'express';
import moment from 'moment';

import {type Work, type UiWork, workToUiWork} from '../types';

import {getWorkFromDateToNow} from '../db';

let router = Router();

// added space to week otherwise the week number is converted to number and used for array index, making it
// impossible to reorder the array
const workToWeek = (work: UiWork): string => moment(work.start).isoWeek() + ' ';
const startOfWeek: string = moment()
  .startOf('isoweek')
  .subtract(21, 'days')
  .utcOffset('+01:00')
  .toString();

router.get('/', (req, res, next) => {
  getWorkFromDateToNow(startOfWeek)
    .then(
      R.pipe(
        R.map(workToUiWork),
        R.groupBy(workToWeek),
        R.toPairs,
        R.reverse,
        R.fromPairs,
      ),
    )
    .then(uiWorksByWeek => res.render('list', {weeks: uiWorksByWeek}))
    .catch(e => 'Failed to fetch getWorkFromDateToNow for list: ' + e);
});

export default router;

// @flow

import util from 'util';

import {Router} from 'express';
import R from 'ramda';
import moment from 'moment';

import {type UiWork, type Work, workToUiWork} from '../types';
import mWork from '../models/work';
import {isLoggedIn} from '../util/auth';

let router = Router();

// added space to week otherwise the week number is converted to number and used for array index, making it
// impossible to reorder the array
const workToWeek = (work: UiWork): string => moment(work.start).isoWeek() + ' ';
const startOfWeek: string = moment()
  .startOf('isoweek')
  .subtract(21, 'days')
  .utcOffset('+01:00')
  .toString();

const trimEnd = (works: Array<Work>): Array<Work> => {
  if (works.length == 0) {
    return works;
  } else if (works[0].duration == 0) {
    return trimEnd(R.tail(works));
  } else {
    return works;
  }
};

const toStartAndEndDate = (works: Array<Work>): Array<Date> => {
  const first = R.head(works);
  const last = R.last(works);
  if (first != null && last != null) {
    return [first.start, last.start];
  }
  return [];
};

const getDateList = (start: Date, end: Date, accu: number): number => {
  if (end < start) {
    return accu;
  }
  const day = start.getDay();
  const newStart = new Date(start);
  newStart.setDate(newStart.getDate() + 1);
  if (day != 0 && day != 6) {
    return getDateList(newStart, end, accu + 1);
  } else {
    return getDateList(newStart, end, accu);
  }
};

const startAndEndToWorkDays = (startEnd: Array<Date>): number => {
  if (startEnd.length == 0) {
    return 0;
  }
  const start = startEnd[1];
  start.setHours(0, 0, 0, 0);
  const end = startEnd[0];
  end.setHours(0, 0, 0, 0);
  return getDateList(start, end, 0);
};

router.get('/', isLoggedIn, (req, res) => {
  mWork
    .fromDateToNow(startOfWeek, req.user.id)
    .then(
      R.pipe(
        R.map(workToUiWork),
        R.groupBy(workToWeek),
        R.toPairs,
        R.reverse,
        R.fromPairs,
      ),
    )
    .then(uiWorksByWeek => {
      mWork
        .all(req.user.id)
        .then(work => {
          const workToNormalHours = R.pipe(
            trimEnd,
            toStartAndEndDate,
            startAndEndToWorkDays,
          );

          const iterator = (accu: number, work: Work) =>
            work.duration - work.lunch + accu;

          const actualHours = R.reduce(iterator, 0, work);

          const normalHours =
            workToNormalHours(work) * (req.user.workhoursperweek / 5);

          const balance = Math.round((actualHours - normalHours) * 4) / 4;

          res.render('list', {
            balance: balance,
            weeks: uiWorksByWeek,
          });
        })
        .catch(e => 'Failed to fetch getAllwork for list: ' + e);
    })
    .catch(e => 'Failed to fetch getWorkFromDateToNow for list: ' + e);
});

export default router;

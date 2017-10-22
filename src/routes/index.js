// @flow

import {Router} from 'express';
import R from 'ramda';
import moment from 'moment';

import {type Work, workToUiWork} from '../types';
import {getWorkFromDate} from '../db';

let router = Router();

const defaultWork: Work = {
  id: null,
  start: new Date(),
  duration: 0,
};

const inProgressOrNew: (?Work) => Work = work => {
  if (work == null || work.duration != 0) {
    return defaultWork;
  }
  return work;
};

const updateDuration: Work => Work = work => {
  const duration = moment.duration(moment(new Date()).diff(work.start));
  const hours = duration.asHours();
  const roundedHours = Math.round(hours * 4) / 4;
  return {...work, duration: roundedHours};
};

router.get('/', (req, res, next) => {
  const date: string = moment(new Date()).format('YYYY-MM-DD');
  getWorkFromDate(date)
    .then(R.pipe(inProgressOrNew, updateDuration, workToUiWork))
    .then(data => res.render('index', data))
    .catch(e => 'Failed to fetch new Hour: ' + e);
});

export default router;

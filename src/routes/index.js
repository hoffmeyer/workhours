// @flow

import {Router} from 'express';
import moment from 'moment';
import R from 'ramda';

import type {Work} from '../types';
import {getHour} from '../db';

let router = Router();

const workToUi = (work: Work) => {
  const date = moment(work.start);
  return {
    work: {
      id: work.id,
      startDate: date.format('YYYY-MM-DD'),
      startTime: date.format('HH:mm'),
      hours: 0,
    },
    isWorking: work.id != null,
  };
};

const getNewWorkHours = (date: Date): Work => {
  return {
    id: null,
    start: date,
    duration: 0,
  };
};

const useOrNew = (work: ?Work): Work => {
  if (work != null && work.duration === 0) {
    return work;
  }
  return getNewWorkHours(new Date());
};

router.get('/', (req, res, next) => {
  getHour(moment(new Date()).format('YYYY-MM-DD'))
    .then(useOrNew)
    .then(workToUi)
    .then(data => res.render('index', data))
    .catch(e => 'Failed to fetch new Hour: ' + e);
});

export default router;

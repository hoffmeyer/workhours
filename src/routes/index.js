// @flow

import {Router} from 'express';
import moment from 'moment';

import type {Hour, InputWork, Work} from '../types';
import {getHour} from '../db';

let router = Router();

type updateWork = {
  work: Work,
  isWorking: boolean,
};

const hourToMaybeWork = (hour: Hour): ?Work => {
  if (hour != null && hour.duration === 0) {
    const date = moment(hour.start);
    return {
      id: hour.id,
      startDate: date.format('YYYY-MM-DD'),
      startTime: date.format('HH:mm'),
      hours: 0,
    };
  }
  return null;
};

const maybeWorkToInputWork = (maybeWork: ?Work): InputWork => {
  const work = maybeWork != null ? maybeWork : getNewWorkHours(new Date());
  return {
    work: work,
    isWorking: work.id != null,
  };
};

const getNewWorkHours = (date: Date): Work => {
  const day = moment(date).format('YYYY-MM-DD');
  const time = moment(date).format('HH:mm');

  const work: Work = {
    id: null,
    startDate: day,
    startTime: time,
    hours: 0,
  };
  return work;
};

router.get('/', (req, res, next) => {
  getHour(moment(new Date()).format('YYYY-MM-DD'))
    .then(hourToMaybeWork)
    .then(maybeWorkToInputWork)
    .then(data => res.render('index', data));
});

export default router;

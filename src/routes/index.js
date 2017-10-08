// @flow

import util from 'util';

import {Router} from 'express';
import moment from 'moment';
import R from 'ramda';

import type {Work} from '../types';
import {getWork} from '../db';

let router = Router();

type UiWork = {
  isWorking: boolean,
  work: {
    id: ?string,
    startDate: string,
    startTime: string,
    hours: number,
  },
};

const workToUi = (work: Work) => {
  const date = moment(work.start);
  const uiWork: UiWork = {
    work: {
      id: work.id,
      startDate: date.format('YYYY-MM-DD'),
      startTime: date.format('HH:mm'),
      hours: 0,
    },
    isWorking: work.id != null,
  };
  return uiWork;
};

const getNewWorkHours = (): Work => {
  return {
    id: null,
    start: new Date(),
    duration: 0,
  };
};

const useOrNew = (work: ?Work): Work => {
  if (work != null && work.duration === 0) {
    return work;
  }
  return getNewWorkHours();
};

router.get('/', (req, res, next) => {
  const date: string = moment(new Date()).format('YYYY-MM-DD');
  getWork(date)
    .then(R.compose(workToUi, useOrNew))
    .then(data => res.render('index', data))
    .catch(e => 'Failed to fetch new Hour: ' + e);
});

export default router;

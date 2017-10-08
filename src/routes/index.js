// @flow

import util from 'util';

import {Router} from 'express';
import R from 'ramda';
import moment from 'moment';

import {type Work, workToUiWork} from '../types';
import {getWork} from '../db';

let router = Router();

const getNewWorkHours = (): Work => {
  return {
    id: null,
    start: new Date(),
    duration: 0,
  };
};

const useOrNew = (work: ?Work): Work =>
  work != null && work.duration == 0 ? work : getNewWorkHours();

router.get('/', (req, res, next) => {
  const date: string = moment(new Date()).format('YYYY-MM-DD');
  getWork(date)
    .then(R.compose(workToUiWork, useOrNew))
    .then(data => res.render('index', data))
    .catch(e => 'Failed to fetch new Hour: ' + e);
});

export default router;

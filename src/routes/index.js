// @flow

import {Router} from 'express';
import R from 'ramda';
import moment from 'moment';

import {type Work, workToUiWork} from '../types';
import {getWork} from '../db';

let router = Router();

const defaultWork: Work = {
  id: null,
  start: new Date(),
  duration: 0,
};

const isInProgress: Work => boolean = R.pipe(R.prop('duration'), R.equals(0));

const inProgressOrNew = R.when(
  R.either(R.isNil, R.complement(isInProgress)),
  R.always(defaultWork),
);

router.get('/', (req, res, next) => {
  const date: string = moment(new Date()).format('YYYY-MM-DD');
  getWork(date)
    .then(R.pipe(inProgressOrNew, workToUiWork))
    .then(data => res.render('index', data))
    .catch(e => 'Failed to fetch new Hour: ' + e);
});

export default router;

import {Router} from 'express';
import {pipe} from 'ramda';
import * as moment from 'moment';
import {log} from 'winston';

import {Work, workToUiWork} from '../types';
import mWork from '../models/work';
import {isLoggedIn} from '../util/auth';

let router = Router();

const inProgressOrNew: (Work) => Work = work => {
  log('info', 'inProgressOrNew');
  if (work == null || work.duration != 0) {
    const defaultWork: Work = {
      id: null,
      start: new Date(),
      duration: 0,
      lunch: 0,
      userid: '',
    };
    return defaultWork;
  }
  return work;
};

const setUser = (work: Work, userid: string) => {
  return {
    ...work,
    userid: userid,
  };
};

const updateDurationAndLunch = work => {
  log('info', 'updateDurationAndLunch');
  if (work.id == null) {
    return work;
  } else {
    const duration = moment.duration(moment(new Date()).diff(work.start));
    const hours = duration.asHours();
    const roundedHours = Math.round(hours * 4) / 4;
    const lunch =
      (!work.duration || work.duration == 0) && roundedHours > 4 ? 0.5 : 0;
    return {...work, duration: roundedHours, lunch: lunch};
  }
};

router.get('/', isLoggedIn, (req, res, next) => {
  const date: string = moment(new Date()).format('YYYY-MM-DD');
  log('info', 'This is from the index route');
  mWork
    .fromDate(date, req.user.id)
    .then(pipe(inProgressOrNew, updateDurationAndLunch, workToUiWork))
    .then(data => res.render('index', setUser(data, req.user.id)))
    .catch(e => 'Failed to fetch new Hour: ' + e);
});

export default router;

// @flow
import util from 'util';

import {Router} from 'express';

import {type UiWork, type Work, uiWorkToWork} from '../types';
import {insertWork, updateWork} from '../db';

const router = Router();

const combineDateAndTime = (date: string, time: string): Date => {
  return new Date(date + ' ' + time);
};

const insertNew = (res, work: Work) => {
  insertWork(work).then(() => {
    res.redirect('/?sucess=true');
  });
};

const updateExisting = (res, work: Work) => {
  updateWork(work).then(() => {
    res.redirect('/?sucess=true');
  });
};

router.post('/', function(req, res, next) {
  const uiWork: UiWork = req.body;
  console.log('saving to database: ' + util.inspect(uiWork));

  if (uiWork.id === null || uiWork.id === '') {
    insertNew(res, uiWorkToWork(uiWork));
  } else {
    updateExisting(res, uiWorkToWork(uiWork));
  }
});

export default router;

// @flow
import {Router} from 'express';

import {type UiWork, type Work, uiWorkToWork} from '../types';
import {insertWork, updateWork} from '../db';
import {isLoggedIn} from '../util/auth';

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

router.post('/', isLoggedIn, function(req, res, next) {
  const uiWork: UiWork = req.body;

  if (uiWork.id === null || uiWork.id === '') {
    insertNew(res, uiWorkToWork({...uiWork, userid: req.user.id}));
  } else {
    updateExisting(res, uiWorkToWork(uiWork));
  }
});

export default router;

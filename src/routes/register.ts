import {Router} from 'express';

import {UiWork, Work, uiWorkToWork} from '../types';
import work from '../models/work';
import {isLoggedIn} from '../util/auth';

const router = Router();

const combineDateAndTime = (date: string, time: string): Date => {
  return new Date(date + ' ' + time);
};

const insertNew = (res, newWork: Work) => {
  work.insert(newWork).then(() => {
    res.redirect('/?sucess=true');
  });
};

const updateExisting = (res, newWork: Work) => {
  work.update(newWork).then(() => {
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

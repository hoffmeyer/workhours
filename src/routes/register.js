// @flow
import util from 'util';

import {Router} from 'express';

import type {Work} from '../types';
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
  const work: Work = req.body;
  console.log('saving to database: ' + util.inspect(work));

  if (work.id === null || work.id === '') {
    insertNew(res, work);
  } else {
    updateExisting(res, work);
  }
});

export default router;

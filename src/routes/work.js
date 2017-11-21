// @flow

import {Router} from 'express';

import {getWorkFromId} from '../db';
import {isLoggedIn} from '../util/auth';
import {workToUiWork} from '../types';

let router = Router();

router.get('/', isLoggedIn, (req, res, next) => {
  res.render('work', null);
});

router.get('/:id', isLoggedIn, (req, res, next) => {
  const id: string = req.params.id;
  getWorkFromId(id).then(work => {
    if (work != null) {
      res.render('work', workToUiWork(work));
    } else {
      res.render('nowork');
    }
  });
});

export default router;

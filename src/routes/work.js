// @flow

import {Router} from 'express';

import {type Work, workToUiWork} from '../types';
import {getWorkFromId} from '../db';

let router = Router();

router.get('/', (req, res, next) => {
  res.render('work', null);
});

router.get('/:id', (req, res, next) => {
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

import {Router} from 'express';

import work from '../models/work';
import {isLoggedIn} from '../util/auth';
import {workToUiWork} from '../types';

let router = Router();

router.get('/', isLoggedIn, (req, res, next) => {
  res.render('work', null);
});

router.get('/:id', isLoggedIn, (req, res, next) => {
  const id: string = req.params.id;
  work.fromId(id).then(work => {
    if (work != null) {
      res.render('work', workToUiWork(work));
    } else {
      res.render('nowork');
    }
  });
});

export default router;

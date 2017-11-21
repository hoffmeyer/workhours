// @flow
import {Router} from 'express';

import {deleteWork} from '../db';
import {isLoggedIn} from '../util/auth';

const router = Router();

router.get('/:id', isLoggedIn, function(req, res, next) {
  const workId: ?string = req.params.id;

  if (workId != null) {
    deleteWork(workId).then(() => res.redirect('/list?sucess=true'));
  } else {
    console.log('Could not delete, no id provided');
  }
});

export default router;

// @flow
import {Router} from 'express';

import {deleteWork} from '../db';

const router = Router();

router.get('/:id', function(req, res, next) {
  const workId: ?string = req.params.id;

  if (workId != null) {
    deleteWork(workId).then(() => res.redirect('/list?sucess=true'));
  } else {
    console.log('Could not delete, no id provided');
  }
});

export default router;

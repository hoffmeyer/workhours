// @flow
import {Router} from 'express';

import Work from '../models/work';
import {isLoggedIn} from '../util/auth';

const router = Router();

router.get('/:id', isLoggedIn, function(req, res, next) {
  const workId: ?string = req.params.id;

  if (workId != null) {
    Work.delete(workId).then(() => res.redirect('/list?sucess=true'));
  } else {
    console.log('Could not delete, no id provided');
  }
});

export default router;

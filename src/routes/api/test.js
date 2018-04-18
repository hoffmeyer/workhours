
// @flow

import {Router} from 'express';

let router = Router();

router.get('/', (req, res) => {
  res.json({testData: ['This', 'is', 'a', 'test']});
});

export default router;
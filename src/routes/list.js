// @flow

import util from 'util';

import {Router} from 'express';

import {getAllWork} from '../db';

let router = Router();

router.get('/', (req, res, next) => {
  getAllWork().then(works => {
    res.render('list', {works: works});
  });
});

export default router;

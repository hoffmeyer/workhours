// @flow
import util from 'util';

import {Router} from 'express';
import uuidV4 from 'uuid/v4';

import type {Work} from '../types';
import db from '../db.js';

const router = Router();

const combineDateAndTime = (date: string, time: string): Date => {
  return new Date(date + ' ' + time);
};

const insertNew = (res, work: Work) => {
  db
    .none('INSERT INTO hours(id, start, duration) VALUES($1, $2, $3);', [
      uuidV4(),
      work.start,
      work.duration,
    ])
    .then(() => {
      console.log('Insert successfull');
      res.redirect('/?sucess=true');
    })
    .catch(error => {
      console.log('Inserting hours into db failed: ' + error);
      console.log(error.message);
    });
};

const updateExisting = (res, work: Work) => {
  db
    .none('UPDATE hours SET start=$2, duration=$3 WHERE id=$1;', [
      work.id,
      work.start,
      work.duration,
    ])
    .then(() => {
      console.log('Update successfull');
      res.redirect('/?sucess=true');
    })
    .catch(error => {
      console.log(
        'Updating hours with id ' +
          (work.id != null ? work.id : 'null') +
          ' failed: ' +
          error,
      );
      console.log(error.message);
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

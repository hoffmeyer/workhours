// @flow

import {
  Router
} from 'express';
import work from '../../models/work';
import type {
  Work
} from '../../types';
import moment from 'moment';
import {
  isLoggedInApi
} from '../../util/auth';
import winston from 'winston';

let router = Router();

const currentWeekAndThreeWeeksBack: string = moment()
  .startOf('isoweek')
  .subtract(21, 'days')
  .utcOffset('+01:00')
  .toString();

router.get('/', isLoggedInApi, (req, res) => {
  if (!req.query.from || !req.query.to) {
    return res.status(400).send({
      message: "From and to date not set"
    });
  }
  const from: Date = new Date(req.query.from);
  const to: Date = new Date(req.query.to);
  const id: string = req.user.id;
  work.get(id, from, to);
  work.all(id).then(data => res.json(data));
});

router.post('/', isLoggedInApi, (req, res) => {
  const id: string = req.user.id;
  const newwork: work = req.body;
  if (!newwork.start) {
    return res.status(400).send({
      message: "work must have a start time"
    });
  }

  // needed because otherwiser it fucks up the timezone in the db
  newwork.start = new Date(newwork.start);

  if(newwork.id) {
    work.update(newwork).then(()=>{
      res.json(newwork);
    });
  } else {
  newwork.userid = id;
  work.insert(newwork).then(id => {
    newwork.id = id;
    res.json(newwork);
  });

  }
});

// get the work registered in the current week plus the last 3 full weeks
router.get('/latest', isLoggedInApi, (req, res) => {
  const id: string = req.user.id;
  work.fromDateToNow(currentWeekAndThreeWeeksBack, id).then(data => res.json(data));
});

export default router;
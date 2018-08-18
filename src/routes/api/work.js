// @flow

import {
  Router
} from 'express';
import work from '../../models/work';
import moment from 'moment';
import {
  isLoggedInApi
} from '../../util/auth';

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
  const newWork: work = req.body;
  if (!newWork.start) {
    return res.status(400).send({
      message: "New work must have a start time"
    });
  }
  newWork.userid = id;
  work.insert(newWork).then(id => {
    newWork.id = id;
    res.json(newWork);
  });
  work.all(id).then(data => res.json(data));
});

// get the work registered in the current week plus the last 3 full weeks
router.get('/latest', isLoggedInApi, (req, res) => {
  const id: string = req.user.id;
  work.fromDateToNow(currentWeekAndThreeWeeksBack, id).then(data => res.json(data));
});

export default router;
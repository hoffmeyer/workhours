// @flow

import {
  Router
} from 'express';
import api from '../../models/api';
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
  api.all().then(data => res.json(data));
});

// get the work registered in the current week plus the last 3 full weeks
router.get('/latest', isLoggedInApi, (req, res) => {
  api.fromDateToNow(currentWeekAndThreeWeeksBack).then(data => res.json(data));
});

export default router;
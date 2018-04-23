
// @flow

import {Router} from 'express';
import mWork from '../../models/work';
import moment from 'moment';

let router = Router();

const startOfWeek: string = moment()
  .startOf('isoweek')
  .subtract(21, 'days')
  .utcOffset('+01:00')
  .toString();

router.get('/', (req, res) => {
  mWork.allApi().then( data => res.json(data));
});

export default router;
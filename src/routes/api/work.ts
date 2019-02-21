import { Router } from 'express';
import * as R from 'ramda';
import work from '../../models/work';
import { Work } from '../../types';
import * as moment from 'moment';
import { isLoggedInApi } from '../../util/auth';

let router = Router();

const currentWeekAndTenWeeksBack: string = moment()
  .startOf('isoWeek')
  .subtract(70, 'days')
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
  const newwork: Work = req.body;
  if (!newwork.start) {
    return res.status(400).send({
      message: "work must have a start time"
    });
  }

  // needed because otherwiser it fucks up the timezone in the db
  newwork.start = new Date(newwork.start);

  if (newwork.id) {
    work.update(newwork).then(() => {
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

router.delete('/', isLoggedInApi, (req, res) => {
  type hasId = { id: string };
  const workId: hasId = req.body;
  if (!workId.id) {
    return res.status(400).send({
      message: "Please provide id of work to delete"
    });
  }

  work.delete(workId.id).then(() => {
    res.json({});
  });
});

// get the work registered in the current week plus the last 3 full weeks
router.get('/latest', isLoggedInApi, (req, res) => {
  const id: string = req.user.id;
  work.fromDateToNow(currentWeekAndTenWeeksBack, id).then(data => res.json(data));
});

// Balance
const trimEnd = (works: Array<Work>): Array<Work> => {
  if (works.length == 0) {
    return works;
  } else if (works[0].duration == 0) {
    return trimEnd(R.tail(works));
  } else {
    return works;
  }
};

const toStartAndEndDate = (works: Array<Work>): Array<Date> => {
  const first = R.head(works);
  const last = R.last(works);
  if (first != null && last != null) {
    return [first.start, last.start];
  }
  return [];
};

const getDateList = (start: Date, end: Date, accu: number): number => {
  if (end < start) {
    return accu;
  }
  const day = start.getDay();
  const newStart = new Date(start);
  newStart.setDate(newStart.getDate() + 1);
  if (day != 0 && day != 6) {
    return getDateList(newStart, end, accu + 1);
  } else {
    return getDateList(newStart, end, accu);
  }
};

const startAndEndToWorkDays = (startEnd: Array<Date>): number => {
  if (startEnd.length == 0) {
    return 0;
  }
  const start = startEnd[1];
  start.setHours(0, 0, 0, 0);
  const end = startEnd[0];
  end.setHours(0, 0, 0, 0);
  return getDateList(start, end, 0);
};

const workToNormalHours = R.pipe(
  trimEnd,
  toStartAndEndDate,
  startAndEndToWorkDays,
);

const allWorkToBalance = (work: Work[], hoursPerWeek) => {
  const iterator = (accu: number, work: Work) =>
    work.duration - work.lunch + accu;

  const actualHours = R.reduce(iterator, 0, work);

  const normalHours =
    workToNormalHours(work) * (hoursPerWeek / 5);

  return Math.round((actualHours - normalHours) * 4) / 4;

};

router.get('/balance', isLoggedInApi, (req, res) => {
  const id: string = req.user.id;
  work.all(id)
    .then(work => {
      work = work ? work : [];
      res.json({ balance: allWorkToBalance(work, req.user.workhoursperweek) });
    });
});

export default router;
import { Router } from 'express';
import * as R from 'ramda';
import work from '../../models/work';
import { Work, WorkPeriod } from '../../types';
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
  work.get(id, from, to).then(data => res.json(data));
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

router.post('/range', isLoggedInApi, (req, res) => {
  const id: string = req.user.id;
  const period: WorkPeriod = req.body;
  if (!period.start || !period.end) {
    return res.status(400).send({
      message: "work must have a start and end date"
    });
  }

  let firstDate = new Date(period.start);
  firstDate.setHours(8, 0);
  let lastDate = new Date(period.end);
  let dates: Date[] = [firstDate];

  // create array of all the dates
  while (firstDate <= lastDate) {
    let newDate = new Date(firstDate);
    newDate.setDate(firstDate.getDate() + 1);
    dates.push(newDate);
    firstDate = newDate;
  }

  // filter out weekends
  let noWeekends = dates.filter(d => d.getDay() > 0 && d.getDay() < 6);

  let promises = noWeekends.map(async d => {
    let newWork: Work = {
      userid: id,
      start: d,
      duration: period.duration,
      lunch: 0
    };

    newWork.id = await work.insert(newWork);
    return newWork;
  });

  Promise.all(promises)
    .then(newWork => res.json(newWork))
    .catch(err => res.status(400).send({ message: "Creating work range failed" }));
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

const groupingFunc = (w1:Work, w2:Work) => {
  return w1.start.toLocaleDateString() === w2.start.toLocaleDateString();
};

const allWorkToBalance = (work: Work[], hoursPerWeek) => {
  const iterator = (accu: number, work: Work) => work.duration - work.lunch + accu;
  const actualHours = R.reduce(iterator, 0, work);
  const workedDays = R.groupWith(groupingFunc, R.filter( w => w.duration > 0, work));
  const normalHours = workedDays.length * (hoursPerWeek / 5)
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
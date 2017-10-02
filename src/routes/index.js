// @flow

import util from 'util';

import {Router} from 'express';
import moment from 'moment';

import type {Work, updateWork} from '../types';
import db from '../db';

let router = Router();

const getWorkHoursAt = (date: Date): Promise<updateWork> => {
  const fromDb = getWorkhoursFromDb(date);
  return fromDb.then(data => {
    console.log('fromDb data: ' + util.inspect(data));
    const work = data != null ? data : getNewWorkHours(date);
    return {
      work: work,
      isWorking: work.id != null,
    };
  });
};

const getWorkhoursFromDb = (date: Date): Promise<Work> => {
  return db
    .oneOrNone(
      "SELECT * FROM hours where start::date = '" +
        moment(date).format('YYYY-MM-DD') +
        "' ORDER BY start DESC LIMIT 1",
    )
    .then(data => {
      console.log('got one: ' + util.inspect(data));
      if (data != null && data.duration === 0) {
        const date = moment(data.start);
        console.log('moment date: ' + util.inspect(date));
        return {
          id: data.id,
          startDate: date.format('YYYY-MM-DD'),
          startTime: date.format('HH:mm'),
          hours: 0,
        };
      }
      return null;
    })
    .catch(error => {
      console.log('Error accurred getWorkhoursFromDb: ' + error);
    });
};

const getNewWorkHours = (date: Date): Work => {
  const day = moment(date).format('YYYY-MM-DD');
  const time = moment(date).format('HH:mm');

  const work: Work = {
    id: null,
    startDate: day,
    startTime: time,
    hours: 0,
  };
  return work;
};

router.get('/', function(req, res, next) {
  getWorkHoursAt(new Date()).then(data => {
    console.log('workhoursAt: ' + util.inspect(data));
    res.render('index', data);
  });
});

export default router;

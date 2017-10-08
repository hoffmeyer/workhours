// @flow

import util from 'util';

import moment from 'moment';

export type Work = {
  id: ?string,
  start: Date,
  duration: number,
};

export type UiWork = {
  id: ?string,
  startDate: string,
  startTime: string,
  hours: number,
};

const workToUiWork = (work: Work): UiWork => {
  const date = moment(work.start);
  return {
    id: work.id,
    startDate: date.format('YYYY-MM-DD'),
    startTime: date.format('HH:mm'),
    hours: work.duration,
  };
};

const uiWorkToWork = (uiWork: UiWork): Work => {
  return {
    id: uiWork.id,
    start: new Date(uiWork.startDate + ' ' + uiWork.startTime),
    duration: uiWork.hours,
  };
};

export {workToUiWork, uiWorkToWork};

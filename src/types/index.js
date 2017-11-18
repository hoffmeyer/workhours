// @flow

import util from 'util';

import moment from 'moment';

export type Work = {
  id: ?string,
  start: Date,
  duration: number,
  lunch: number,
};

export type UiWork = {
  id: ?string,
  start: Date,
  startDate: string,
  startTime: string,
  formattedStart: string,
  duration: number,
  lunch: number,
};

const workToUiWork = (work: Work): UiWork => {
  const date = moment(work.start);
  return {
    id: work.id,
    start: work.start,
    startDate: date.format('YYYY-MM-DD'),
    startTime: date.format('HH:mm'),
    formattedStart: moment(work.start).format('dddd DD/MM/YY, HH:mm'),
    duration: work.duration,
    lunch: work.lunch,
  };
};

const uiWorkToWork = (uiWork: UiWork): Work => {
  return {
    id: uiWork.id,
    start: new Date(uiWork.startDate + ' ' + uiWork.startTime),
    duration: uiWork.duration,
    lunch: uiWork.lunch,
  };
};

export {workToUiWork, uiWorkToWork};

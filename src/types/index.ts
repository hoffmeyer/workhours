import * as moment from 'moment';

export type User = {
  id: string,
  username: string,
  name: string,
  password: string,
  workHoursPerWeek: number,
  balanceFrom: Date
};

export type Work = {
  id?: string,
  start: Date,
  duration: number,
  lunch: number,
  userid: string,
};

export type UiWork = {
  id?: string,
  start: Date,
  startDate: string,
  startTime: string,
  formattedStart: string,
  duration: number,
  lunch: number,
  userid: string,
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
    userid: work.userid,
  };
};

const uiWorkToWork = (uiWork: UiWork): Work => {
  return {
    id: uiWork.id,
    start: new Date(uiWork.startDate + ' ' + uiWork.startTime),
    duration: uiWork.duration,
    lunch: uiWork.lunch,
    userid: uiWork.userid,
  };
};

export { workToUiWork, uiWorkToWork };

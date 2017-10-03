// @flow

export type Hour = {
  id: string,
  start: Date,
  duration: number,
};

export type InputWork = {
  work: Work,
  isWorking: boolean,
};

export type Work = {
  id: ?string,
  startDate: string,
  startTime: string,
  hours: number,
};

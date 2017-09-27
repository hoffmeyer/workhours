// @flow

export type Work = {
  id: ?string,
  startDate: string,
  startTime: string,
  hours: number
};

export type updateWork = {
  work: Work,
  isWorking: boolean
};

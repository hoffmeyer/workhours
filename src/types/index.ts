export type User = {
  id: string,
  username: string,
  name: string,
  password: string,
  workhoursperweek: number,
  balancefrom: Date
};

export type Work = {
  id?: string,
  start: Date,
  duration: number,
  lunch: number,
  userid: string,
};

export type WorkPeriod = {
  start: Date,
  end: Date,
  duration: number
}
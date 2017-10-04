// @flow
import pgp from 'pg-promise';

import type {Work} from './types';

const options = {};

const connection = {
  host: 'localhost',
  port: 5432,
  database: 'postgres',
  user: 'postgres',
  password: 'workhours',
};
const db = pgp(options)(connection);

const getHour = (date: string): Promise<?Work> =>
  db.oneOrNone(
    "SELECT * FROM work where start::date = '" +
      date +
      "' ORDER BY start DESC LIMIT 1",
  );

export {getHour};

export default db;

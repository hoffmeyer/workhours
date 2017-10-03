// @flow
import pgp from 'pg-promise';

const options = {};

const connection = {
  host: 'localhost',
  port: 5432,
  database: 'postgres',
  user: 'postgres',
  password: 'postgres',
};
const db = pgp(options)(connection);

const getHour = (date: string) =>
  db.oneOrNone(
    "SELECT * FROM hours where start::date = '" +
      date +
      "' ORDER BY start DESC LIMIT 1",
  );

export {getHour};

export default db;

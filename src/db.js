// @flow
import pgp from 'pg-promise';
import R from 'ramda';
import uuidV4 from 'uuid/v4';

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

const getWork = (date: string): Promise<?Work> => {
  return db.oneOrNone(
    "SELECT * FROM work where start::date = '" +
      date +
      "' ORDER BY start DESC LIMIT 1",
  );
};

const insertWork = (work: Work): Promise<void> => {
  return db
    .none('INSERT INTO hours(id, start, duration) VALUES($1, $2, $3);', [
      uuidV4(),
      work.start,
      work.duration,
    ])
    .then(() => {
      console.log('Insert successfull');
    })
    .catch(error => {
      console.log('Inserting hours into db failed: ' + error);
      console.log(error.message);
    });
};

const updateWork = (work: Work): Promise<void> => {
  return db
    .none('UPDATE hours SET start=$2, duration=$3 WHERE id=$1;', [
      work.id,
      work.start,
      work.duration,
    ])
    .then(() => {
      console.log('Insert successfull');
    })
    .catch(error => {
      console.log(
        'Updating hours with id ' +
          (work.id != null ? work.id : 'null') +
          ' failed: ' +
          error,
      );
      console.log(error.message);
    });
};

export {getWork, insertWork, updateWork};

export default db;

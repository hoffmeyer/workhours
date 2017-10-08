// @flow
import util from 'util';

import pgp from 'pg-promise';
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
  return db
    .oneOrNone(
      "SELECT * FROM work where start::date = '" +
        date +
        "' ORDER BY start DESC LIMIT 1",
    )
    .then(one => {
      console.log('getWork returned: ' + util.inspect(one));
      return one;
    })
    .catch(error => {
      console.log('getWork from db failed: ' + error);
      console.log(error.message);
    });
};

const insertWork = (work: Work): Promise<void> => {
  return db
    .none('INSERT INTO work(id, start, duration) VALUES($1, $2, $3);', [
      uuidV4(),
      work.start,
      work.duration,
    ])
    .then(() => {
      console.log('Insert successfull');
    })
    .catch(error => {
      console.log('insertWork db failed: ' + error);
      console.log(error.message);
    });
};

const updateWork = (work: Work): Promise<void> => {
  return db
    .none('UPDATE work SET start=$2, duration=$3 WHERE id=$1;', [
      work.id,
      work.start,
      work.duration,
    ])
    .then(() => {
      console.log('updateWork successfull');
    })
    .catch(error => {
      console.log(
        'Updating work with id ' +
          (work.id != null ? work.id : 'null') +
          ' failed: ' +
          error,
      );
      console.log(error.message);
    });
};

export {getWork, insertWork, updateWork};

export default db;

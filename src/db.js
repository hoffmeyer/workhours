// @flow
import util from 'util';

import pgp from 'pg-promise';
import uuidV4 from 'uuid/v4';
import winston from 'winston';

import type {Work, User} from './types';

const options = {};

const connection = {
  host: process.env.DATABASE_HOST || 'localhost',
  port: process.env.DATABASE_PORT || 5432,
  database: process.env.DATBASE_NAME || 'postgres',
  user: process.env.DATABASE_USERNAME || 'postgres',
  password: process.env.DATABASE_PASSWORD || 'postgres',
};

winston.log(
  'info',
  'Initializing database: ' +
    connection.host +
    ':' +
    connection.port +
    ' db: ' +
    connection.database +
    ' user: ' +
    connection.user,
);

const db = pgp(options)(connection);

winston.log('info', 'Database initialized');

const getWorkFromDate = (date: string): Promise<?Work> => {
  return db
    .oneOrNone(
      "SELECT * FROM work where start::date = '" +
        date +
        "' ORDER BY start DESC LIMIT 1",
    )
    .then(one => {
      winston.log('info', 'getWorkFromDate returned: ' + util.inspect(one));
      return one;
    })
    .catch(error => {
      winston.log('error', 'getWorkFromDate from db failed: ' + error);
      winston.log('error', error.message);
    });
};

const getWorkFromId = (id: string): Promise<?Work> => {
  return db
    .oneOrNone(
      "SELECT * FROM work where id = '" + id + "' ORDER BY start DESC LIMIT 1",
    )
    .then(one => {
      winston.log('info', 'getWorkFromId returned: ' + util.inspect(one));
      return one;
    })
    .catch(error => {
      winston.log('error', 'getWorkFromId from db failed: ' + error);
      winston.log('error', error.message);
    });
};

const insertWork = (work: Work): Promise<void> => {
  return db
    .none(
      'INSERT INTO work(id, start, duration, lunch) VALUES($1, $2, $3, $4);',
      [uuidV4(), work.start, work.duration, work.lunch],
    )
    .then(() => {
      winston.log('info', 'Insert successfull');
    })
    .catch(error => {
      winston.log('error', 'insertWork db failed: ' + error);
      winston.log('error', error.message);
    });
};

const deleteWork = (id: string): Promise<void> => {
  return db
    .none('DELETE from work where id = $1;', [id])
    .then(() => {
      winston.log('info', 'deleted work with id: ' + id);
    })
    .catch(error => {
      winston.log('error', 'Deleting work with id ' + id + ' failed: ' + error);
      winston.log('error', error.message);
    });
};

const updateWork = (work: Work): Promise<void> => {
  return db
    .none('UPDATE work SET start=$2, duration=$3, lunch=$4 WHERE id=$1;', [
      work.id,
      work.start,
      work.duration,
      work.lunch,
    ])
    .then(() => {
      winston.log('info', 'updateWork successfull');
    })
    .catch(error => {
      winston.log(
        'error',
        'Updating work with id ' +
          (work.id != null ? work.id : 'null') +
          ' failed: ' +
          error,
      );
      winston.log('error', error.message);
    });
};

const getAllWork = (userId: string): Promise<Array<Work>> => {
  return db
    .any("SELECT * FROM work WHERE userId='" + userId + "' ORDER BY start DESC")
    .then(any => {
      winston.log(
        'info',
        'query all work successfull, ' + any.length + ' items',
      );
      return any;
    })
    .catch(error => {
      winston.log('error', error.message);
    });
};

const getWorkFromDateToNow = (
  date: string,
  userId: string,
): Promise<Array<Work>> => {
  return db
    .any(
      "SELECT * FROM work WHERE start::date >= '" +
        date +
        "' AND userId='" +
        userId +
        "' ORDER BY start",
    )
    .then(any => {
      winston.log(
        'info',
        'query work week successfull, ' + any.length + ' items',
      );
      return any;
    })
    .catch(error => {
      winston.log('error', error.message);
    });
};

const userById = (id: string): Promise<User> => {
  return db
    .one("SELECT * FROM users WHERE id = '" + id + "'")
    .then(user => {
      winston.log(
        'info',
        'found user by id: ' + user.id + ' name: ' + user.username,
      );
      return user;
    })
    .catch(error => {
      winston.log('error', error.message);
    });
};

const userByUsername = (username: string): Promise<User> => {
  return db
    .one("SELECT * FROM users WHERE username = '" + username + "'")
    .then(user => {
      winston.log(
        'info',
        'found user by usernamme: ' + username + ' name: ' + user.id,
      );
      return user;
    })
    .catch(error => {
      winston.log('error', error.message);
    });
};

export {
  getWorkFromDate,
  getWorkFromId,
  getAllWork,
  getWorkFromDateToNow,
  insertWork,
  deleteWork,
  updateWork,
  userById,
  userByUsername,
};

export default db;

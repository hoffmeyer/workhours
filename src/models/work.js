// @flow
import util from 'util';

import winston from 'winston';
import uuidV4 from 'uuid/v4';

import type {Work} from '../types';
import db from '../db.js';

export default {
  fromDate: (date: string, userId: string): Promise<?Work> => {
    return db
      .oneOrNone(
        "SELECT * FROM work where start::date = '" +
          date +
          "' AND userid = '" +
          userId +
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
  },

  fromId: (id: string): Promise<?Work> => {
    return db
      .oneOrNone(
        "SELECT * FROM work where id = '" +
          id +
          "' ORDER BY start DESC LIMIT 1",
      )
      .then(one => {
        winston.log('info', 'getWorkFromId returned: ' + util.inspect(one));
        return one;
      })
      .catch(error => {
        winston.log('error', 'getWorkFromId from db failed: ' + error);
        winston.log('error', error.message);
      });
  },

  insert: (work: Work): Promise<void> => {
    return db
      .none(
        'INSERT INTO work(id, start, duration, lunch, userid) VALUES($1, $2, $3, $4, $5);',
        [uuidV4(), work.start, work.duration, work.lunch, work.userid],
      )
      .then(() => {
        winston.log('info', 'Insert successfull');
      })
      .catch(error => {
        winston.log('error', 'insertWork db failed: ' + error);
        winston.log('error', error.message);
      });
  },

  delete: (id: string): Promise<void> => {
    return db
      .none('DELETE from work where id = $1;', [id])
      .then(() => {
        winston.log('info', 'deleted work with id: ' + id);
      })
      .catch(error => {
        winston.log(
          'error',
          'Deleting work with id ' + id + ' failed: ' + error,
        );
        winston.log('error', error.message);
      });
  },

  update: (work: Work): Promise<void> => {
    return db
      .none(
        'UPDATE work SET start=$2, duration=$3, lunch=$4, userid=$5 WHERE id=$1;',
        [work.id, work.start, work.duration, work.lunch, work.userid],
      )
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
  },

  all: (userid: string): Promise<Array<Work>> => {
    return db
      .any(
        "SELECT * FROM work WHERE userid='" + userid + "' ORDER BY start DESC",
      )
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
  },

  fromDateToNow: (date: string, userid: string): Promise<Array<Work>> => {
    return db
      .any(
        "SELECT * FROM work WHERE start::date >= '" +
          date +
          "' AND userid='" +
          userid +
          "' ORDER BY start",
      )
      .then(any => {
        winston.log(
          'info',
          'query getWorkFromDateToNow successfull, ' + any.length + ' items',
        );
        return any;
      })
      .catch(error => {
        winston.log('error', error.message);
      });
  },
};

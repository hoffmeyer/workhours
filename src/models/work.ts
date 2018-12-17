import { inspect } from 'util';
import { log } from 'winston';
import * as uuidV4 from 'uuid/v4';
import { Work } from '../types';
import db from '../db.js';

export default {
  fromDate: (date: string, userId: string): Promise<void | Work> => {
    return db
      .oneOrNone(
        "SELECT * FROM work where start::date = '" +
        date +
        "' AND userid = '" +
        userId +
        "' ORDER BY start DESC LIMIT 1",
      )
      .then(one => {
        log('info', 'getWorkFromDate returned: ' + inspect(one));
        return one;
      })
      .catch(error => {
        log('error', 'getWorkFromDate from db failed: ' + error);
        log('error', error.message);
      });
  },
  get: (userId: string, from: Date, to: Date): Promise<void | Work> => {
    return db
      .oneOrNone(
        "SELECT * FROM work where start::date >= '" +
        from.toISOString() +
        "' AND start::date <=  '" +
        to.toISOString() +
        "' AND userid = '" +
        userId +
        "' ORDER BY start",
      )
      .then(one => {
        log('info', 'getWorkFromDate returned: ' + inspect(one));
        return one;
      })
      .catch(error => {
        log('error', 'getWorkFromDate from db failed: ' + error);
        log('error', error.message);
      });
  },

  fromId: (id: string): Promise<void | Work> => {
    return db
      .oneOrNone(
        "SELECT * FROM work where id = '" +
        id +
        "' ORDER BY start DESC LIMIT 1",
      )
      .then(one => {
        log('info', 'getWorkFromId returned: ' + inspect(one));
        return one;
      })
      .catch(error => {
        log('error', 'getWorkFromId from db failed: ' + error);
        log('error', error.message);
      });
  },

  insert: (work: Work): Promise<string> => {
    return db
      .one(
        'INSERT INTO work(id, start, duration, lunch, userid) VALUES($1, $2, $3, $4, $5) RETURNING id;', [uuidV4(), work.start, work.duration, work.lunch, work.userid],
      )
      .then(id => {
        log('info', 'Insert successfull');
        return id.id;
      })
      .catch(error => {
        log('error', 'insertWork db failed: ' + error);
        log('error', error.message);
      });
  },

  delete: (id: string): Promise<void> => {
    return db
      .none('DELETE from work where id = $1;', [id])
      .then(() => {
        log('info', 'deleted work with id: ' + id);
      })
      .catch(error => {
        log(
          'error',
          'Deleting work with id ' + id + ' failed: ' + error,
        );
        log('error', error.message);
      });
  },

  update: (work: Work): Promise<void> => {
    return db
      .none(
        'UPDATE work SET start=$2, duration=$3, lunch=$4, userid=$5 WHERE id=$1;', [work.id, work.start, work.duration, work.lunch, work.userid],
      )
      .then(() => {
        log('info', 'updateWork successfull');
      })
      .catch(error => {
        log(
          'error',
          'Updating work with id ' +
          (work.id != null ? work.id : 'null') +
          ' failed: ' +
          error,
        );
        log('error', error.message);
      });
  },

  all: (userid: string): Promise<void | Work[]> => {
    return db
      .any(
        "SELECT * FROM work WHERE userid='" + userid + "' AND start::date >= '2019-01-01' ORDER BY start DESC",
      )
      .then(any => {
        log(
          'info',
          'query all work successfull, ' + any.length + ' items',
        );
        return any;
      })
      .catch(error => {
        log('error', error.message);
      });
  },

  fromDateToNow: (date: string, userid: string): Promise<void | Work[]> => {
    return db
      .any(
        "SELECT * FROM work WHERE start::date >= '" +
        date +
        "' AND userid='" +
        userid +
        "' ORDER BY start DESC",
      )
      .then(any => {
        log(
          'info',
          'query getWorkFromDateToNow successfull, ' + any.length + ' items',
        );
        return any;
      })
      .catch(error => {
        log('error', error.message);
      });
  },
};
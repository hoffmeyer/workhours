// @flow
import util from 'util';

import winston from 'winston';
import uuidV4 from 'uuid/v4';

import type {
    Work
} from '../types';
import db from '../db.js';

export default {
    all: (): Promise < Array < Work >> => {
        return db
            .any(
                "SELECT * FROM work ORDER BY start DESC",
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
    fromDateToNow: (date: string): Promise < Array < Work >> => {
        return db
            .any(
                "SELECT * FROM work WHERE start::date >= '" +
                date +
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
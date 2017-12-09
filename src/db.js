// @flow
import util from 'util';

import pgp from 'pg-promise';
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

export default db;

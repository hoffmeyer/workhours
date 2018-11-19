import * as pgp from 'pg-promise';
import {log} from 'winston';

const options = {};

const connection = {
  host: process.env.DATABASE_HOST || 'localhost',
  port: +process.env.DATABASE_PORT || 5432,
  database: process.env.DATBASE_NAME || 'postgres',
  user: process.env.DATABASE_USERNAME || 'postgres',
  password: process.env.DATABASE_PASSWORD || 'postgres',
};

log(
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

log('info', 'Database initialized');

export default db;

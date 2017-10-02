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

export default pgp(options)(connection);

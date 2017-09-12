// @flow
import pgp from 'pg-promise';

const username = 'workhours'
const password = 'workhours';
const host = 'localhost';
const port = 'port';
const database = 'database';

export default pgp('postgres://' + username + ':' + password + '@' + host + ':' + port + '/' + database);



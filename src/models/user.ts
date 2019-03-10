import * as winston from "winston";
import { User } from "../types";
import db from "../db.js";
import { log } from 'winston';

const byId = (id: string): Promise<User> => {
  return db
    .one("SELECT * FROM users WHERE id = '" + id + "'")
    .then(user => {
      winston.log(
        "info",
        "found user by id: " + user.id + " name: " + user.username
      );
      return user;
    })
    .catch(error => {
      winston.log("error", error.message);
    });
};

const authenticate = (username: string, password: string): Promise<User> => {
  return db
    .one("SELECT * FROM users WHERE username = '" + username + "' AND password = crypt('" + password + "', password)")
    .then(user => {
      winston.log(
        "info",
        "authenticated user with usernamme: " + username + " name: " + user.id
      );
      return user;
    })
    .catch(error => {
      winston.log("error", error.message);
    });
};

const update = (user: User): Promise<void> => {
  console.log("db user");
  console.log(user);
  return db
    .none(
      'UPDATE users SET balancefrom=$2, workHoursPerWeek=$3 WHERE id=$1;', [user.id, user.balancefrom, user.workhoursperweek],
    )
    .then((user) => {
      log('info', 'update User successfull');
    })
    .catch(error => {
      log(
        'error',
        'Updating user with id ' +
        (user.id != null ? user.id : 'null') +
        ' failed: ' +
        error,
      );
      log('error', error.message);
    });
};

export default {
  byId,
  authenticate,
  update
};

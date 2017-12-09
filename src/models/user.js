// @flow

import winston from "winston";

import type { User } from "../types";
import db from "../db.js";

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

const byUsername = (username: string): Promise<User> => {
  return db
    .one("SELECT * FROM users WHERE username = '" + username + "'")
    .then(user => {
      winston.log(
        "info",
        "found user by usernamme: " + username + " name: " + user.id
      );
      return user;
    })
    .catch(error => {
      winston.log("error", error.message);
    });
};

export default {
  byId,
  byUsername
};

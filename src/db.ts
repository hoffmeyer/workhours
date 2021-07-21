import * as pgp from "pg-promise";
import log from "./util/logger";

const options = {};

const connection = {
  host: process.env.DATABASE_HOST || "localhost",
  port: +process.env.DATABASE_PORT || 5432,
  database: process.env.DATBASE_NAME || "postgres",
  user: process.env.DATABASE_USERNAME || "postgres",
  password: process.env.DATABASE_PASSWORD || "postgres",
};

log.info(
  "Initializing database: " +
    connection.host +
    ":" +
    connection.port +
    " db: " +
    connection.database +
    " user: " +
    connection.user
);

const db = pgp(options)(connection);

db.connect()
  .then(function(obj) {
    log.info("Database connection test successfull");
    obj.done(); // success, release connection;
  })
  .catch(function(error) {
    log.error("Could not connect to database");
    log.error(error.message);
  });

log.info("Database initialized");

export default db;

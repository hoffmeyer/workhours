// @flow
import express from "express";
import path from "path";
import favicon from "serve-favicon";
import logger from "morgan";
import cookieParser from "cookie-parser";
import bodyParser from "body-parser";
import db from "./db.js";

import index from "./routes/index";
import users from "./routes/users";
import register from "./routes/register";

const app = express();

// view engine setup
app.set("views", path.join(__dirname, "../views"));
app.set("view engine", "pug");

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger("dev"));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, "public")));

app.use("/", index);
app.use("/users", users);
app.use("/register", register);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  const err = new Error("Not Found");
  //err.status = 404;
  next(err);
});

// error handler
app.use(function(err: Error, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get("env") === "development" ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render("error");
});

db.none(
  "CREATE TABLE IF NOT EXISTS hours (id uuid PRIMARY KEY NOT NULL, startDate timestamp NOT NULL, endDate timestamp);"
);

export default app;

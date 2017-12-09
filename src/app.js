// @flow
import fs from "fs";
import path from "path";

import bodyParser from "body-parser";
import cookieParser from "cookie-parser";
import express from "express";
import logger from "morgan";
import passport from "passport";
import passportLocal from "passport-local";
import session from "express-session";
import winston from "winston";

import db from "./db.js";
import deleteWork from "./routes/delete";
import index from "./routes/index";
import list from "./routes/list";
import login from "./routes/login";
import mUser from "./models/user";
import register from "./routes/register";
import work from "./routes/work";

winston.remove(winston.transports.Console);
winston.add(winston.transports.Console, { timestamp: true });
winston.level = process.env.LOG_LEVEL || "info";

const app = express();
const LocalStrategy = passportLocal.Strategy;

// view engine setup
app.set("views", path.join(__dirname, "../views"));
app.set("view engine", "pug");

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger("dev"));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.set("trust proxy", 1); // trust first proxy
app.use(
  session({
    secret: "valid unicorn",
    resave: false,
    saveUninitialized: false
  })
);
app.use(passport.initialize());
app.use(passport.session());
app.use(express.static(path.join(__dirname, "public")));

passport.serializeUser(function(user, done) {
  console.log("serializing auth");
  done(null, user.id);
});

passport.deserializeUser(function(id, done) {
  console.log("deserializing auth");
  mUser
    .byId(id)
    .then(user => done(null, user))
    .catch(e => done(e));
});

passport.use(
  new LocalStrategy({ passReqToCallback: true }, function(
    req,
    username,
    password,
    done
  ) {
    mUser
      .byUsername(username)
      .then(function(user) {
        if (!user) {
          return done(null, false);
        }
        if (user.password === password) {
          return done(null, user);
        }
        return done(null, false);
      })
      .catch(err => {
        return done(err);
      });
  })
);

app.use("/", index);
app.use("/register", register);
app.use("/work", work);
app.use("/list", list);
app.use("/delete", deleteWork);
app.use("/login", login);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  const err = new Error("Not Found");
  next(err);
});

// error handler
app.use(function(err: Error, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get("env") === "development" ? err : {};

  // render the error page
  res.status(404);
  res.render("error");
});

const sql = fs.readFileSync(path.join(__dirname, "..", "database.sql"), "utf8");
db.none(sql).catch(e => {
  winston.log("error", "" + e);
});

export default app;

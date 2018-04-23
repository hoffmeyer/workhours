import 'source-map-support/register';
import {readFileSync} from "fs";
import {join} from "path";

import * as bodyParser from "body-parser";
import * as cookieParser from "cookie-parser";
import * as express from "express";
import * as logger from "morgan";
import * as passport from "passport";
import {Strategy as LocalStrategy} from "passport-local";
import * as session from "express-session";
import {log, add, remove, transports} from "winston";

import db from "./db.js";
import deleteWork from "./routes/delete";
import index from "./routes/index";
import list from "./routes/list";
import login from "./routes/login";
import mUser from "./models/user";
import register from "./routes/register";
import work from "./routes/work";

import apiTest from "./routes/api/test";
import apiWork from "./routes/api/work";

remove(transports.Console);
add(transports.Console, { timestamp: true });
//level = process.env.LOG_LEVEL || "info";

const app = express();

// view engine setup
app.set("views", join(__dirname, "../views"));
app.set("view engine", "pug");

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger("dev"));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
  extended: false
}));
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
app.use(express.static(join(__dirname, "public")));

passport.serializeUser(function (user, done) {
  console.log("serializing auth");
  done(null, user.id);
});

passport.deserializeUser(function (id, done) {
  console.log("deserializing auth");
  mUser
    .byId(id)
    .then(user => done(null, user))
    .catch(e => done(e));
});

passport.use(
  new LocalStrategy({
    passReqToCallback: true
  }, function (
    req,
    username,
    password,
    done
  ) {
    mUser
      .byUsername(username)
      .then(function (user) {
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

app.use("/api/test", apiTest);
app.use("/api/work", apiWork);

// catch 404 and forward to error handler
app.use(function (req, res, next) {
  const err = new Error("Not Found");
  next(err);
});

// error handler
app.use(function (err: Error, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get("env") === "development" ? err : {};

  // render the error page
  res.status(404);
  res.render("error");
});

const sql = readFileSync(join(__dirname, "..", "database.sql"), "utf8");
db.none(sql).catch(e => {
  log("error", "" + e);
});

export default app;
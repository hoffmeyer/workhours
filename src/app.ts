import 'source-map-support/register';
import { readFileSync } from "fs";
import { join } from "path";
import * as bodyParser from "body-parser";
import * as cookieParser from "cookie-parser";
import * as express from "express";
import * as logger from "morgan";
import * as passport from "passport";
import { Strategy as LocalStrategy } from "passport-local";
import * as session from "express-session";
import { log, add, remove, transports } from "winston";
import * as path from "path";
import db from "./db.js";
import mUser from "./models/user";
import apiWork from "./routes/api/work";
import apiLogin from "./routes/api/login";
import apiUser from "./routes/api/user";

remove(transports.Console);
add(transports.Console, { timestamp: true });

const app = express();

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
  'login',
  new LocalStrategy({
    session: false
  }, function (username, password, done) {
    mUser
      .authenticate(username, password)
      .then(function (user) {
        if (user == null) {
          return done(null, false, { message: 'authentication failed' });
        }
        return done(null, user);
      })
      .catch(err => {
        return done(err);
      });
  })
);

app.use("/api/work", apiWork);
app.use("/api/login", apiLogin);
app.use("/api/user", apiUser);
app.use("*", function (req, resp) {
  resp.sendFile(path.resolve(__dirname, 'public/index.html'));
});

const sql = readFileSync(join(__dirname, "..", "database.sql"), "utf8");
log("info", "Migration script start")
db.none(sql).then(r => {
  log("info", "Migration script end")
}).catch(e => {
  log("error", "" + e);
});

export default app;
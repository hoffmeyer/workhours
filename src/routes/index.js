// @flow
import { Router } from "express";
import moment from "moment";
import type { Work } from "../types";
import db from "../db.js";
let router = Router();

const getWorkHoursAt = (date: Date): Work => {
  const fromDb = getWorkhoursFromDb(date);
  if (fromDb != null) {
    return fromDb;
  } else {
    return getNewWorkHours(date);
  }
};

const getWorkhoursFromDb = (date: Date): ?Work => {
  db
    .oneOrNone(
      "SELECT * FROM hours where startDate = " +
        date.toString() +
        "ORDER BY startDate DESC LIMIT 1"
    )
    .then(data => {})
    .catch(error => {});
};

const getNewWorkHours = (date: Date): Work => {
  const day = moment(date).format("YYYY-MM-DD");
  const time = moment(date).format("HH:mm");

  const work: Work = {
    startDate: day,
    startTime: time,
    hours: 0,
    minutes: 0,
    lunch: 30
  };
  return work;
};

router.get("/", function(req, res, next) {
  res.render("index", getWorkHoursAt(new Date()));
});

export default router;

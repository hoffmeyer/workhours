// @flow
import { Router } from "express";
import moment from "moment";
let router = Router();

router.get("/", function(req, res, next) {
  const date = new Date();
  const today = moment().format("YYYY-MM-DD");
  const now = moment().format("HH:mm");

  const time: {
    startDate: string,
    startTime: string,
    hours: number,
    minutes: number,
    lunch: number
  } = {
    startDate: today,
    startTime: now,
    hours: 0,
    minutes: 0,
    lunch: 30
  };

  res.render("index", time);
});

export default router;

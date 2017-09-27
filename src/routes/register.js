// @flow
import { Router } from "express";
import type { Work } from "../types";
import db from "../db.js";
import uuidV4 from "uuid/v4";

const router = Router();

const combineDateAndTime = (date: string, time: string): Date => {
  return new Date(date + " " + time);
};

router.post("/", function(req, res, next) {
  const work: Work = req.body;

  db
    .none("INSERT INTO hours(id, start, duration) VALUES($1, $2, $3)", [
      uuidV4(),
      new Date(work.startDate + " " + work.startTime),
      work.hours
    ])
    .then(() => {
      console.log("Insert successfull");
      res.redirect("/?sucess=true");
    })
    .catch(error => {
      console.log("Inserting hours into db failed: " + error);
      console.log(error.message);
    });
});

export default router;

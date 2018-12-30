/*open Jest;*/

open Types;

open Js.Date;

let createWorkOnDate = (id, date) => {
  id,
  start: date,
  duration: 1.,
  lunch: 0.,
  userid: Some("testerman"),
};

let dateHelp = makeWithYMDHM(~year=2018., ~month=6., ~hours=8., ~minutes=0.);
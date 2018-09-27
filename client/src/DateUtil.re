open Js.Date;

let addDays = (date, days) => {
  let newDate = fromFloat(valueOf(date));
  setDate(newDate, getDate(newDate) +. days) |> ignore;
  newDate;
};

let startOfWeek = date => {
  let dayOfWeek = getDay(date);
  let day = addDays(date, dayOfWeek *. (-1.));
  setHoursMSMs(
    day,
    ~hours=0.,
    ~minutes=0.,
    ~seconds=0.,
    ~milliseconds=0.,
    (),
  ) |> ignore;
  day;
};

let endOfWeek = date => {
  let dayOfWeek = getDay(date);
  let day = addDays(date, 6. -. dayOfWeek);
  setHoursMSMs(
    day,
    ~hours=13.,
    ~minutes=59.,
    ~seconds=59.,
    ~milliseconds=999.,
    (),
  ) |> ignore;
  day;
};

let isBefore = (d1, d2) => valueOf(d1) > valueOf(d2);

let isAfter = (d1, d2) => valueOf(d1) < valueOf(d2);

let isSameDay = (d1, d2) =>
  getFullYear(d1) == getFullYear(d2)
  && getMonth(d1) == getMonth(d2)
  && getDay(d1) == getDay(d2);

let compareAsc = (d1: Js.Date.t, d2: Js.Date.t) =>
  if (d1 > d2) {
    1;
  } else if (d1 < d2) {
    (-1);
  } else {
    0;
  };

let compareDesc = (d1, d2) => compareAsc(d2, d1);
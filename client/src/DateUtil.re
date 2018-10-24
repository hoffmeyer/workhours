open Js.Date;

let addDays = (date, days) => {
  let newDate = fromFloat(valueOf(date));
  setDate(newDate, getDate(newDate) +. days) |> ignore;
  newDate;
};

let startOfWeek = date => {
  let dayOfWeek = getDay(date);
  let day = addDays(date, dayOfWeek *. (-1.));
  setHoursMSMs(day, ~hours=0., ~minutes=0., ~seconds=0., ~milliseconds=0., ())
  |> ignore;
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
  )
  |> ignore;
  day;
};

let isBefore = (d1, d2) => valueOf(d1) > valueOf(d2);

let isAfter = (d1, d2) => valueOf(d1) < valueOf(d2);

let isSameDay = (d1, d2) =>
  getFullYear(d1) == getFullYear(d2)
  && getMonth(d1) == getMonth(d2)
  && getDay(d1) == getDay(d2);

let compareAsc = (d1: t, d2: t) =>
  if (d1 > d2) {
    1;
  } else if (d1 < d2) {
    (-1);
  } else {
    0;
  };

let compareDesc = (d1, d2) => compareAsc(d2, d1);

let dateToWeekNo = date => {
  /* Copy date so don't modify original */
  let d =
    makeWithYMD(
      ~year=getFullYear(date),
      ~month=getMonth(date),
      ~date=getDate(date),
      (),
    );
  /* Set to nearest Thursday: current date + 4 - current day number */
  /* Make Sunday's day number 7 */
  let dayOfWeek = getUTCDay(d);
  Js.log("dayOfWeek: " ++ Js.Float.toString(dayOfWeek));
  setUTCDate(d, getUTCDate(d) +. 4. -. dayOfWeek )
  |> ignore;
  /* Get first day of year */
  let yearStart =
    makeWithYMD(~year=getUTCFullYear(date), ~month=0., ~date=1., ());
  Js.log("yearStart: " ++ Js.Date.toLocaleString(yearStart));
  /* Calculate full weeks to nearest Thursday */
  let weekNo =
    Js.Math.ceil(
      ((valueOf(d) -. valueOf(yearStart)) /. 86400000. +. 1.) /. 7.,
    );
  /* Return array of year and week number */
  Js.log("Weekno: " ++ Js.Int.toString(weekNo));
  weekNo;
};
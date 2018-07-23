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
  );
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
  );
  day;
};

let isBefore = (date1, date2) => valueOf(date1) > valueOf(date2);

let isAfter = (date1, date2) => valueOf(date1) < valueOf(date2);
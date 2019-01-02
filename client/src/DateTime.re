type dt = {
  .
  "year": float,
  "day": float,
  "month": float,
  "hour": float,
  "minute": float,
  "second": float,
  "millisecond": float,
};

[@bs.module "luxon"] [@bs.scope "DateTime"]
external local:
  (
    ~year: float,
    ~month: float,
    ~day: float,
    ~hour: float,
    ~minute: float,
    ~second: float,
    ~millisecond: float,
    unit
  ) =>
  dt =
  "";

[@bs.module "luxon"] [@bs.scope "DateTime"]
external fromJSDate: Js.Date.t => dt = "";

[@bs.send.pipe: dt] external toISO: unit => string = "";
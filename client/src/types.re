type work = {
  id: option(string),
  start: Js.Date.t,
  duration: float,
  lunch: float,
  userid: option(string),
};

type user = {
  id: string,
  username: string,
  name: string,
  workhoursPerWeek: float,
  balanceFrom: Js.Date.t,
};

module Decode = {
  let work = (json): work =>
    Json.Decode.{
      id: json |> optional(field("id", string)),
      start: json |> field("start", date),
      duration: json |> field("duration", Json.Decode.float),
      lunch: json |> field("lunch", Json.Decode.float),
      userid: json |> optional(field("userid", string)),
    };
  let workList = (json): array(work) => Json.Decode.(json |> array(work));
  let balance = (json): float =>
    json |> Json.Decode.field("balance", Json.Decode.float);
  let user = (json): user =>
    Json.Decode.{
      id: json |> field("id", string),
      username: json |> field("username", string),
      name: json |> field("name", string),
      workhoursPerWeek: json |> field("workhoursperweek", Json.Decode.float),
      balanceFrom: json |> field("balancefrom", Json.Decode.date),
    };
};

module Encode = {
  let work = (w: work) =>
    Json.Encode.(
      object_([
        ("id", nullable(string, w.id)),
        ("start", date(w.start)),
        ("duration", Json.Encode.float(w.duration)),
        ("lunch", Json.Encode.float(w.lunch)),
        ("userid", nullable(string, w.userid)),
      ])
    );
  let user = (u: user) =>
    Json.Encode.(
      object_([
        ("id", string(u.id)),
        ("name", string(u.name)),
        ("username", string(u.username)),
        ("workhoursperweek", Json.Encode.float(u.workhoursPerWeek)),
        ("balancefrom", date(u.balanceFrom)),
      ])
    );
};

type action =
  | WorkFetch
  | WorkAdd(work)
  | WorkUpdate(work)
  | WorkFetched(array(work))
  | WorkFetchFailed(string)
  | WorkDelete(string);
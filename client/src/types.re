type work = {
  id: option(string),
  start: Js.Date.t,
  duration: float,
  lunch: float,
  userid: option(string),
};

module Decode = {
  let work = json: work =>
    Json.Decode.{
      id: json |> optional(field("id", string)),
      start: json |> field("start", date),
      duration: json |> field("duration", Json.Decode.float),
      lunch: json |> field("lunch", Json.Decode.float),
      userid: json |> optional(field("userid", string)),
    };
  let workList = json: array(work) => Json.Decode.(json |> array(work));
  let balance = json: float =>
    json |> Json.Decode.field("balance", Json.Decode.float);
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
};

type action =
  | WorkFetch
  | WorkAdd(work)
  | WorkUpdate(work)
  | WorkFetched(array(work))
  | WorkFetchFailed(string)
  | WorkDelete(string);
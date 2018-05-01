open Types;

type state =
  | Loading
  | Error
  | Loaded(array(work));

type action =
  | WorkFetch
  | WorkFetched(array(work))
  | WorkFetchFailed;

let str = ReasonReact.stringToElement;

module Decode = {
  let work = json : work =>
    Json.Decode.{
      id: json |> field("id", string),
      start: json |> field("start", date),
      duration: json |> field("duration", Json.Decode.float),
      lunch: json |> field("lunch", Json.Decode.float),
      userid: json |> field("userid", string),
    };
  let workList = json : array(work) => Json.Decode.(json |> array(work));
};

let component = ReasonReact.reducerComponent("Work");

let listWork = workList =>
  <ul>
    (
      Belt.Array.map(workList, work =>
        <li key=work.id>
          (
            ReasonReact.stringToElement(
              DateFns.format("dddd DD/MM/YY", work.start),
            )
          )
        </li>
      )
      |> ReasonReact.arrayToElement
    )
  </ul>;

let workListToCurrentWeek = (workList: array(work)) =>
  switch (workList) {
  | [||] => workList
  | xs =>
    let today = Js.Date.make();
    let startOfWeek = today |> DateFns.startOfWeek;
    let endOfWeek = today |> DateFns.endOfWeek;
    let isBeforeThisWeek = date => date |> DateFns.isBefore(startOfWeek);
    let isAfterThisWeek = date => date |> DateFns.isAfter(endOfWeek);
    let inThisWeek = date =>
      ! isBeforeThisWeek(date) && ! isAfterThisWeek(date);
    xs |> Js.Array.filter(work => inThisWeek(work.start));
  };

let workArrayToHours = (workList: array(work)) =>
  Js.Array.reduce(
    (sum, work) => sum +. work.duration,
    0.,
    workListToCurrentWeek(workList),
  );

let make = _children => {
  ...component,
  initialState: _state => Loading,
  reducer: (action, _state) =>
    switch (action) {
    | WorkFetch =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (
          self =>
            Js.Promise.(
              Fetch.fetch("api/work/latest")
              |> then_(Fetch.Response.json)
              |> then_(json =>
                   json
                   |> Decode.workList
                   |> (work => self.send(WorkFetched(work)))
                   |> resolve
                 )
              |> catch(_err =>
                   Js.Promise.resolve(self.send(WorkFetchFailed))
                 )
              |> ignore
            )
        ),
      )
    | WorkFetched(work) => ReasonReact.Update(Loaded(work))
    | WorkFetchFailed => ReasonReact.Update(Error)
    },
  didMount: self => {
    self.send(WorkFetch);
    ReasonReact.NoUpdate;
  },
  render: self =>
    switch (self.state) {
    | Error => <div> ("An error occurred!" |> str) </div>
    | Loading => <div> ("Loading..." |> str) </div>
    | Loaded(workList) =>
      <div>
        <h1> ("Work" |> str) </h1>
        (listWork(workList))
        <h2> ("Hours this week" |> str) </h2>
        <p> (workList |> workArrayToHours |> Js.Float.toString |> str) </p>
      </div>
    },
};
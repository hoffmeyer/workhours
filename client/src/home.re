open Types;

type state =
  | Initial
  | Starting
  | Started(work)
  | Stopping
  | Stopped(work)
  | Error(string);

type action =
  | StopWork
  | WorkStopped(work)
  | WorkStopFailed
  | StartWork
  | WorkStarted(work)
  | WorkStartFailed;

let str = ReasonReact.string;

let unfinishedWork = workList : array(work) =>
  Js.Array.filter(w => w.duration == 0., workList);

let latestOnDate = (date, workList) =>
  workList
  |> Js.Array.filter(w => DateUtil.isSameDay(w.start, date))
  |> Js.Array.sortInPlaceWith((w1, w2) =>
       DateUtil.compareAsc(w1.start, w2.start)
     )
  |> Js.Array.pop;

let diffInHours = (d1, d2) =>
  (Js.Date.getTime(d2) -. Js.Date.getTime(d1)) /. 1000. /. 60. /. 60.;

let roundToQuarters = v => Js.Math.round(v *. 4.) /. 4.;

let dateToDiff = d =>
  diffInHours(d, Js.Date.make()) |> roundToQuarters |> string_of_float;

let inProgressWorkToday = l =>
  l |> unfinishedWork |> latestOnDate(Js.Date.make());

let component = ReasonReact.reducerComponent("Home");

let make = (~workList: array(work), _children) => {
  ...component,
  initialState: _state => Initial,
  reducer: (action, _state) =>
    switch (action) {
    | StopWork => ReasonReact.Update(Initial)
    | WorkStopped(_work) => ReasonReact.Update(Initial)
    | WorkStopFailed => ReasonReact.Update(Initial)
    | StartWork => ReasonReact.Update(Initial)
    | WorkStarted(_work) => ReasonReact.Update(Initial)
    | WorkStartFailed => ReasonReact.Update(Initial)
    },
  render: _self =>
    <div>
      <h1> (str("Home page")) </h1>
      (
        switch (workList |> inProgressWorkToday) {
        | None =>
          <div>
            <p> (str("You are not working")) </p>
            <button onClick=(_evt => Js.log("Starting work..."))>
              (str("Start work"))
            </button>
          </div>
        | Some(w) =>
          <div>
            <p> (str((w.start |> dateToDiff) ++ " hours and counting")) </p>
            <button> (str("Stop work")) </button>
          </div>
        }
      )
    </div>,
};
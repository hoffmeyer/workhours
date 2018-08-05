open Types;

let component = ReasonReact.statelessComponent("Home");

let str = ReasonReact.stringToElement;

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

let make = (~workList: array(work), _children) => {
  ...component,
  render: _self =>
    <div>
      <h1> (str("Home page")) </h1>
      (
        switch (workList |> unfinishedWork |> latestOnDate(Js.Date.make())) {
        | None => <p> (str("You are not working")) </p>
        | Some(w) =>
          <p>
            (
              str(
                string_of_float(
                  diffInHours(w.start, Js.Date.make()) |> roundToQuarters,
                )
                ++ " hours and counting",
              )
            )
          </p>
        }
      )
      <button> (str("Start work")) </button>
    </div>,
};
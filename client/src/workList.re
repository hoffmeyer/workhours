open Types;

let str = ReasonReact.string;

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

let component = ReasonReact.statelessComponent("WorkList");

let listWork = workList =>
  <table>
    <tbody>
      <tr>
        <th> ("Start" |> str) </th>
        <th> ("Hours" |> str) </th>
        <th> ("Break" |> str) </th>
        <th> ("Total" |> str) </th>
        <th />
        <th />
      </tr>
      (
        Belt.Array.map(workList, work =>
          <tr key=work.id>
            <td> (work.start |> Js.Date.toString |> str) </td>
            <td> (work.duration |> Js.Float.toString |> str) </td>
            <td> (work.lunch |> Js.Float.toString |> str) </td>
            <td>
              (work.duration -. work.lunch |> Js.Float.toString |> str)
            </td>
            <td> <a href="/edit"> (ReasonReact.string("E")) </a> </td>
          </tr>
        )
        |> ReasonReact.array
      )
    </tbody>
  </table>;

let workListToCurrentWeek = (today, workList: list(work)) => {
  let startOfWeek = today |> DateUtil.startOfWeek;
  let endOfWeek = today |> DateUtil.endOfWeek;
  let isBeforeThisWeek = date => date |> DateUtil.isBefore(startOfWeek);
  let isAfterThisWeek = date => date |> DateUtil.isAfter(endOfWeek);
  let inThisWeek = date =>
    ! isBeforeThisWeek(date) && ! isAfterThisWeek(date);
  workList |> List.filter(work => inThisWeek(work.start));
};

let workArrayToHours = (workList: list(work)) =>
  List.fold_left((sum, work) => sum +. work.duration, 0., workList);

let make = (~workList, _children) => {
  ...component,
  render: _self =>
    <div>
      <h1> ("Work" |> str) </h1>
      (listWork(workList))
      <h2> ("Hours this week" |> str) </h2>
      <p>
        (
          workList
          |> Array.to_list
          |> workListToCurrentWeek(Js.Date.make())
          |> workArrayToHours
          |> Js.Float.toString
          |> str
        )
      </p>
    </div>,
};
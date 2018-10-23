open Types;

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("WorkList");

let listWork = workList =>
  <div>
    {
      switch (Belt.List.head(workList)) {
      | None => <p> {"Error" |> str} </p>
      | Some(x) =>
        <h3> {DateUtil.dateToWeekNo(x.start) |> Js.Int.toString |> str} </h3>
      }
    }
    <table>
      <tbody>
        <tr>
          <th> {"Start" |> str} </th>
          <th> {"Hours" |> str} </th>
          <th> {"Break" |> str} </th>
          <th> {"Total" |> str} </th>
          <th />
          <th />
        </tr>
        {
          Belt.List.map(workList, work =>
            <tr
              key={
                switch (work.id) {
                | Some(v) => v
                | None => "0"
                }
              }>
              <td> {work.start |> Js.Date.toString |> str} </td>
              <td> {work.duration |> Js.Float.toString |> str} </td>
              <td> {work.lunch |> Js.Float.toString |> str} </td>
              <td>
                {work.duration -. work.lunch |> Js.Float.toString |> str}
              </td>
              <td> <a href="/edit"> {ReasonReact.string("E")} </a> </td>
            </tr>
          )
          |> Belt.List.toArray
          |> ReasonReact.array
        }
      </tbody>
    </table>
  </div>;

let workListToCurrentWeek = (today, workList: list(work)) => {
  let startOfWeek = today |> DateUtil.startOfWeek;
  let endOfWeek = today |> DateUtil.endOfWeek;
  let isBeforeThisWeek = date => date |> DateUtil.isBefore(startOfWeek);
  let isAfterThisWeek = date => date |> DateUtil.isAfter(endOfWeek);
  let inThisWeek = date => !isBeforeThisWeek(date) && !isAfterThisWeek(date);
  workList |> List.filter(work => inThisWeek(work.start));
};

let rec group = (list: list('a), p): list(list('a)) =>
  switch (Belt.List.head(list)) {
  | None => []
  | Some(b) =>
    let comp = x => p(b) == p(x);
    let (p1, p2) = Belt.List.partition(list, comp);
    Belt.List.concat([p1], group(p2, p));
  };

let x = (workList: array(work)) =>
  Js.Array.map(x => (DateUtil.dateToWeekNo(x.start), x), workList);

let workArrayToHours = (workList: list(work)) =>
  List.fold_left((sum, work) => sum +. work.duration, 0., workList);

let groupWorkByWeek = workList =>
  group(workList, x => DateUtil.dateToWeekNo(x.start));

let test = workList => groupWorkByWeek(workList);
let test2 = (workList, p) => Belt.List.map(test(workList), p);

let make = (~workList, _children) => {
  ...component,
  render: _self =>
    <div>
      <h1> {"Work" |> str} </h1>
      <h2> {"Hours this week" |> str} </h2>
      <p>
        {
          workList
          |> Array.to_list
          |> workListToCurrentWeek(Js.Date.make())
          |> workArrayToHours
          |> Js.Float.toString
          |> str
        }
      </p>
      {
        test2(Belt.List.fromArray(workList), listWork)
        |> Belt.List.toArray
        |> ReasonReact.array
      }
    </div>,
};
open Types;

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("WorkList");

let formatDate = date => Js.Date.toUTCString(date);

let listWork = workList =>
  <div>
  {
    switch(Belt.List.head(workList)){
      | None => <p>{"Error" |> str}</p>
      | Some(x) => <h2>{"Week " ++ (DateUtil.dateToWeekNo(x.start) |> Js.Int.toString ) |> str}</h2>
    }
  }
  <table id="workList">
    <tbody>
      <tr className="borderBottom">
        <th className="left"> {"Start" |> str} </th>
        <th className="right"> {"Hours" |> str} </th>
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
            <td className="left"> {work.start |> formatDate |> str} </td>
            <td className="right">
              {work.duration -. work.lunch |> Js.Float.toString |> str}
            </td>
            <td> <a href="/edit"> {ReasonReact.string("E")} </a> </td>
            <td> <a href="/delete"> {ReasonReact.string("X")} </a> </td>
          </tr>
        )
        |> Belt.List.toArray |> ReasonReact.array
      }
      <tr className="borderBottom">
        <td className="bold"> {"Sum" |> str} </td>
        <td className="bold right"> { Belt.List.map(workList, work => (work.duration -. work.lunch)) |> x => Belt.List.reduce(x, 0., (x,y) => x+.y) |> Js.Float.toString |> str} </td>
        <td />
        <td />
      </tr>
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

let rec group = (list: list('a), p):list(list('a)) => {
  switch(Belt.List.head(list)){
    | None =>  []
    | Some(b) => {
      let comp = x => p(b) == p(x);
      let (p1, p2) = Belt.List.partition(list, comp );
      Belt.List.concat([p1], group(p2, p)); 
    }
  };
};

let x = (workList: array(work)) =>
  Js.Array.map(x => (DateUtil.dateToWeekNo(x.start), x), workList);

let workArrayToHours = (workList: list(work)) =>
  List.fold_left((sum, work) => sum +. work.duration, 0., workList);

let groupWorkByWeek = workList => {
group(workList, x => DateUtil.dateToWeekNo(x.start))
};

let test = workList => groupWorkByWeek(workList);
let test2 = (workList, p) => Belt.List.map(test(workList), p);

let make = (~workList, _children) => {
  ...component,
  render: _self =>
    <div>
      <p>
        {
          let hours = workList
          |> Array.to_list
          |> workListToCurrentWeek(Js.Date.make())
          |> workArrayToHours
          |> Js.Float.toString;

          ("Hours this week: " ++ hours) |> str;
        }
      </p>
      {
        test2(Belt.List.fromArray(workList), listWork) |> Belt.List.toArray |> ReasonReact.array
      }
    </div>,
};
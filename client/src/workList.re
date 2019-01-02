open Types;

let str = ReasonReact.string;

type state =
  | Initial
  | Deleting
  | Error(string);

type action =
  | Delete(work)
  | Edit(work)
  | Deleted
  | Failed(string);

let component = ReasonReact.reducerComponent("WorkList");

let formatDate = date => Js.Date.toLocaleDateString(date);

let listWork = (send: action => unit, workList) =>
  <div>
    {
      switch (Belt.List.head(workList)) {
      | None => <p> {"Error" |> str} </p>
      | Some(x) =>
        <h2>
          {
            "Week "
            ++ (DateUtil.dateToWeekNo(x.start) |> Js.Int.toString)
            |> str
          }
        </h2>
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
              <td>
                <a href="# " onClick={_event => send(Edit(work))}>
                  {ReasonReact.string("E")}
                </a>
              </td>
              <td>
                <a href="# " onClick={_event => send(Delete(work))}>
                  {ReasonReact.string("X")}
                </a>
              </td>
            </tr>
          )
          |> Belt.List.toArray
          |> ReasonReact.array
        }
        <tr className="borderTop">
          <td className="bold"> {"Sum" |> str} </td>
          <td className="bold right">
            {
              Belt.List.map(workList, work => work.duration -. work.lunch)
              |> (
                x =>
                  Belt.List.reduce(x, 0., (x, y) => x +. y)
                  |> Js.Float.toString
                  |> str
              )
            }
          </td>
          <td />
          <td />
        </tr>
      </tbody>
    </table>
  </div>;

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

let groupWorkByWeek = workList =>
  group(workList, x => DateUtil.dateToWeekNo(x.start));

let test = workList => groupWorkByWeek(workList);
let test2 = (workList, p) => Belt.List.map(test(workList), p);

let make = (~handleAction, ~workList, _children) => {
  ...component,
  initialState: () => Initial,
  reducer: (action, _state) =>
    switch (action) {
    | Delete(work) =>
      switch (work.id) {
      | None => ReasonReact.Update(Error("Cannot delete work with no Id"))
      | Some(id) =>
        ReasonReact.UpdateWithSideEffects(
          Deleting,
          (
            self =>
              Js.Promise.(
                Work.delete(id)
                |> then_(() => {
                     handleAction(Types.WorkDelete(id));
                     self.send(Deleted) |> resolve;
                   })
                |> catch(err => {
                     Js.log2("Error deleting work: ", err);
                     resolve(self.send(Failed("Error deleting work")));
                   })
                |> ignore
              )
          ),
        )
      }
    | Deleted => ReasonReact.Update(Initial)
    | Edit(work) =>
      ReasonReact.SideEffects(
        (
          _self =>
            Router.Edit(work.id)
            |> Router.routeToString
            |> ReasonReact.Router.push
        ),
      )
    | Failed(msg) => ReasonReact.Update(Error(msg))
    },
  render: self =>
    <div>
      {
        switch (self.state) {
        | Initial => <div />
        | Deleting => <p> {"Deleting...." |> str} </p>
        | Error(msg) => <p> {"Error: " ++ msg |> str} </p>
        }
      }
      <Balance />
      {
        test2(Belt.List.fromArray(workList), listWork(self.send))
        |> Belt.List.toArray
        |> ReasonReact.array
      }
    </div>,
};
open Types;

type state =
  | Loading
  | Error(string)
  | Loaded(array(work));

type action =
  | WorkFetch
  | WorkFetched(array(work))
  | WorkFetchFailed(string)
  | WorkEdit(string);

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

let component = ReasonReact.reducerComponent("WorkList");

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
              |> then_(res => {
                   let status = Fetch.Response.status(res);
                   switch (status) {
                   | 200 => Fetch.Response.json(res)
                   | 401 =>
                     ReasonReact.Router.push("/login");
                     Js.Exn.raiseError(Fetch.Response.statusText(res));
                   | _ => Js.Exn.raiseError(Fetch.Response.statusText(res))
                   };
                 })
              |> then_(json =>
                   json
                   |> Decode.workList
                   |> (work => self.send(WorkFetched(work)))
                   |> resolve
                 )
              |> catch(err => {
                   Js.log2("Error loading latest work: ", err);
                   resolve(self.send(WorkFetchFailed("Hi")));
                 })
              |> ignore
            )
        ),
      )
    | WorkFetched(work) => ReasonReact.Update(Loaded(work))
    | WorkFetchFailed(msg) => ReasonReact.Update(Error(msg))
    },
  didMount: self => self.send(WorkFetch),
  render: self =>
    switch (self.state) {
    | Error(msg) => <div> ("An error occurred: " ++ msg |> str) </div>
    | Loading => <div> ("Loading..." |> str) </div>
    | Loaded(workList) =>
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
      </div>
    },
};
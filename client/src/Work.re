open Belt;

type work = {
  id: string,
  start: string,
  duration: float,
  lunch: float,
  userid: string,
};

type state =
  | Loading
  | Error
  | Loaded(array(work));

type action =
  | WorkFetch
  | WorkFetched(array(work))
  | WorkFetchFailed;

module Decode = {
  let work = json : work =>
    Json.Decode.{
      id: json |> field("id", string),
      start: json |> field("start", string),
      duration: json |> field("duration", float),
      lunch: json |> field("lunch", float),
      userid: json |> field("userid", string),
    };
  let workList = json : array(work) => Json.Decode.(json |> array(work));
};

let component = ReasonReact.reducerComponent("Work");

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
              Fetch.fetch("api/work")
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
    | Error =>
      <div> (ReasonReact.stringToElement("An error occurred!")) </div>
    | Loading => <div> (ReasonReact.stringToElement("Loading...")) </div>
    | Loaded(workList) =>
      <div>
        <h1> (ReasonReact.stringToElement("Work")) </h1>
        <ul>
          (
            Array.map(workList, work =>
              <li key=work.id> (ReasonReact.stringToElement(work.start)) </li>
            )
            |> ReasonReact.arrayToElement
          )
        </ul>
      </div>
    },
};
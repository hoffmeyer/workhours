[%bs.raw {|require('./app.css')|}];

open Types;

type state =
  | Loading
  | Error(string)
  | Loaded(array(work));

type action =
  | WorkFetch
  | WorkFetched(array(work))
  | WorkFetchFailed(string);

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

let component = ReasonReact.reducerComponent("App");

let make = (~currentRoute, _children) => {
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
    <div className="App">
      <div className="App-header">
        <h2> (ReasonReact.string("Workhours")) </h2>
      </div>
      <div>
        <nav>
          <ul className="navigation">
            <li>
              <Router.NavLink route=Config.Home>
                (ReasonReact.string("Home"))
              </Router.NavLink>
            </li>
            <li>
              <Router.NavLink route=Config.Edit>
                (ReasonReact.string("Edit"))
              </Router.NavLink>
            </li>
            <li>
              <Router.NavLink route=Config.List>
                (ReasonReact.string("List"))
              </Router.NavLink>
            </li>
          </ul>
        </nav>
        <main>
          (
            switch (self.state) {
            | Loading => <p> (str("Loading")) </p>
            | Loaded(workList) =>
              <div> (Config.routeToComponent(currentRoute, workList)) </div>
            }
          )
        </main>
      </div>
    </div>,
};
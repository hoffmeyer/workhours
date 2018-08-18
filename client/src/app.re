[%bs.raw {|require('./app.css')|}];

open Types;

type state =
  | Loading
  | Error(string)
  | Loaded(array(work));

let str = ReasonReact.string;

let component = ReasonReact.reducerComponent("App");

let make = (~currentRoute, _children) => {
  ...component,
  initialState: _state => Loading,
  reducer: (action, state) =>
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
                   resolve(
                     self.send(
                       WorkFetchFailed("Error loading leatest work"),
                     ),
                   );
                 })
              |> ignore
            )
        ),
      )
    | WorkFetched(work) => ReasonReact.Update(Loaded(work))
    | WorkFetchFailed(msg) => ReasonReact.Update(Error(msg))
    | WorkAdd(work) =>
      switch (state) {
      | Loaded(workList) =>
        Js.Array.push(work, workList);
        ReasonReact.Update(Loaded(workList));
      | _ => ReasonReact.Update(Error("Failed to add new work"))
      }
    },
  didMount: self => {
    let handleAction = action => self.send(action);
    self.send(WorkFetch);
  },
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
              <div>
                (
                  Config.routeToComponent(currentRoute, workList, action =>
                    self.send(action)
                  )
                )
              </div>
            | Error(msg) => <div> (str(msg)) </div>
            }
          )
        </main>
      </div>
    </div>,
};
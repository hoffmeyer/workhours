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
              Fetch.fetch("/api/work/latest")
              |> then_(res => {
                   let status = Fetch.Response.status(res);
                   switch (status) {
                   | 200 => Fetch.Response.json(res)
                   | 401 =>
                     self.send(WorkFetched([||]))
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
              |> ignore
            )
        ),
      )
    | WorkFetched(work) => ReasonReact.Update(Loaded(work))
    | WorkFetchFailed(msg) => ReasonReact.Update(Error(msg))
    | WorkAdd(work) =>
      switch (state) {
      | Loaded(workList) =>
        Js.Array.push(work, workList) |> ignore;
        ReasonReact.Update(Loaded(workList));
      | _ => ReasonReact.Update(Error("Failed to add new work"))
      }
    | WorkUpdate(work) =>
      switch (state) {
      | Loaded(workList) =>
        let newList = Js.Array.map(w => if(w.id == work.id){
          work
        } else w, workList);
        ReasonReact.Update(Loaded(newList));
      | _ => ReasonReact.Update(Error("Failed to update work"))
      }
    },
  didMount: self => {
    self.send(WorkFetch);
  },
  render: self =>
    <div className="App">
      <header>
        <h1> (ReasonReact.string("Workhours")) </h1>
      </header>
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
        <section>
          (
            switch (self.state) {
            | Loading => <p> (str("Loading")) </p>
            | Loaded(workList) =>
              <div>
                (
                  Config.routeToComponent(currentRoute, ~workList= workList, ~handleAction=action =>
                    self.send(action)
                  )
                )
              </div>
            | Error(msg) => <div> (str(msg)) </div>
            }
          )
        </section>
      </div>
    </div>,
};
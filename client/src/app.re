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
        self =>
          Js.Promise.(
            Models.Work.getLatest
            |> then_(workList =>
                 workList
                 |> (work => self.send(WorkFetched(work)))
                 |> resolve
               )
            |> catch(_err => self.send(WorkFetched([||])) |> resolve)
            |> ignore
          ),
      )
    | WorkFetched(work) => ReasonReact.Update(Loaded(work))
    | WorkFetchFailed(msg) => ReasonReact.Update(Error(msg))
    | WorkAdd(work) =>
      switch (state) {
      | Loaded(workList) =>
        Js.Array.push(work, workList) |> ignore;
        workList
        |> Js.Array.sortInPlaceWith((x, y) => {
             let startTime = x => x.start->Js.Date.getTime;
             switch (x->startTime, y->startTime) {
             | (x, y) when x < y => 1
             | (x, y) when x > y => (-1)
             | _ => 0
             };
           })
        |> ignore;
        ReasonReact.Update(Loaded(workList));
      | _ => ReasonReact.Update(Error("Failed to add new work"))
      }
    | WorkUpdate((work: work)) =>
      switch (state) {
      | Loaded(workList) =>
        let newList =
          Js.Array.map(
            (w: work) =>
              if (w.id == work.id) {
                work;
              } else {
                w;
              },
            workList,
          );
        ReasonReact.Update(Loaded(newList));
      | _ => ReasonReact.Update(Error("Failed to update work"))
      }
    | WorkDelete(id) =>
      switch (state) {
      | Loaded(workList) =>
        let newList =
          Js.Array.filter(
            (w: work) =>
              switch (w.id) {
              | None => true
              | Some(wId) => wId !== id
              },
            workList,
          );
        ReasonReact.Update(Loaded(newList));
      | _ => ReasonReact.Update(Error("Failed to delete work"))
      }
    },
  didMount: self => self.send(WorkFetch),
  render: self =>
    <div className="App">
      <header>
        <div />
        <div
          className="logo"
          onClick={_event =>
            ReasonReact.Router.push(Router.routeToString(Router.Home))
          }>
          {ReasonReact.string("Workhours")}
        </div>
        {switch (self.state) {
         | Loaded([||]) => <div></div>
         | Loaded(_) =>
           <div className="settings">
             <button
               className="settingsButton"
               title="Settings"
               onClick={_ =>
                 Router.Settings
                 |> Router.routeToString
                 |> ReasonReact.Router.push
               }>
             </button>
           </div>
         | _ => <div />
         }}
      </header>
      <section>
        {switch (self.state) {
         | Loading => <p> {str("Loading")} </p>
         | Loaded(workList) =>
           <div>
             {Config.routeToComponent(
                currentRoute, ~workList, ~handleAction=action =>
                self.send(action)
              )}
           </div>
         | Error(msg) => <div> {str(msg)} </div>
         }}
      </section>
    </div>,
};
open Types;

type state =
  | Loading
  | Loaded(work)
  | Saving
  | Failed(string);

type action =
  | SaveWork(work)
  | LoadWork
  | WorkLoaded(work);

let component = ReasonReact.reducerComponent("NewWork");

let str = ReasonReact.string;

let newWork = {
  id: None,
  start: Js.Date.make(),
  duration: 0.,
  lunch: 0.,
  userid: None,
};

let make = (~handleAction: Types.action => unit, _children) => {
  ...component,
  initialState: () => Loading,
  reducer: (action: action, _state: state) =>
    switch (action) {
    | LoadWork =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (self => self.send(WorkLoaded(newWork))),
      )
    | WorkLoaded(work) => ReasonReact.Update(Loaded(work))
    | SaveWork(work) =>
      ReasonReact.UpdateWithSideEffects(
        Saving,
        (
          _self =>
            Js.Promise.(
              Work.save(work)
              |> then_(work =>
                   handleAction(Types.WorkAdd(work))
                   |> (() => ReasonReact.Router.push("/") |> resolve)
                 )
              |> catch(err => {
                   Js.log2("Error starting work: ", err);
                   resolve();
                 })
              |> ignore
            )
        ),
      )
    },
  didMount: self => self.send(LoadWork),
  render: self =>
    switch (self.state) {
    | Loading => <h1> {str("Loading...")} </h1>
    | Loaded(work) =>
      <div>
        <h1> {ReasonReact.string("New registration")} </h1>
        <div>
          <EditForm work submitAction=(work => self.send(SaveWork(work))) />
        </div>
      </div>
    | Saving => <h1> {str("Saving...")} </h1>
    | Failed(msg) =>
      <div> <h1> {str("Failed")} </h1> <p> {str(msg)} </p> </div>
    },
};
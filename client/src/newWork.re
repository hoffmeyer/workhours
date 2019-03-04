open Types;

type state =
  | Loading
  | Loaded(work)
  | Saving
  | Deleting
  | Failed(string);

type action =
  | SaveWork(work)
  | LoadWork
  | Delete(string)
  | WorkLoaded(work)
  | LoadFailed(string);

let component = ReasonReact.reducerComponent("NewWork");

let str = ReasonReact.string;

let newWork = {
  id: None,
  start: Js.Date.make(),
  duration: 0.,
  lunch: 0.,
  userid: None,
};

let findWork = (id, workList) => {
  let matchesId = (work: work): bool =>
    switch (work.id) {
    | None => false
    | Some(workId) => workId === id
    };

  let matches = workList |> Js.Array.filter(matchesId);
  if (Js.Array.length(matches) > 0) {
    Some(matches[0]);
  } else {
    None;
  };
};

let make =
    (
      ~handleAction: Types.action => unit,
      ~workList: array(work),
      ~id: option(string),
      _children,
    ) => {
  ...component,
  initialState: () => Loading,
  reducer: (action: action, _state: state) =>
    switch (action) {
    | LoadWork =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        self =>
          switch (id) {
          | None => self.send(WorkLoaded(newWork))
          | Some(id) =>
            switch (findWork(id, workList)) {
            | None =>
              self.send(LoadFailed("No work with id " ++ id ++ " found"))
            | Some(work) => self.send(WorkLoaded(work))
            }
          },
      )
    | WorkLoaded(work) => ReasonReact.Update(Loaded(work))
    | SaveWork(work) =>
      ReasonReact.UpdateWithSideEffects(
        Saving,
        _self =>
          Js.Promise.(
            Models.Work.save(work)
            |> then_(savedWork =>
                 (
                   switch (work.id) {
                   | None => handleAction(Types.WorkAdd(savedWork))
                   | Some(_) => handleAction(Types.WorkUpdate(savedWork))
                   }
                 )
                 |> (() => ReasonReact.Router.push("/") |> resolve)
               )
            |> catch(err => {
                 Js.log2("Error starting work: ", err);
                 resolve();
               })
            |> ignore
          ),
      )
    | Delete(id) =>
      ReasonReact.UpdateWithSideEffects(
        Deleting,
        self =>
          Js.Promise.(
            Models.Work.delete(id)
            |> then_(() => {
                 handleAction(Types.WorkDelete(id));
                 ReasonReact.Router.push(Router.routeToString(Router.Home))
                 |> resolve;
               })
            |> catch(err => {
                 Js.log2("Error deleting work: ", err);
                 resolve(self.send(LoadFailed("Error deleting work")));
               })
            |> ignore
          ),
      )
    | LoadFailed(msg) => ReasonReact.Update(Failed(msg))
    },
  didMount: self => self.send(LoadWork),
  render: self =>
    switch (self.state) {
    | Loading => <h1> {str("Loading...")} </h1>
    | Loaded(work) =>
      <div>
        <h1>
          {ReasonReact.string(
             switch (work.id) {
             | None => "New work"
             | Some(_) => "Edit work"
             },
           )}
        </h1>
        <div>
          <EditForm
            work
            saveAction={work => self.send(SaveWork(work))}
            deleteAction={id => self.send(Delete(id))}
          />
        </div>
      </div>
    | Saving => <h1> {str("Saving...")} </h1>
    | Deleting => <h1> {str("Deleting...")} </h1>
    | Failed(msg) =>
      <div> <h1> {str("Failed")} </h1> <p> {str(msg)} </p> </div>
    },
};
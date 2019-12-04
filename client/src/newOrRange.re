open Types;

type state =
  | Single
  | Range;

type action =
  | ToSingle
  | ToRange;

let component = ReasonReact.reducerComponent("SingleOrRange");

let title: option(string) => string =
  id =>
    switch (id) {
    | None => "New Work"
    | Some(_) => "Edit Work"
    };

let make =
    (
      ~handleAction: Types.action => unit,
      ~workList: array(work),
      ~id: option(string),
      _children,
    ) => {
  ...component,
  initialState: () => Single,
  reducer: (action: action, _state: state) =>
    switch (action) {
    | ToSingle => ReasonReact.Update(Single)
    | ToRange => ReasonReact.Update(Range)
    },
  render: self =>
    <div>
      <h1> {ReasonReact.string(title(id))} </h1>
      {switch (id) {
       | None =>
         <label>
           {ReasonReact.string("Range")}
           <input
             type_="checkbox"
             checked={self.state === Range}
             onChange={_ =>
               self.send(self.state === Range ? ToSingle : ToRange)
             }
           />
         </label>
       | Some(_) => <div />
       }}
      {switch (self.state) {
       | Single => <NewWork handleAction workList id />
       | Range => <NewWorkRange.Jsx2 handleAction workList />
       }}
    </div>,
};
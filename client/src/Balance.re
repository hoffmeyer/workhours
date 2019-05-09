type state =
  | Initial
  | Loading
  | Loaded(float)
  | Failed(string);

type action =
  | Load
  | Loaded(float)
  | Error(string);

let str = ReasonReact.string;

let component = ReasonReact.reducerComponent("Balance");

let make = _children => {
  ...component,
  initialState: () => Initial,
  didMount: self => self.send(Load),
  reducer: (action, _state) =>
    switch (action) {
    | Load =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        self =>
          Js.Promise.(
            Models.Work.balance
            |> then_(balance => self.send(Loaded(balance)) |> resolve)
            |> ignore
          ),
      )
    | Loaded(balance) => ReasonReact.Update(Loaded(balance))
    | Error(msg) => ReasonReact.Update(Failed(msg))
    },
  render: self =>
    switch (self.state) {
    | Initial => <p> {"Initial" |> str} </p>
    | Loading => <p> {"Loading" |> str} </p>
    | Loaded(balance) =>
      <p className="center">
        {Printf.sprintf("Balance: %.2f", balance) |> str}
      </p>
    | Failed(msg) => <p> {"Fetching balance failed: " ++ msg |> str} </p>
    },
};
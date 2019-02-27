open Types;

type action =
  | LoadUser
  | UserLoaded(user);

type state =
  | Loading
  | Loaded(user)
  | Error(string);

let component = ReasonReact.reducerComponent("Settings");

let make = _children => {
  ...component,
  initialState: _state => Loading,
  didMount: self => self.send(LoadUser),
  reducer: (action, _state) => {
    switch (action) {
    | LoadUser =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        self =>
          Js.Promise.(
            Fetch.fetch("/api/user/")
            |> then_(res => Fetch.Response.json(res))
            |> then_(json =>
                 json
                 |> Decode.user
                 |> (user => self.send(UserLoaded(user)))
                 |> resolve
               )
            |> ignore
          ),
      )
    | UserLoaded(user) => ReasonReact.Update(Loaded(user))
    };
  },
  render: self =>
    switch (self.state) {
    | Loading => <h1> {"Loading..." |> ReasonReact.string} </h1>
    | Loaded(user) =>
      <div>
        <h1> {"Settings" |> ReasonReact.string} </h1>
        <UserForm user />
      </div>
    | Error(err) => <h1> {err |> ReasonReact.string} </h1>
    },
};
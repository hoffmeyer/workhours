open Types;

type action =
  | LoadUser
  | UserLoaded(user)
  | SaveUser(user);

type state =
  | Loading
  | Saving
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
    | SaveUser(user) =>
      ReasonReact.UpdateWithSideEffects(
        Saving,
        _self =>
          Js.Promise.(
            Models.User.save(user)
            |> then_(_savedUser =>
                 ReasonReact.Router.push(Router.Home |> Router.routeToString)
                 |> resolve
               )
            |> resolve
          )
          |> ignore,
      )
    | UserLoaded(user) => ReasonReact.Update(Loaded(user))
    };
  },
  render: self =>
    switch (self.state) {
    | Loading => <h1> {"Loading..." |> ReasonReact.string} </h1>
    | Saving => <h1> {"Saving..." |> ReasonReact.string} </h1>
    | Loaded(user) =>
      <div>
        <h1> {"Settings" |> ReasonReact.string} </h1>
        <UserForm user saveUser={user => self.send(SaveUser(user))} />
      </div>
    | Error(err) => <h1> {err |> ReasonReact.string} </h1>
    },
};
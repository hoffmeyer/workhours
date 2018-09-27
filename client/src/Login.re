type cred = {
  username: string,
  password: string,
};

type action =
  | UsernameKeyDown(string)
  | PasswordKeyDown(string)
  | LoginClicked;

let str = ReasonReact.string;

module Encode = {
  let cred = (m: cred) =>
    Json.Encode.(
      object_([
        ("username", string(m.username)),
        ("password", string(m.password)),
      ])
    );
};

let component = ReasonReact.reducerComponent("Login");

let make = _children => {
  ...component,
  initialState: _state => {username: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | UsernameKeyDown(value) =>
      ReasonReact.Update({...state, username: value})
    | PasswordKeyDown(value) =>
      ReasonReact.Update({...state, password: value})
    | LoginClicked =>
      ReasonReact.UpdateWithSideEffects(
        state,
        (
          _self =>
            Js.Promise.(
              Fetch.fetchWithInit(
                "/api/login",
                Fetch.RequestInit.make(
                  ~method_=Post,
                  ~body=
                    Fetch.BodyInit.make(
                      Js.Json.stringify(Encode.cred(state)),
                    ),
                  (),
                ),
              )
              |> then_(Fetch.Response.text)
              |> then_(text => {
                   print_endline(text);
                   ReasonReact.Router.push("/");
                   resolve();
                 })
              |> ignore
            )
        ),
      )
    },
  render: ({state, send}) =>
    <div>
      <h1> (str("Login page")) </h1>
      <input
        value=state.username
        onChange=(
          event =>
            send(
              UsernameKeyDown(
                ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value,
              ),
            )
        )
      />
      <input
        value=state.password
        onChange=(
          event =>
            send(
              PasswordKeyDown(
                ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value,
              ),
            )
        )
      />
      <button onClick=(_event => send(LoginClicked))>
        ("Login" |> str)
      </button>
    </div>,
};
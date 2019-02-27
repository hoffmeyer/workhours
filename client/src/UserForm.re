open Types;

type action =
  | ChangeHoursPerWeek(float)
  | ChangeBalanceFrom(Js.Date.t)
  | ChangeUsername(string)
  | ChangeName(string)
  | Error(string);

type state =
  | Initial
  | Error(string);

let str = ReasonReact.string;

let component = ReasonReact.reducerComponent("UserForm");

let make = (~user: user, _children) => {
  ...component,
  initialState: () => Initial,
  reducer: (action, _state: state) => {
    switch (action) {
    | ChangeHoursPerWeek(hours) => ReasonReact.NoUpdate
    | ChangeBalanceFrom(date) => ReasonReact.NoUpdate
    | ChangeUsername(username) => ReasonReact.NoUpdate
    | ChangeName(name) => ReasonReact.NoUpdate
    | Error(msg) => ReasonReact.NoUpdate
    };
  },
  render: self =>
    <form>
      <label htmlFor="username"> {"Username" |> str} </label>
      <input
        id="username"
        value={user.username}
        onChange={event =>
          self.send(ChangeUsername(ReactEvent.Form.target(event)##value))
        }
      />
      <label htmlFor="name"> {"Full name" |> str} </label>
      <input
        id="name"
        value={user.name}
        onChange={event =>
          self.send(ChangeName(ReactEvent.Form.target(event)##value))
        }
      />
      <label htmlFor="hoursPerWeek"> {"Hours per week" |> str} </label>
      <input
        id="hoursPerWeek"
        value={user.workhoursPerWeek |> string_of_float}
        onChange={event =>
          self.send(
            ChangeHoursPerWeek(ReactEvent.Form.target(event)##value),
          )
        }
      />
      <label htmlFor="balanceFrom"> {"Calculate balance from" |> str} </label>
      <DateInput
        id="balanceFrom"
        value={user.balanceFrom}
        onChange={date => self.send(ChangeBalanceFrom(date))}
      />
      <button type_="button"> {"Save" |> str} </button>
    </form>,
};
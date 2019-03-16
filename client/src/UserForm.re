open Types;

module UserForm = {
  open Formality;

  type field =
    | HoursPerWeek
    | BalanceFrom;

  type message = string;

  type state = {
    hoursPerWeek: string,
    balanceFrom: Js.Date.t,
  };

  module HoursPerWeekField = {
    let update = (state: state, value: string) => {
      ...state,
      hoursPerWeek: value,
    };
    let validator = {
      field: HoursPerWeek,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: state => {
        let num = state.hoursPerWeek |> Js.Float.fromString;
        switch (num) {
        | _ as value when value == 0. => Error("Must not be empty or zero")
        | _ as value when Js.Float.isNaN(value) => Error("Invalid number")
        | _ as value when value < 0. => Error("Negative hours not allowed")
        | _ => Ok(Valid)
        };
      },
    };
  };

  module BalanceFromField = {
    let update = (state: state, value: Js.Date.t) => {
      ...state,
      balanceFrom: value,
    };
    let validator = {
      field: BalanceFrom,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: state => {
        switch (state.balanceFrom) {
        | _ as value when DateUtil.isAfter(Js.Date.make(), value) =>
          Error("Must not be in the future")
        | _ => Ok(Valid)
        };
      },
    };
  };

  let validators = [HoursPerWeekField.validator, BalanceFromField.validator];

  let formDataToUser = (oldUser, formData): user => {
    {
      ...oldUser,
      workhoursPerWeek: formData.hoursPerWeek |> float_of_string,
      balanceFrom: formData.balanceFrom,
    };
  };

  let userToFormData = user => {
    {
      hoursPerWeek: user.workhoursPerWeek |> string_of_float,
      balanceFrom: user.balanceFrom,
    };
  };
};

type message = string;

module UserFormContainer = Formality.Make(UserForm);

let component = ReasonReact.statelessComponent(__MODULE__);

let make = (~user: user, ~saveUser: user => unit, _children) => {
  ...component,
  render: _ =>
    <UserFormContainer
      initialState={user |> UserForm.userToFormData}
      onSubmit={(state, _form) =>
        UserForm.formDataToUser(user, state) |> saveUser
      }>
      ...{form =>
        <div>
          <form onSubmit={form.submit->Formality.Dom.preventDefault}>
            <label htmlFor="hoursPerWeek">
              {"Hours per week" |> ReasonReact.string}
            </label>
            <input
              id="hoursPerWeek"
              value={form.state.hoursPerWeek}
              onChange={event =>
                form.change(
                  HoursPerWeek,
                  UserForm.HoursPerWeekField.update(
                    form.state,
                    event->ReactEvent.Form.target##value,
                  ),
                )
              }
            />
            {switch (HoursPerWeek->(form.result)) {
             | Some(Error(message)) =>
               <div className={Cn.make(["form-message", "failure"])}>
                 message->ReasonReact.string
               </div>
             | Some(Ok(Valid))
             | Some(Ok(NoValue))
             | None => ReasonReact.null
             }}
            <label htmlFor="balanceFrom">
              {"Calculate balance from" |> ReasonReact.string}
            </label>
            <DateInput
              id="balanceFrom"
              value={form.state.balanceFrom}
              onChange={date =>
                form.change(
                  BalanceFrom,
                  UserForm.BalanceFromField.update(form.state, date),
                )
              }
            />
            {switch (BalanceFrom->(form.result)) {
             | Some(Error(message)) =>
               <div className={Cn.make(["form-message", "failure"])}>
                 message->ReasonReact.string
               </div>
             | Some(Ok(Valid))
             | Some(Ok(NoValue))
             | None => ReasonReact.null
             }}
            <button> {"Save" |> ReasonReact.string} </button>
          </form>
        </div>
      }
    </UserFormContainer>,
};
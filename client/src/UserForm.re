open Types;

type formData = {
  hoursPerWeek: string,
  balanceFrom: Js.Date.t,
};

type action =
  | ChangeHoursPerWeek(string)
  | ChangeBalanceFrom(Js.Date.t)
  | Validate;

type state = {
  formData,
  validationErrors: list(string),
};

let str = ReasonReact.string;

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

let validate = formData => {
  EditForm.validateFloat("Hours per week", formData.hoursPerWeek);
};

let component = ReasonReact.reducerComponent("UserForm");

let make = (~user: user, ~saveUser: user => unit, _children) => {
  ...component,
  initialState: () => {
    formData: user |> userToFormData,
    validationErrors: [],
  },
  reducer: (action, state: state) => {
    switch (action) {
    | ChangeHoursPerWeek(hours) =>
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          hoursPerWeek: hours,
        },
      })
    | ChangeBalanceFrom(date) =>
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          balanceFrom: date,
        },
      })
    | Validate =>
      let errors = validate(state.formData);
      switch (errors) {
      | [] =>
        ReasonReact.SideEffects(
          _ => saveUser(state.formData |> formDataToUser(user)),
        )
      | err => ReasonReact.Update({...state, validationErrors: err})
      };
    };
  },
  render: self =>
    <div>
      {switch (self.state.validationErrors) {
       | [] => <div />
       | errors =>
         ReasonReact.array(
           Js.Array.mapi(
             (error, i) =>
               <div key={string_of_int(i)} className="error">
                 {error |> str}
               </div>,
             Array.of_list(errors),
           ),
         )
       }}
      <form>
        <label htmlFor="hoursPerWeek"> {"Hours per week" |> str} </label>
        <input
          id="hoursPerWeek"
          value={self.state.formData.hoursPerWeek}
          onChange={event =>
            self.send(
              ChangeHoursPerWeek(ReactEvent.Form.target(event)##value),
            )
          }
        />
        <label htmlFor="balanceFrom">
          {"Calculate balance from" |> str}
        </label>
        <DateInput
          id="balanceFrom"
          value={self.state.formData.balanceFrom}
          onChange={date => self.send(ChangeBalanceFrom(date))}
        />
        <button type_="button" onClick={_ => self.send(Validate)}>
          {"Save" |> str}
        </button>
      </form>
    </div>,
};
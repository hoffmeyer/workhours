open Types;

type formData = {
  id: option(string),
  start: Js.Date.t,
  duration: string,
  lunch: string,
  userid: option(string),
};

type state = {
  formData,
  validationErrors: list(string),
};

type action =
  | ChangeStartDate(Js.Date.t)
  | ChangeStartTime(Js.Date.t)
  | ChangeDuration(string)
  | ChangeLunch(string)
  | Validate;

let workToFormData = (work: work): formData => {
  {
    id: work.id,
    start: work.start,
    duration: work.duration |> string_of_float,
    lunch: work.lunch |> string_of_float,
    userid: work.userid,
  };
};

let formatFloatString = str => Js.String.replace(",", ".", str);

let formDataToWork = (formData: formData): work => {
  {
    id: formData.id,
    start: formData.start,
    duration: formData.duration |> formatFloatString |> float_of_string,
    lunch: formData.lunch |> formatFloatString |> float_of_string,
    userid: formData.userid,
  };
};

let isNan = (fieldName, num) =>
  Js.Float.isNaN(num) ? [fieldName ++ " is not a valid number"] : [];

let isPositive = (fieldName, num) =>
  num < 0. ? [fieldName ++ " must not be negative"] : [];

let validateFloat = (fieldName, value) => {
  let num = Js.Float.fromString(value |> formatFloatString);
  Belt.List.concat(isNan(fieldName, num), isPositive(fieldName, num));
};

let validate = formData => {
  let duration = validateFloat("Duration", formData.duration);
  let lunch = validateFloat("Lunch", formData.lunch);
  Belt.List.concat(duration, lunch);
};

let str = ReasonReact.string;

let component = ReasonReact.reducerComponent("EditForm");

let make = (~work: work, ~submitAction: Types.work => unit, _children) => {
  ...component,
  initialState: () => {
    formData: work |> workToFormData,
    validationErrors: [],
  },
  reducer: (action: action, state: state) =>
    switch (action) {
    | ChangeStartDate(date) =>
      Js.Date.setHours(date, Js.Date.getHours(state.formData.start))
      |> ignore;
      Js.Date.setMinutes(date, Js.Date.getMinutes(state.formData.start))
      |> ignore;
      Js.Date.setSeconds(date, Js.Date.getSeconds(state.formData.start))
      |> ignore;
      Js.Date.setMilliseconds(
        date,
        Js.Date.getMilliseconds(state.formData.start),
      )
      |> ignore;
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          start: date,
        },
      });
    | ChangeStartTime(date) =>
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          start: date,
        },
      })
    | ChangeDuration(duration) =>
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          duration,
        },
      })
    | ChangeLunch(lunch) =>
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          lunch,
        },
      })
    | Validate =>
      let errors = validate(state.formData);
      switch (errors) {
      | [] =>
        submitAction(formDataToWork(state.formData))
        |> (_ => ReasonReact.NoUpdate)
      | err => ReasonReact.Update({...state, validationErrors: err})
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
      <form id="registerHours">
        <label htmlFor="startDate"> {"Start date" |> str} </label>
        <DateInput
          id="startDate"
          value={self.state.formData.start}
          onChange={date => self.send(ChangeStartDate(date))}
        />
        <label htmlFor="startTime"> {"Start time" |> str} </label>
        <TimeInput
          id="startTime"
          value={self.state.formData.start}
          onChange={date => self.send(ChangeStartTime(date))}
        />
        <label htmlFor="duration"> {"Duration" |> str} </label>
        <input
          id="duration"
          value={self.state.formData.duration}
          onChange={event =>
            self.send(ChangeDuration(ReactEvent.Form.target(event)##value))
          }
        />
        <label htmlFor="lunch"> {"Lunch" |> str} </label>
        <input
          id="lunch"
          value={self.state.formData.lunch}
          onChange={event =>
            self.send(ChangeLunch(ReactEvent.Form.target(event)##value))
          }
        />
        <button type_="button" onClick={_event => self.send(Validate)}>
          {(
             switch (work.id) {
             | None => "Add"
             | Some(_) => "Save"
             }
           )
           |> str}
        </button>
      </form>
    </div>,
};
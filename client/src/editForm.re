open Types;

type formData = {
  id: option(string),
  startDate: Js.Date.t,
  endDate: Js.Date.t,
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
  | ChangeEndTime(Js.Date.t)
  | ChangeDuration(string)
  | ChangeLunch(string)
  | Validate;

let calcEndDate = (startDate: Js.Date.t, duration: float): Js.Date.t => {
  let endDate = Js.Date.fromFloat(Js.Date.getTime(startDate));
  Js.Date.setHours(endDate, Js.Date.getHours(startDate) +. ceil(duration))
  |> ignore;
  Js.Date.setMinutes(
    endDate,
    Js.Date.getMinutes(startDate) +. mod_float(duration, 1.),
  )
  |> ignore;
  endDate;
};

let calcDuration = (startDate: Js.Date.t, endDate: Js.Date.t): float => {
  let diffMs = (endDate |> Js.Date.getTime) -. (startDate |> Js.Date.getTime);
  diffMs /. 1000. /. 60. /. 60.;
};

let workToFormData = (work: work): formData => {
  {
    id: work.id,
    startDate: work.start,
    endDate: calcEndDate(work.start, work.duration),
    duration: work.duration |> Printf.sprintf("%.12g"),
    lunch: work.lunch |> Printf.sprintf("%.12g"),
    userid: work.userid,
  };
};

let formatFloatString = str => Js.String.replace(",", ".", str);

let formDataToWork = (formData: formData): work => {
  {
    id: formData.id,
    start: formData.startDate,
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
      Js.Date.setHours(date, Js.Date.getHours(state.formData.startDate))
      |> ignore;
      Js.Date.setMinutes(date, Js.Date.getMinutes(state.formData.startDate))
      |> ignore;
      Js.Date.setSeconds(date, Js.Date.getSeconds(state.formData.startDate))
      |> ignore;
      Js.Date.setMilliseconds(
        date,
        Js.Date.getMilliseconds(state.formData.startDate),
      )
      |> ignore;
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          startDate: date,
        },
      });
    | ChangeStartTime(date) =>
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          startDate: date,
          duration:
            calcDuration(state.formData.startDate, date) |> string_of_float,
        },
      })
    | ChangeEndTime(date) =>
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          endDate: date,
          duration:
            calcDuration(state.formData.startDate, date) |> string_of_float,
        },
      })
    | ChangeDuration(duration) =>
      ReasonReact.Update({
        ...state,
        formData: {
          ...state.formData,
          endDate:
            calcEndDate(
              state.formData.startDate,
              duration |> float_of_string,
            ),
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
          value={self.state.formData.startDate}
          onChange={date => self.send(ChangeStartDate(date))}
        />
        <label htmlFor="startTime"> {"Start time" |> str} </label>
        <TimeInput
          id="startTime"
          value={self.state.formData.startDate}
          onChange={date => self.send(ChangeStartTime(date))}
        />
        <label htmlFor="endTime"> {"End time" |> str} </label>
        <TimeInput
          id="endTime"
          value={self.state.formData.endDate}
          onChange={date => self.send(ChangeEndTime(date))}
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
        <button
          type_="button" onClick={_event => ReasonReact.Router.push("/")}>
          {"Cancel" |> str}
        </button>
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
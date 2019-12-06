/* In Image.re */
[@bs.config {jsx: 3}];

open Types;

type formData = {
  startDate: Js.Date.t,
  endDate: Js.Date.t,
  duration: string,
};

type state = {
  formData,
  validationErrors: list(string),
};

let initState = {
  formData: {
    startDate: Js.Date.make(),
    endDate: Js.Date.make(),
    duration: ""
  },
  validationErrors: [],
};

let formDataToWorkRange = (fd:formData): workRange => {
  {
    startDate: fd.startDate,
    endDate: fd.endDate,
    duration: Js.Float.fromString(fd.duration)
  }
};

[@react.component]
let make = (~handleAction) => {
  let (state, setState) = React.useState(() => initState);
  <div>
    <form id="registerHours">
      <label htmlFor="startDate"> {"Start date" |> React.string} </label>
      <DateInput
        id="startDate"
        value={state.formData.startDate}
        onChange={date =>
          setState(state =>
            {
              ...state,
              formData: {
                ...state.formData,
                startDate: date,
              },
            }
          )
        }
      />
      <label htmlFor="endDate"> {"End date" |> React.string} </label>
      <DateInput
        id="endDate"
        value={state.formData.endDate}
        onChange={date =>
          setState(state =>
            {
              ...state,
              formData: {
                ...state.formData,
                endDate: date,
              },
            }
          )
        }
      />
      <label htmlFor="duration"> {"Duration" |> React.string} </label>
      <input
        id="duration"
        value={state.formData.duration}
        onChange={event => {
          let value = ReactEvent.Form.target(event)##value;
          setState(state =>
            {
              ...state,
              formData: {
                ...state.formData,
                duration: value,
              },
            }
          )
        }
        }
      />
      <div className="actionButtons">
        <button
          className="actionButtons__cancel"
          type_="button"
          onClick={_event => ReasonReact.Router.push("/")}>
          {"Cancel" |> React.string}
        </button>
        <button
          className="actionButtons__add"
          type_="button"
          onClick={_event => {
            Js.Promise.(
            Models.Work.saveRange(formDataToWorkRange(state.formData))
            |> then_(workList => {
              workList |> Array.iter( w => handleAction(WorkAdd(w)));
              resolve();
            })
            |> catch(err => {
                Js.log2("Error saving  work range: ", err);
                resolve();
            })
            |> ignore
            );
          }}>
          {"Add" |> React.string}
        </button>
      </div>
    </form>
  </div>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("NewWorkRange");
  /* `children` is not labelled, as it is a regular parameter in version 2 of JSX */
  let make = (~handleAction, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~handleAction, ()),
      children,
    );
};
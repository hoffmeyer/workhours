/* In Image.re */
[@bs.config {jsx: 3}];

type formData = {
  startDate: Js.Date.t,
  endDate: Js.Date.t,
  duration: string,
  userid: option(string),
};

type state = {
  formData,
  validationErrors: list(string),
};

let initState = {
  formData: {
    startDate: Js.Date.make(),
    endDate: Js.Date.make(),
    duration: "",
    userid: None,
  },
  validationErrors: [],
};

let getValueFromEvent = (event): string =>
  (event |> ReactEvent.Form.target )##value;

[@react.component]
let make = (~handleAction, ~workList, ~id) => {
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
        onChange={event =>
          setState(state =>
            {
              ...state,
              formData: {
                ...state.formData,
                duration: getValueFromEvent(event),
              },
            }
          )
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
          onClick={_event => Js.log("Dakedak")}>
          {"Add" |> React.string}
        </button>
      </div>
    </form>
  </div>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("NewWorkRange");
  /* `children` is not labelled, as it is a regular parameter in version 2 of JSX */
  let make = (~handleAction, ~workList, ~id, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~handleAction, ~workList, ~id, ()),
      children,
    );
};
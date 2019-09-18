/* In Image.re */
[@bs.config {jsx: 3}];

let addZ = num => (num < 10.) ? "0" ++ Js.Float.toString(num) : Js.Float.toString(num);
let toString = date => 
 Js.Float.toString(Js.Date.getFullYear(date))
  ++ "-"
  ++ addZ(Js.Date.getMonth(date) +. 1.)
  ++ "-"
  ++ addZ(Js.Date.getDate(date));


let toDate = Js.Date.fromString;

[@react.component]
let make = (~value: Js.Date.t, ~id: string, ~onChange ) =>
    <input
      id
      type_="date"
      value={value |> toString}
      onChange={
        event => onChange(ReactEvent.Form.target(event)##value |> toDate)
      }
    />;

module Jsx2 = {
  let component = ReasonReact.statelessComponent("DateInput");
  /* `children` is not labelled, as it is a regular parameter in version 2 of JSX */
  let make = (~value, ~id, ~onChange, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~value, ~id, ~onChange, ()),
      children,
    );
};
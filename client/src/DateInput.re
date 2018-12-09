let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("DateInput");

let addZ = num => (num < 10.) ? "0" ++ Js.Float.toString(num) : Js.Float.toString(num);
let toString = date => 
 Js.Float.toString(Js.Date.getFullYear(date))
  ++ "-"
  ++ addZ(Js.Date.getMonth(date) +. 1.)
  ++ "-"
  ++ addZ(Js.Date.getDate(date));


let toDate = Js.Date.fromString;

let make = (~value: Js.Date.t, ~id: string, ~onChange, _children) => {
  ...component,
  render: _self =>
    <input
      id
      type_="date"
      value={value |> toString}
      onChange={
        event => onChange(ReactEvent.Form.target(event)##value |> toDate)
      }
    />,
};
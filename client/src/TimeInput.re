let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("TimeInput");

let addZ = num =>
  num < 10. ? "0" ++ Js.Float.toString(num) : Js.Float.toString(num);

let toString = date =>
  addZ(Js.Date.getHours(date)) ++ ":" ++ addZ(Js.Date.getMinutes(date));

let toDate = (date: Js.Date.t, timeString) =>  {
  let vals = Js.String.split(":", timeString);
  let hours = vals[0];
  let minutes = vals[1];
  Js.Date.setHours(date, Js.Float.fromString(hours)) |> ignore
  Js.Date.setMinutes(date, Js.Float.fromString(minutes)) |> ignore
  date;
};

let make = (~value: Js.Date.t, ~id: string, ~onChange, _children) => {
  ...component,
  render: _self =>
    <input
      id
      type_="time"
      value={value |> toString}
      onChange={
        event => onChange(ReactEvent.Form.target(event)##value |> toDate(value))
      }
    />,
};